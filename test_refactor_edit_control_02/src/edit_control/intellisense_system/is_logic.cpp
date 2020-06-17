#include "pch.h"
#include "intelisense.h"


void IntelliSense::Analise_Line(std::wstring& Line, std::vector<COLORREF>& Colors) {
    if (Line.size() == 0) return;
    if (Line.at(0) == L'#') {
        for (size_t Pos = 0; Pos < Colors.size(); Pos++)
            Colors.at(Pos) = RGB(100, 100, 100);
        return;
    }

    Start_New_Line();   //Reset all parameters - fresh start

    std::vector<wchar_t> Brackets;
    size_t Last_Pos = 0;
    bool New_Last_Pos = true;
    size_t Line_Size = Line.size();

    bool Is_String = false, Skip = false, Single_Q = false;
    //data get block 0 0 0
    for (size_t Pos = 0; Pos < Line_Size; Pos++) {
        wchar_t ch = Line.at(Pos);

        if (Skip) { Skip = false; continue; }
        if (Is_String) {
            if (ch == L'\\') {
                Skip = true;
                continue;
            }

            if ((!Single_Q && ch == L'"') || (Single_Q && ch == L'\'')) {
                Is_String = false;
            } else {
                continue;
            }
        }

        else if (ch == L'"' || ch == L'\'') {
            Is_String = true;
            Single_Q = (ch == L'\'');
            continue;
        }

        switch (ch) {
        case L'[': Brackets.push_back(L'['); break;
        case L'{': Brackets.push_back(L'{'); break;

        default:
            if (Brackets.size() == 0 && (ch == L'}' || ch == L']')) {
                for (size_t CPos = Last_Pos; CPos < Colors.size(); CPos++)
                    Colors.at(CPos) = RGB(255, 0, 0);
                Error_Handler << L"Brackets don't add up";
                return;
            }
            
            else {
                switch (ch) {
                case L'}':
                    if (Brackets.back() == L'{') Brackets.pop_back();
                    else {
                        for (size_t CPos = Last_Pos; CPos < Colors.size(); CPos++)
                            Colors.at(CPos) = RGB(255, 0, 0);
                        Error_Handler << L"Brackets don't add up";
                        return;
                    }
                    break;
                case L']':
                    if (Brackets.back() == L'[') Brackets.pop_back();
                    else {
                        for (size_t CPos = Last_Pos; CPos < Colors.size(); CPos++)
                            Colors.at(CPos) = RGB(255, 0, 0);
                        Error_Handler << L"Brackets don't add up";
                        return;
                    }
                    break;
                }
            }
        }

        if (Pos + 1 == Line_Size && Brackets.size() > 0) {
            for (size_t CPos = Last_Pos; CPos < Colors.size(); CPos++)
                Colors.at(CPos) = RGB(255, 0, 0);
            Error_Handler << L"Brackets don't add up";
            return;
        }
        
        if ((ch == L' ' || Pos + 1 == Line_Size) && Brackets.size() == 0) {

            if (m_Syntax_Obj.Has_Name(L"children")) {
                m_Syntax_Obj.Obj(L"children");
                
                std::wstring Word = Line.substr(Last_Pos, Pos - Last_Pos + (ch != L' '));   //Get correct word
                auto Names = m_Syntax_Obj.Get_Names();  //"effect", "give", "tp", ...

                std::wstring Result;
                bool Found_Match = false;
                for (size_t NPos = 0; NPos < Names.size(); NPos++) {

                    if (m_Syntax_Obj.Has_Name(Names.at(NPos)))
                        m_Syntax_Obj.Obj(Names.at(NPos));

                    if (m_Syntax_Obj.Has_Name(L"type")) {
                        auto Type = m_Syntax_Obj.Obj(L"type").Str();
                        m_Syntax_Obj.Back();

                        if (Type == L"literal") {
                            if (Names.at(NPos) == Word) {
                                for (size_t CPos = Last_Pos; CPos < Pos + 1; CPos++)
                                    Colors.at(CPos) = Get_Color(Word);
                                Found_Match = true;
                            }
                        }

                        else if (Type == L"argument") {
                            m_Syntax_Obj.Obj(L"parser");

                            Result = Parser_Matcher(Word);

                            if (Result != L"ERROR") {
                                for (size_t CPos = Last_Pos; CPos < Pos + 1; CPos++)
                                    Colors.at(CPos) = Get_Color(Result);
                                Found_Match = true;
                            }
                        }

                        if (Found_Match) break;
                    }


                    m_Syntax_Obj.Back();
                }

                if (!Found_Match) {
                    for (size_t CPos = Last_Pos; CPos < Colors.size(); CPos++)
                        Colors.at(CPos) = RGB(255, 0, 0);
                    Log_IO::Set_Color::Error();
                    Log_IO::wcout() << L"No match is found ("<< Word <<") ignoring rest of the line" << std::endl;
                    Log_IO::Set_Color::Default();
                    return;
                }

                //if (Result != L"INCOMPLETE")  //not necessary?
                if (!m_Paraser_Set_Lock)
                    Last_Pos = Pos + 1;

                if (!m_Paraser_Set_Lock && m_Syntax_Obj.Has_Name(L"redirect")) {
                    m_Syntax_Obj.Obj(L"redirect");

                    std::vector<std::wstring> Recirect;
                    for (size_t Elm = 0; Elm < m_Syntax_Obj.Size(); Elm++) {
                        Recirect.push_back(m_Syntax_Obj.Arr(Elm).Str());
                        m_Syntax_Obj.Back();
                    }

                    m_Syntax_Obj.Root();
                    for (size_t a = 0; a < Recirect.size(); a++) {
                        m_Syntax_Obj.Obj(L"children").Obj(Recirect.at(a));
                    }
                }
            }

            else {
                for (size_t CPos = Last_Pos; CPos < Colors.size(); CPos++)
                    Colors.at(CPos) = RGB(255, 0, 0);
                Log_IO::Set_Color::Error();
                Log_IO::wcout() << L"Command does not take any more paramaters" << std::endl;
                Log_IO::Set_Color::Default();
            }

            if (m_Paraser_Set_Lock) {
                m_Syntax_Obj.Back().Back();
            }

        }
    }

    m_Syntax_Obj.Size();
}
