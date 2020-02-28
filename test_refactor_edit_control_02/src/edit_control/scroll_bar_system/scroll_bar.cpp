#include "pch.h"
#include "edit_control/_edit_control/edit_control.h"
//#include "scroll_bar.h" --> It's already included in edit_control.h

Scroll_Bar::Scroll_Bar(Edit_Control& EC)
    :m_EC(EC)
{}


Scroll_Bar::~Scroll_Bar() {}


void Scroll_Bar::Set_Size(RECT& Rect) {
    CHECK_ERR(SetWindowPos(m_hWnd, NULL, Rect.left, Rect.top, Rect.right, Rect.bottom, SWP_NOZORDER), ERR_MSG_SET_WINDOW_POS);
    Set_Scroll_Info();
}


void Scroll_Bar::Init(HWND Parent, int* Pos_Referance, DWORD Bar, UINT Page, int Range) {
    m_Init_Calls++;
    if (m_Initialised) return;

    switch (Bar) {
    case SB_VERT: m_Message_Notification_Code = WM_VSCROLL; break;
    case SB_HORZ: m_Message_Notification_Code = WM_HSCROLL; break;
    default: CHECK_ERR(0, "The 'Bar' Paramater of Init of Scroll_Bar is invalid!"); break;
    }
    
    m_Pos = Pos_Referance;
    m_Parent = Parent;
    m_Range = Range;
    m_Page = Page;
    m_Bar = Bar;
    m_hWnd = CreateWindowEx(0, L"SCROLLBAR", NULL,
                            WS_CHILD | WS_VISIBLE | Bar,
                            0, 0, 100, 300,
                            m_Parent, NULL, GetModuleHandle(NULL), NULL);
    CHECK_ERR(m_hWnd, ERR_MSG_CREATE_WINDOW_EX);

    m_SI.cbSize = sizeof(SCROLLINFO);
    m_SI.fMask = SIF_ALL;
    m_SI.nMin = 0; m_SI.nMax = Range;
    m_SI.nPage = Page;
    m_SI.nPos = 0;
    SetScrollInfo(m_hWnd, SB_CTL, &m_SI, TRUE);

    m_Initialised = true;
}


void Scroll_Bar::Proc_Msg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    m_EC.m_Dispatcher.Return = false;

    if (m_Message_Notification_Code == uMsg) {
        m_EC.m_Dispatcher.Return_Val = 0;

        switch (LOWORD(wParam)) {
        case SB_LINEUP:
            if ((*m_Pos) > 0) {
                Set_Pos((*m_Pos) - 1);
            }
            m_EC.m_Dispatcher.Return = true;
            break;

        case SB_LINEDOWN:
            if ((int)(m_Range - m_Page) >= (*m_Pos)) {
                Set_Pos((*m_Pos) + 1);
            }
            m_EC.m_Dispatcher.Return = true;
            break;

        case SB_THUMBTRACK:
            m_SI.fMask = SIF_TRACKPOS;
            GetScrollInfo(m_hWnd, SB_CTL, &m_SI);
            Set_Pos(m_SI.nTrackPos);
            m_EC.m_Dispatcher.Return = true;
            break;
        }
    }

}

void Scroll_Bar::Set_Range(int Range) {
    m_Range = Range;
    Set_Scroll_Info();
}

void Scroll_Bar::Set_Page(UINT Page) {
    m_Page = Page;
    Set_Scroll_Info();
}

void Scroll_Bar::Set_Pos(int Pos) {
    if ((*m_Pos) == Pos) return;

    (*m_Pos) = Pos;
    Set_Scroll_Info();
    CHECK_ERR(InvalidateRect(m_Parent, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
}


void Scroll_Bar::Set_Scroll_Info() {
    m_SI.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    m_SI.nMin = 0; m_SI.nMax = m_Range;
    m_SI.nPage = m_Page;
    m_SI.nPos = (*m_Pos);

    SetScrollInfo(m_hWnd, SB_CTL, &m_SI, TRUE);
}


int Scroll_Bar::Get_Pos() {
    m_SI.fMask = SIF_POS;
    GetScrollInfo(m_hWnd, SB_CTL, &m_SI);
    return m_SI.nPos;
}
