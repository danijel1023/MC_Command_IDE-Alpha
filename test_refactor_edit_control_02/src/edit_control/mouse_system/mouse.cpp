#include "pch.h"
#include "edit_control/_edit_control/edit_control.h"
//#include "mouse.h" --> It's already included in edit_control.h

Mouse::Mouse(Edit_Control& EC)
    :m_EC(EC)
{}


Mouse::~Mouse() {
    if (m_EC.m_Focus) {
        if (m_EC.m_CaretVisible)
            CHECK_ERR(HideCaret(m_hWnd), ERR_MSG_HIDE_CARET);
        CHECK_ERR(DestroyCaret(), ERR_MSG_DESTROY_CARET);
    }
}


void Mouse::Init(HWND hWnd) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_MOUSE);
    Log_IO::wcout() << "Init" << std::endl;
    m_hWnd = hWnd;
}


void Mouse::Proc_Msg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    m_EC.m_Dispatcher.Return = false;

    switch (uMsg) {
    case WM_LBUTTONUP:
        WM_LBtn_Up(wParam, lParam);
        if (m_EC.m_Dispatcher.Return) return;
        break;
    case WM_LBUTTONDOWN:
        WM_LBtn_Down(wParam, lParam);
        if (m_EC.m_Dispatcher.Return) return;
        break;
    case WM_LBUTTONDBLCLK:
        WM_LBtn_DblClk(wParam, lParam);
        if (m_EC.m_Dispatcher.Return) return;
        break;
    case WM_MOUSEMOVE:
        WM_Mouse_Move(wParam, lParam);
        if (m_EC.m_Dispatcher.Return) return;
        break;
    case WM_MOUSEWHEEL:
        WM_Mouse_Wheel(wParam, lParam);
        if (m_EC.m_Dispatcher.Return) return;
        break;
    }
}


void Mouse::WM_LBtn_Up(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_MOUSE);
    //ReleaseCapture();
}


void Mouse::WM_LBtn_Down(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_MOUSE);
    Log_IO::wcout() << "WM_LBtn_Down" << std::endl;
    if (!m_EC.m_Focus) {
        CHECK_ERR(SetFocus(m_hWnd), ERR_MSG_SET_FOCUS);
        m_EC.m_Focus = true;
    }
    //SetCapture(m_EC.m_hWnd);

    Caret_Struct Caret_Old = m_EC.m_Caret;
    
    //Create double the amount of "test" fields on X axis -> divide each character into left and right part
    int Screen_MouseX = (GET_X_LPARAM(lParam) * 2 / m_EC.m_Char_Width);
    int Screen_MouseY = (GET_Y_LPARAM(lParam) / m_EC.m_Char_Height);

    if (Screen_MouseX % 2 == 1) Screen_MouseX++;
    Screen_MouseX /= 2;

    m_EC.m_Caret.Y = m_EC.m_Screen_YOff + Screen_MouseY;
    m_EC.m_Caret.X = m_EC.m_Screen_XOff + Screen_MouseX;
    if (m_EC.m_Caret.Y + 1 > m_EC.m_Text.size()) m_EC.m_Caret.Y = m_EC.m_Text.size() - 1;
    if (m_EC.m_Caret.X> m_EC.m_Text.at(m_EC.m_Caret.Y).size()) m_EC.m_Caret.X = m_EC.m_Text.at(m_EC.m_Caret.Y).size();

    if (m_EC.m_Caret != Caret_Old) CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
    if (HIBYTE(GetKeyState(VK_SHIFT)) == 0) m_EC.m_CaretSel = m_EC.m_Caret;
    m_EC.Corect_Scrolling();

    m_EC.m_Caret_Virtual_X = m_EC.m_Caret.X;
    m_EC.m_Dispatcher.Return_Val = 0;
    m_EC.m_Dispatcher.Return = true;
}


void Mouse::WM_Mouse_Move(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_MOUSE);
    //Console_IO::wcout << "WM_Mouse_Move" << std::endl;

    if (HIBYTE(GetKeyState(VK_LBUTTON)) != 0) {
        //Console_IO::wcout << "WM_Mouse_Move + VK_LBUTTON" << std::endl;
        Caret_Struct Caret_Old = m_EC.m_Caret;

        //Create double the amount of "test" fields on X axis -> divide each character into left and right part
        int Screen_MouseX = (GET_X_LPARAM(lParam) * 2 / m_EC.m_Char_Width);
        int Screen_MouseY = GET_Y_LPARAM(lParam) / m_EC.m_Char_Height;

        if (Screen_MouseX % 2 == 1) Screen_MouseX++;
        Screen_MouseX /= 2;

        m_EC.m_Caret.Y = m_EC.m_Screen_YOff + Screen_MouseY;
        m_EC.m_Caret.X = m_EC.m_Screen_XOff + Screen_MouseX;
        if (m_EC.m_Caret.Y + 1 > m_EC.m_Text.size()) m_EC.m_Caret.Y = m_EC.m_Text.size() - 1;
        if (m_EC.m_Caret.X > m_EC.m_Text.at(m_EC.m_Caret.Y).size()) m_EC.m_Caret.X = m_EC.m_Text.at(m_EC.m_Caret.Y).size();

        if (m_EC.m_Caret != Caret_Old) {
            Log_IO::wcout() << "WM_Mouse_Move + VK_LBUTTON + (m_EC.m_Caret != CaretOld)" << std::endl;
            m_EC.Corect_Scrolling();
            CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
        }
        m_EC.m_Caret_Virtual_X = m_EC.m_Caret.X;
    }

    m_EC.m_Dispatcher.Return_Val = 0;
    m_EC.m_Dispatcher.Return = true;
}


void Mouse::WM_Mouse_Wheel(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_MOUSE);
    Log_IO::wcout() << "WM_Mouse_Wheel" << std::endl;

    int Wheel_Direction = GET_WHEEL_DELTA_WPARAM(wParam);
    if (HIBYTE(GetKeyState(VK_CONTROL)) == 0) {
        Log_IO::wcout() << "WM_Mouse_Wheel + (HIBYTE(GetKeyState(VK_CONTROL)) == 0)" << std::endl;
        int Screen_YOffOld = m_EC.m_Screen_YOff;
        if (Wheel_Direction > 0) {
            if (m_EC.m_Screen_YOff > 0) {
                m_EC.m_Screen_YOff -= (m_EC.m_Screen_YOff > 3) ? 4 : m_EC.m_Screen_YOff;
            }
        } else {
            if ((size_t)(m_EC.m_Screen_YOff) + 1 <= m_EC.m_LineNumbers.size()) {
                m_EC.m_Screen_YOff += (m_EC.m_LineNumbers.size() - m_EC.m_Screen_YOff > 5) ? 4 : (int)(m_EC.m_LineNumbers.size() - m_EC.m_Screen_YOff - 1);
            }
        }

        if (m_EC.m_Screen_YOff != Screen_YOffOld) {
            CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
            m_EC.m_Vert_Scrollbar.Set_Scroll_Info();
        }

    } else {
        int Font_SizeOld = m_EC.m_Font_Size;
        if (Wheel_Direction > 0) {
            if (m_EC.m_Font_Size < 100) {
                m_EC.m_Font_Size += (int)(m_EC.m_Font_Size * 0.05) + 1;
            }
        } else {
            if (m_EC.m_Font_Size > 5) {
                m_EC.m_Font_Size -= (int)(m_EC.m_Font_Size * 0.05) + 1;
            }
        }

        if (Font_SizeOld != m_EC.m_Font_Size) {
            CHECK_ERR(DeleteObject(m_EC.m_Font), ERR_MSG_DELETE_OBJECT_HFONT);
            m_EC.m_Font = CreateFont(m_EC.m_Font_Size, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, L"Consolas");
            CHECK_ERR(m_EC.m_Font, ERR_MSG_CREATE_FONT);

            m_EC.Calculate_Text_Metrics();

            if (m_EC.m_Focus) {
                if (m_EC.m_CaretVisible)
                    CHECK_ERR(HideCaret(m_hWnd), ERR_MSG_HIDE_CARET);
                CHECK_ERR(DestroyCaret(), ERR_MSG_DESTROY_CARET);
                CHECK_ERR(CreateCaret(m_hWnd, NULL, m_EC.m_Caret_Width, m_EC.m_Caret_Height), ERR_MSG_CREATE_CARET);
                CHECK_ERR(ShowCaret(m_hWnd), ERR_MSG_SHOW_CARET);
                m_EC.m_CaretVisible = true;
            }

            m_EC.Size(m_EC.m_Rect);
        }
    }

    m_EC.m_Dispatcher.Return_Val = 0;
    m_EC.m_Dispatcher.Return = true;
}


void Mouse::WM_LBtn_DblClk(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_MOUSE);
    Log_IO::wcout() << "WM_LBtn_DblClk" << std::endl;
    CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);

    m_EC.m_Dispatcher.Return_Val = 0;
    m_EC.m_Dispatcher.Return = true;
}
