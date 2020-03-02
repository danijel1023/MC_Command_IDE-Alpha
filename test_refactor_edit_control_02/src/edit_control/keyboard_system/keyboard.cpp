#include "pch.h"
#include "edit_control/_edit_control/edit_control.h"
#include "history_defines.h"
//#include "keyboard.h" --> It's already included in edit_control.h



Keyboard::Keyboard(Edit_Control& EC)
    :m_EC(EC) {}

Keyboard::~Keyboard() {}


void Keyboard::Init(HWND hWnd) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_KEYBOARD);
    Log_IO::wcout() << "Init" << std::endl;
    m_hWnd = hWnd;
}


void Keyboard::Proc_Msg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    m_EC.m_Dispatcher.Return = false;

    switch (uMsg) {
    case WM_SYSKEYDOWN:
        WM_SysKeyDown(wParam, lParam);
        if (m_EC.m_Dispatcher.Return) return;
        break;
    case WM_KEYDOWN:
        WM_KeyDown(wParam, lParam);
        if (m_EC.m_Dispatcher.Return) return;
        break;
    case WM_CHAR:
        WM_Char(wParam, lParam);
        if (m_EC.m_Dispatcher.Return) return;
        break;
    }
}


void Keyboard::WM_SysKeyDown(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_KEYBOARD);
    Log_IO::wcout() << "WM_SysKeyDown" << std::endl;
    m_EC.m_Dispatcher.Return_Val = 0;
    m_EC.m_Dispatcher.Return = true;

}


void Keyboard::WM_KeyDown(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_KEYBOARD);
    m_EC.m_Dispatcher.Return_Val = 0;
    m_EC.m_Dispatcher.Return = true;

    if (HIBYTE(GetKeyState(VK_CONTROL)) != 0 && HIBYTE(GetKeyState(VK_MENU)) == 0) {
        switch (wParam) {
        case 'C':
            if ((0x40000000 & lParam) == 0) Copy();
            break;

        case 'X':
            if ((0x40000000 & lParam) == 0) Cut();
            Calculate_Max_Line_Length();
            break;

        case 'V':
            Paste();
            Calculate_Max_Line_Length();
            break;

        case 'A':
            m_EC.m_CaretSel = { 0, 0 };
            m_EC.m_Caret.Y = m_EC.m_Text.size() - 1;
            m_EC.m_Caret.X = m_EC.m_Text.back().size();
            CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
            break;

        case 'Z':
            UnDo();
            break;

        case 'Y':
            ReDo();
            break;

        case VK_HOME:
            m_EC.m_Caret.X = 0;
            m_EC.m_Caret.Y = 0;
            m_EC.m_CaretSel = m_EC.m_Caret;
            break;

        case VK_END:
            m_EC.m_Caret.X = 0;
            m_EC.m_Caret.Y = m_EC.m_Text.size() - 1;
            m_EC.m_CaretSel = m_EC.m_Caret;
            break;
        }
    }
    
    else {
        switch (wParam) {
        case VK_HOME:
            m_EC.m_Caret.X = 0;
            m_EC.m_Caret_Virtual_X = m_EC.m_Caret.X;
            m_EC.m_CaretSel = m_EC.m_Caret;
            CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
            break;

        case VK_END:
            m_EC.m_Caret.X = m_EC.m_Text.at(m_EC.m_Caret.Y).size();
            m_EC.m_Caret_Virtual_X = m_EC.m_Caret.X;
            m_EC.m_CaretSel = m_EC.m_Caret;
            CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
            break;

        case VK_PRIOR:  //Page up
            Log_IO::wcout() << "WM_KeyDown - VK_PRIOR" << std::endl;
            if (m_EC.m_Caret.Y < m_EC.m_Screen_MaxY)
                m_EC.m_Caret.Y = 0;
            else
                m_EC.m_Caret.Y -= m_EC.m_Screen_MaxY;

            m_EC.m_Caret.X = m_EC.m_Caret_Virtual_X;
            if (m_EC.m_Caret.X > m_EC.m_Text.at(m_EC.m_Caret.Y).size())
                m_EC.m_Caret.X = m_EC.m_Text.at(m_EC.m_Caret.Y).size();

            m_EC.m_CaretSel = m_EC.m_Caret;
            CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
            break;

        case VK_NEXT:  //Page down
            Log_IO::wcout() << "WM_KeyDown - VK_NEXT" << std::endl;
            m_EC.m_Caret.Y += m_EC.m_Screen_MaxY;
            if (m_EC.m_Caret.Y + 1 >= m_EC.m_Text.size())
                m_EC.m_Caret.Y = m_EC.m_Text.size() - 1;

            m_EC.m_Caret.X = m_EC.m_Caret_Virtual_X;
            if (m_EC.m_Caret.X > m_EC.m_Text.at(m_EC.m_Caret.Y).size())
                m_EC.m_Caret.X = m_EC.m_Text.at(m_EC.m_Caret.Y).size();

            m_EC.m_CaretSel = m_EC.m_Caret;
            CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
            break;

        case VK_LEFT:
            Log_IO::wcout() << "WM_KeyDown - VK_LEFT" << std::endl;
            if (HIBYTE(GetKeyState(VK_SHIFT)) == 0 && m_EC.m_Caret != m_EC.m_CaretSel) {
                Caret_Struct Start, End;
                m_EC.Differentiate_Start_End(&Start, &End, m_EC.m_Caret, m_EC.m_CaretSel);
                m_EC.m_Caret = Start;
                m_EC.m_CaretSel = m_EC.m_Caret;
            }

            else {
                if ((m_EC.m_Caret.Y != 0 || m_EC.m_Caret.X != 0)) {
                    if (m_EC.m_Caret.X != 0) {
                        m_EC.m_Caret.X--;
                    } else {
                        m_EC.m_Caret.Y--;
                        m_EC.m_Caret.X = m_EC.m_Text.at(m_EC.m_Caret.Y).size();
                    }
                }

                if (HIBYTE(GetKeyState(VK_SHIFT)) == 0) m_EC.m_CaretSel = m_EC.m_Caret;
            }

            m_EC.m_Caret_Virtual_X = m_EC.m_Caret.X;
            CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
            break;

        case VK_RIGHT:
            Log_IO::wcout() << "WM_KeyDown - VK_RIGHT" << std::endl;

            if (HIBYTE(GetKeyState(VK_SHIFT)) == 0 && m_EC.m_Caret != m_EC.m_CaretSel) {
                Caret_Struct Start, End;
                m_EC.Differentiate_Start_End(&Start, &End, m_EC.m_Caret, m_EC.m_CaretSel);
                m_EC.m_Caret = End;
                m_EC.m_CaretSel = m_EC.m_Caret;
            }

            else {
                if (m_EC.m_Caret.Y + 1 != m_EC.m_Text.size() || m_EC.m_Caret.X != m_EC.m_Text.back().size()) {
                    if (m_EC.m_Caret.X != m_EC.m_Text.at(m_EC.m_Caret.Y).size()) {
                        m_EC.m_Caret.X++;
                    } else {
                        m_EC.m_Caret.Y++;
                        m_EC.m_Caret.X = 0;
                    }
                }

                if (HIBYTE(GetKeyState(VK_SHIFT)) == 0) m_EC.m_CaretSel = m_EC.m_Caret;
            }

            m_EC.m_Caret_Virtual_X = m_EC.m_Caret.X;
            CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
            break;

        case VK_UP:
            Log_IO::wcout() << "WM_KeyDown - VK_UP" << std::endl;
            if (m_EC.m_Caret.Y > 0) {
                m_EC.m_Caret.Y--;
                m_EC.m_Caret.X = m_EC.m_Caret_Virtual_X;

                if (m_EC.m_Text.at(m_EC.m_Caret.Y).size() < m_EC.m_Caret.X)
                    m_EC.m_Caret.X = m_EC.m_Text.at(m_EC.m_Caret.Y).size();

                if (HIBYTE(GetKeyState(VK_SHIFT)) == 0) m_EC.m_CaretSel = m_EC.m_Caret;
                CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
            }
            break;

        case VK_DOWN:
            Log_IO::wcout() << "WM_KeyDown - VK_DOWN" << std::endl;
            if (m_EC.m_Caret.Y < m_EC.m_Text.size() - 1) {
                m_EC.m_Caret.Y++;
                m_EC.m_Caret.X = m_EC.m_Caret_Virtual_X;

                if (m_EC.m_Text.at(m_EC.m_Caret.Y).size() < m_EC.m_Caret.X)
                    m_EC.m_Caret.X = m_EC.m_Text.at(m_EC.m_Caret.Y).size();

                if (HIBYTE(GetKeyState(VK_SHIFT)) == 0) m_EC.m_CaretSel = m_EC.m_Caret;
                CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
            }
            break;

        case VK_DELETE:
            Log_IO::wcout() << "WM_KeyDown - VK_DELETE" << std::endl;
            if (!Delete_Selected_Unordered(m_EC.m_CaretSel, m_EC.m_Caret)) {
                if (!((m_EC.m_Text.size() - 1 <= m_EC.m_Caret.Y) && (m_EC.m_Text.at(m_EC.m_Caret.Y).size() <= m_EC.m_Caret.X))) {

                    if (m_EC.m_Text.at(m_EC.m_Caret.Y).size() > m_EC.m_Caret.X) {
                        auto& Str = m_EC.m_Text.at(m_EC.m_Caret.Y);
                        Register_History(HISTORY_MSG_DELETE, m_EC.m_Caret, m_EC.m_CaretSel, { std::wstring() = Str.at(m_EC.m_Caret.X) });
                        auto& Clr = m_EC.m_TextColor.at(m_EC.m_Caret.Y);
                        Str.erase(Str.begin() + m_EC.m_Caret.X);
                        Clr.erase(Clr.begin() + m_EC.m_Caret.X);

                        if (m_EC.m_MaxLine_Length.Line_Pos == m_EC.m_Caret.Y) {
                            Calculate_Max_Line_Length();
                        }
                    } else {
                        Register_History(HISTORY_MSG_DELETE, m_EC.m_Caret, m_EC.m_CaretSel, { std::wstring() = VK_RETURN });
                        auto& Str = m_EC.m_Text.at(m_EC.m_Caret.Y + 1);
                        auto& Clr = m_EC.m_TextColor.at(m_EC.m_Caret.Y + 1);
                        m_EC.m_Text.at(m_EC.m_Caret.Y) += m_EC.m_Text.at(m_EC.m_Caret.Y + 1);
                        m_EC.m_TextColor.at(m_EC.m_Caret.Y) += m_EC.m_TextColor.at(m_EC.m_Caret.Y + 1);
                        m_EC.m_Text.erase(m_EC.m_Text.begin() + (m_EC.m_Caret.Y + 1));
                        m_EC.m_TextColor.erase(m_EC.m_TextColor.begin() + (m_EC.m_Caret.Y + 1));

                        m_EC.Set_Lines(m_EC.m_Text.size());
                        Calculate_Max_Line_Length();
                    }

                    m_EC.m_Caret_Virtual_X = m_EC.m_Caret.X;
                    CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
                }
            }
            break;

        case VK_BACK:
            Log_IO::wcout() << "WM_KeyDown - VK_BACK" << std::endl;
            if (!Delete_Selected_Unordered(m_EC.m_CaretSel, m_EC.m_Caret) && (m_EC.m_Caret.X + m_EC.m_Caret.Y) != 0) {


                if (m_EC.m_Caret.X != 0) {
                    auto& Str = m_EC.m_Text.at(m_EC.m_Caret.Y);
                    auto& Clr = m_EC.m_TextColor.at(m_EC.m_Caret.Y);

                    auto History_Str = Str.at(m_EC.m_Caret.X - 1);

                    Str.erase(Str.begin() + (m_EC.m_Caret.X - 1));
                    Clr.erase(Clr.begin() + (m_EC.m_Caret.X - 1));

                    m_EC.m_Caret.X--;
                    m_EC.m_CaretSel = m_EC.m_Caret;

                    if (m_EC.m_MaxLine_Length.Line_Pos == m_EC.m_Caret.Y) {
                        Calculate_Max_Line_Length();
                    }
                    Register_History(HISTORY_MSG_BACKSPACE, m_EC.m_Caret, m_EC.m_CaretSel, { std::wstring() = History_Str });

                } else {
                    auto& Str = m_EC.m_Text.at(m_EC.m_Caret.Y);
                    auto& Clr = m_EC.m_TextColor.at(m_EC.m_Caret.Y);

                    m_EC.m_Caret.X = m_EC.m_Text.at(m_EC.m_Caret.Y - 1).size();
                    m_EC.m_Caret.Y--;
                    m_EC.m_CaretSel = m_EC.m_Caret;

                    m_EC.m_Text.at(m_EC.m_Caret.Y) += Str;
                    m_EC.m_TextColor.at(m_EC.m_Caret.Y) += Clr;

                    m_EC.m_Text.erase(m_EC.m_Text.begin() + (m_EC.m_Caret.Y + 1));
                    m_EC.m_TextColor.erase(m_EC.m_TextColor.begin() + (m_EC.m_Caret.Y + 1));

                    m_EC.Set_Lines(m_EC.m_Text.size());
                    Calculate_Max_Line_Length();
                    Register_History(HISTORY_MSG_BACKSPACE, m_EC.m_Caret, m_EC.m_CaretSel, { std::wstring() = VK_RETURN });
                }

                m_EC.m_Caret_Virtual_X = m_EC.m_Caret.X;
                CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
            }
            break;

        default:
            Log_IO::wcout() << "WM_KeyDown" << std::endl;
        }
    }
    m_EC.Corect_Scrolling();

}


void Keyboard::WM_Char(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_KEYBOARD);
    m_EC.m_Dispatcher.Return_Val = 0;
    m_EC.m_Dispatcher.Return = true;
    if (wParam < VK_SPACE && wParam != VK_RETURN) return;


    if (m_EC.m_Caret != m_EC.m_CaretSel) {
        std::vector<std::wstring> Selected_Text;
        Extract_Selected(m_EC.m_Text, m_EC.m_Caret, m_EC.m_CaretSel, &Selected_Text);
        Register_History(HISTORY_MSG_DEL_SEL_INSERT, m_EC.m_Caret, m_EC.m_CaretSel, { std::wstring() = (wchar_t)wParam }, Selected_Text);
    }
    else
        Register_History(HISTORY_MSG_INSERT, m_EC.m_Caret, m_EC.m_CaretSel, { std::wstring() = (wchar_t)wParam });
    Delete_Selected_Unordered(m_EC.m_CaretSel, m_EC.m_Caret, false);


    if (wParam != VK_RETURN) {
        Log_IO::wcout() << "WM_Char - wParam: " << (wchar_t)wParam << "(" << wParam << ")" << std::endl;

        Insert({ std::wstring() = (wchar_t)wParam }, m_EC.m_Caret);
        
        if (m_EC.m_MaxLine_Length.Line_Lenght < m_EC.m_Text.at(m_EC.m_Caret.Y).size()) {
            m_EC.m_MaxLine_Length.Line_Lenght = m_EC.m_Text.at(m_EC.m_Caret.Y).size();
            m_EC.m_MaxLine_Length.Line_Pos = m_EC.m_Caret.Y;
            m_EC.m_Horz_Scrollbar.Set_Range((int)m_EC.m_MaxLine_Length.Line_Lenght + (int)m_EC.m_Screen_MaxX - 20);
        }
        else if (m_EC.m_MaxLine_Length.Line_Pos == m_EC.m_Caret.Y) {
            m_EC.m_MaxLine_Length.Line_Lenght++;
            m_EC.m_Horz_Scrollbar.Set_Range((int)m_EC.m_MaxLine_Length.Line_Lenght + (int)m_EC.m_Screen_MaxX - 20);
        }

    }
    else {
        Log_IO::wcout() << "WM_Char - VK_RETURN " << std::endl;

        Insert({ L"", L"" }, m_EC.m_Caret);

        if (m_EC.m_MaxLine_Length.Line_Pos + 1 == m_EC.m_Caret.Y) {
            if (m_EC.m_Text.at(m_EC.m_Caret.Y).size() == m_EC.m_MaxLine_Length.Line_Lenght) {
                m_EC.m_MaxLine_Length.Line_Pos++;
            } else {
                Calculate_Max_Line_Length();
            }
        } else if (m_EC.m_MaxLine_Length.Line_Pos >= m_EC.m_Caret.Y) {
            m_EC.m_MaxLine_Length.Line_Pos++;
        }

        m_EC.Set_Lines(m_EC.m_Text.size());
    }

    m_EC.Corect_Scrolling();
    CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
}


void Keyboard::Copy() {
    if (m_EC.m_Caret == m_EC.m_CaretSel) return;
    Caret_Struct Start, End;
    m_EC.Differentiate_Start_End(&Start, &End, m_EC.m_Caret, m_EC.m_CaretSel);

    std::wstring Selected_Text;
    size_t X_Start = Start.X;
    for (size_t Y_Pos = Start.Y; Y_Pos <= End.Y; Y_Pos++) {
        if (Y_Pos == End.Y) {
            Selected_Text += m_EC.m_Text.at(Y_Pos).substr(X_Start, End.X);
        } else {
            Selected_Text += m_EC.m_Text.at(Y_Pos).substr(X_Start) + L"\r\n";
        }
        X_Start = 0;
    }

    HGLOBAL hMem = Convert_Str_To_Global_Object(Selected_Text);
    CHECK_ERR_RET(OpenClipboard(0), ERR_MSG_OPEN_CLIPBOARD, CHECK_ERR_VAL_EMPTY);
    CHECK_ERR_RET(EmptyClipboard(), ERR_MSG_EMTY_CLIPBOARD, CHECK_ERR_VAL_EMPTY);
    CHECK_ERR_RET(SetClipboardData(CF_UNICODETEXT, hMem), ERR_MSG_SET_CLIPBOARD_DATA, CHECK_ERR_VAL_EMPTY);
    CHECK_ERR_RET(CloseClipboard(), ERR_MSG_CLOSE_CLIPBOARD, CHECK_ERR_VAL_EMPTY);
}


void Keyboard::Cut() {
    if (m_EC.m_Caret == m_EC.m_CaretSel) return;
    Caret_Struct Start, End;
    m_EC.Differentiate_Start_End(&Start, &End, m_EC.m_Caret, m_EC.m_CaretSel);

    std::wstring Selected_Text;
    size_t X_Start = Start.X;
    for (size_t Y_Pos = Start.Y; Y_Pos <= End.Y; Y_Pos++) {
        if (Y_Pos == End.Y) {
            Selected_Text += m_EC.m_Text.at(Y_Pos).substr(X_Start, End.X);
        } else {
            Selected_Text += m_EC.m_Text.at(Y_Pos).substr(X_Start) + L"\r\n";
        }
        X_Start = 0;
    }

    Delete_Selected(Start, End);
    CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);

    HGLOBAL hMem = Convert_Str_To_Global_Object(Selected_Text);
    CHECK_ERR_RET(OpenClipboard(0), ERR_MSG_OPEN_CLIPBOARD, CHECK_ERR_VAL_EMPTY);
    CHECK_ERR_RET(EmptyClipboard(), ERR_MSG_EMTY_CLIPBOARD, CHECK_ERR_VAL_EMPTY);
    CHECK_ERR_RET(SetClipboardData(CF_UNICODETEXT, hMem), ERR_MSG_SET_CLIPBOARD_DATA, CHECK_ERR_VAL_EMPTY);
    CHECK_ERR_RET(CloseClipboard(), ERR_MSG_CLOSE_CLIPBOARD, CHECK_ERR_VAL_EMPTY);
}


void Keyboard::Paste() {
    CHECK_ERR_RET(OpenClipboard(0), ERR_MSG_OPEN_CLIPBOARD, CHECK_ERR_VAL_EMPTY);
    HANDLE Clipboard_Data = GetClipboardData(CF_UNICODETEXT);
    CHECK_ERR_RET(Clipboard_Data, ERR_MSG_GET_CLIPBOARD_DATA, CHECK_ERR_VAL_EMPTY);
    std::wstring Raw_Clipboard_Text;
    Convert_Global_Object_To_Str(Clipboard_Data, &Raw_Clipboard_Text);
    CHECK_ERR_RET(CloseClipboard(), ERR_MSG_CLOSE_CLIPBOARD, CHECK_ERR_VAL_EMPTY);
    
    std::vector<std::wstring> Clipboard_Text = Format_To_Internal(Raw_Clipboard_Text);

    if (m_EC.m_Caret == m_EC.m_CaretSel)
        Register_History(HISTORY_MSG_INSERT, m_EC.m_Caret, m_EC.m_CaretSel, Clipboard_Text);
    else {
        std::vector<std::wstring> Extracted;
        Extract_Selected(m_EC.m_Text, m_EC.m_Caret, m_EC.m_CaretSel, &Extracted);
        Register_History(HISTORY_MSG_DEL_SEL_INSERT, m_EC.m_Caret, m_EC.m_CaretSel, Clipboard_Text, Extracted);
    }
    
    Delete_Selected_Unordered(m_EC.m_CaretSel, m_EC.m_Caret, false);
    Insert(Clipboard_Text, m_EC.m_Caret);

    CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
}
