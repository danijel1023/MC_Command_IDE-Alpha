#include "pch.h"
#include "edit_control/_edit_control/edit_control.h"
#include "history_defines.h"



HGLOBAL Keyboard::Convert_Str_To_Global_Object(const std::wstring& String_Copy) {
    const size_t Len = (String_Copy.size() + 1) * sizeof(WCHAR);
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, Len);
    CHECK_ERR(hMem, ERR_MSG_GLOBAL_ALLOC);

    LPVOID GL_Ptr = GlobalLock(hMem);
    CHECK_ERR(GL_Ptr, ERR_MSG_GLOBAL_LOCK);
    memcpy(GL_Ptr, String_Copy.c_str(), Len);

    if (GlobalUnlock(hMem) == NULL && GetLastError() != NO_ERROR)
        CHECK_ERR(NULL, ERR_MSG_GLOBAL_UNLOCK);
    return hMem;
}


void Keyboard::Convert_Global_Object_To_Str(HGLOBAL hMem, std::wstring* Converted_Str) {
    PWCHAR pchData = (PWCHAR)GlobalLock(hMem);
    CHECK_ERR(pchData, ERR_MSG_GLOBAL_LOCK);

    (*Converted_Str) = pchData;
    if (GlobalUnlock(hMem) == NULL && GetLastError() != NO_ERROR)
        CHECK_ERR(NULL, ERR_MSG_GLOBAL_UNLOCK);
}


void Keyboard::Extract_Selected(std::vector<std::wstring> const& Text,
                                                     Caret_Struct& Caret, Caret_Struct& CaretSel,
                                                     std::vector<std::wstring>* Dest) {
    Caret_Struct Start, End;
    m_EC.Differentiate_Start_End(&Start, &End, Caret, CaretSel);
    for (size_t Line = Start.Y; Line <= End.Y; Line++) {
        if (Line == Start.Y && Line == End.Y) {
            (*Dest).push_back(Text.at(Line).substr(Start.X, End.X - Start.X));
        }
        else if (Line == Start.Y) {
            (*Dest).push_back(Text.at(Line).substr(Start.X));
        }
        else if (Line == End.Y) {
            (*Dest).push_back(Text.at(Line).substr(0, End.X));
        }
        else {
            (*Dest).push_back(Text.at(Line));
        }
    }
}


bool Keyboard::Delete_Selected_Unordered(Caret_Struct& Caret, Caret_Struct& CaretSel, bool Record_History) {
    Caret_Struct Start, End;
    m_EC.Differentiate_Start_End(&Start, &End, CaretSel, Caret);
    return Delete_Selected(Start, End, Record_History);
}


bool Keyboard::Delete_Selected(Caret_Struct& Sel_Start, Caret_Struct& Sel_End, bool Record_History) {
    if (Sel_Start == Sel_End) return false;

    if (Record_History)
        Register_History(HISTORY_MSG_DEL_SEL, m_EC.m_Caret, m_EC.m_CaretSel, m_EC.m_Text);

    if (Sel_Start.Y != Sel_End.Y) { //multiline
        size_t Str_Size = 0;
        size_t& Y_End_Pos = Sel_End.Y;
        bool First_It = true;
        for (Y_End_Pos; Y_End_Pos > Sel_Start.Y; Y_End_Pos--) {
            if (First_It) {
                auto& Str_OnStart = m_EC.m_Text.at(Sel_Start.Y);
                auto& Clr_OnStart = m_EC.m_TextColor.at(Sel_Start.Y);
                Str_OnStart.erase(Str_OnStart.begin() + Sel_Start.X, Str_OnStart.end());
                Clr_OnStart.erase(Clr_OnStart.begin() + Sel_Start.X, Clr_OnStart.end());

                auto& Str = m_EC.m_Text.at(Y_End_Pos);
                auto& Clr = m_EC.m_TextColor.at(Y_End_Pos);
                Str.erase(Str.begin(), Str.begin() + Sel_End.X);
                Clr.erase(Clr.begin(), Clr.begin() + Sel_End.X);
                Str_OnStart += Str;
                Clr_OnStart += Clr;
                Str_Size = Str.size();
                First_It = false;
            }
            m_EC.m_Text.erase(m_EC.m_Text.begin() + Y_End_Pos);
            m_EC.m_TextColor.erase(m_EC.m_TextColor.begin() + Y_End_Pos);
        }
        m_EC.Set_Lines(m_EC.m_Text.size());

        m_EC.m_Text.shrink_to_fit();
        m_EC.m_TextColor.shrink_to_fit();

        Calculate_Max_Line_Length();
    }

    else {  //singleline
        auto& Str = m_EC.m_Text.at(Sel_Start.Y);
        auto& Clr = m_EC.m_TextColor.at(Sel_Start.Y);
        Str.erase(Str.begin() + Sel_Start.X, Str.begin() + Sel_End.X);
        Clr.erase(Clr.begin() + Sel_Start.X, Clr.begin() + Sel_End.X);
        if (m_EC.m_MaxLine_Length.Line_Pos == Sel_Start.Y) Calculate_Max_Line_Length();
    }

    m_EC.m_Caret = Sel_Start;
    m_EC.m_CaretSel = m_EC.m_Caret;
    m_EC.m_Caret_Virtual_X = m_EC.m_Caret.X;
    CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
    return true;
}


void Keyboard::Calculate_Max_Line_Length() {
    size_t Max_Length = 0;
    size_t Max_Pos = 0;

    for (size_t i = 0; i < m_EC.m_Text.size(); i++) {
        if (m_EC.m_Text.at(i).size() > Max_Length) {
            Max_Length = m_EC.m_Text.at(i).size();
            Max_Pos = i;
        }
    }

    m_EC.m_MaxLine_Length.Line_Lenght = Max_Length;
    m_EC.m_MaxLine_Length.Line_Pos = Max_Pos;

    m_EC.m_Horz_Scrollbar.Set_Range((int)m_EC.m_MaxLine_Length.Line_Lenght + (int)m_EC.m_Screen_MaxX - 20);
}


std::wstring Keyboard::Format_To_External(const std::vector<std::wstring>& Formated_Data) {
    std::wstring Output_Data;

    size_t F_Data_Size = Formated_Data.size();
    for (size_t Line = 0; Line < F_Data_Size; Line++) {
        Output_Data += Formated_Data.at(Line);

        if (Line + 1 != F_Data_Size) {
            Output_Data += L"\r\n";
        }
    }

    return Output_Data;
}


std::vector<std::wstring> Keyboard::Format_To_Internal(const std::wstring& UnFormated_Data) {
    std::vector<std::wstring> Output_Data;

    size_t Last_Pos = 0;
    size_t Y_Pos = 0;
    size_t UF_Data_Size = UnFormated_Data.size();
    std::wstring Sub_Str;
    for (size_t Char = 0; Char < UF_Data_Size; Char++) {
        if (UnFormated_Data.at(Char) == L'\n' || Char + 1 == UF_Data_Size) {
            if (Char + 1 == UF_Data_Size && UnFormated_Data.at(Char) != L'\n')
                Sub_Str = UnFormated_Data.substr(Last_Pos, Char + 1);
            else
                Sub_Str = UnFormated_Data.substr(Last_Pos, (Char - 1) - Last_Pos);

            Output_Data.push_back(Sub_Str);

            if (Char + 1 == UF_Data_Size && UnFormated_Data.at(Char) == L'\n') {
                Output_Data.push_back(L"");
            }

            Y_Pos++;
            Last_Pos = Char + 1;
        }
    }

    return Output_Data;
}


std::vector<COLORREF> Keyboard::Generate_Clr(const std::wstring& Text, const Caret_Struct& Caret) {
    std::vector<COLORREF> Clr;
    size_t Text_Size = Text.size();
    for (size_t i = 0; i < Text_Size; i++) {
        Clr.push_back(m_EC.Get_Last_Color(Caret));
    }
    return Clr;
}


void Keyboard::Insert(const std::vector<std::wstring>& Text, Caret_Struct Caret) {
    std::wstring Str_Rest_Of_Line;
    std::vector<COLORREF> Clr_Rest_Of_Line;

    size_t Text_Size = Text.size();
    if (Text_Size == 0) return;

    for (size_t Line = 0; Line < Text_Size; Line++) {
        if (Line == 0) {
            auto& Str = m_EC.m_Text.at(Caret.Y);
            auto& Clr = m_EC.m_TextColor.at(Caret.Y);

            Str_Rest_Of_Line.insert(Str_Rest_Of_Line.begin(), Str.begin() + Caret.X, Str.end());
            Clr_Rest_Of_Line.insert(Clr_Rest_Of_Line.begin(), Clr.begin() + Caret.X, Clr.end());

            Str.erase(Str.begin() + Caret.X, Str.end());
            Clr.erase(Clr.begin() + Caret.X, Clr.end());

            Str += Text.at(Line);
            Clr += Generate_Clr(Text.at(Line), Caret);
            Caret.X += Text.at(Line).size();
        }
        
        else {
            Caret.X = 0;
            m_EC.m_Text.insert(m_EC.m_Text.begin() + Caret.Y + Line, Text.at(Line));
            m_EC.m_TextColor.insert(m_EC.m_TextColor.begin() + Caret.Y + Line, Generate_Clr(Text.at(Line), Caret));
            Caret.X = Text.at(Line).size();
        }
    }

    Caret.Y += Text.size() - 1;
    m_EC.m_Text.at(Caret.Y) += Str_Rest_Of_Line;
    m_EC.m_TextColor.at(Caret.Y) += Clr_Rest_Of_Line;

    m_EC.m_Caret = Caret;
    m_EC.m_CaretSel = m_EC.m_Caret;
    m_EC.m_Caret_Virtual_X = m_EC.m_Caret.X;
    m_EC.Set_Lines(m_EC.m_Text.size());
}
