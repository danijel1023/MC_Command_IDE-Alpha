#include "pch.h"
#include "edit_control/_edit_control/edit_control.h"

UINT Line_Number::m_uIdSubclass = 0;

LRESULT CALLBACK Global_Edit_Proc(HWND hWnd, UINT uMsg, WPARAM wParam,
                                  LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

Line_Number::Line_Number(Edit_Control& EC)
    :m_EC(EC)
{}

Line_Number::~Line_Number() {}


void Line_Number::Set_Size(RECT& Rect) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_LINE_NUMBER);
    Log_IO::wcout() << "Set_Size" << std::endl;
    CHECK_ERR(SetWindowPos(m_hWnd, NULL, Rect.left, Rect.top, Rect.right, Rect.bottom, SWP_NOZORDER), ERR_MSG_SET_WINDOW_POS);
}


void Line_Number::Init(HWND hWnd) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_LINE_NUMBER);
    Log_IO::wcout() << "Init" << std::endl;
    m_InitCalls++;
    if (m_Is_Initialized) {
        MB_ERR("Initialization Function was Called More than 1 Time!");
        return;
    }
    m_Parent = hWnd;

    m_hWnd = CreateWindowEx(0, L"STATIC", NULL,
                            WS_CHILD | WS_VISIBLE | WS_TABSTOP | SS_LEFT | SS_NOTIFY,
                            0, 0, 0, 0,
                            m_Parent, NULL, GetModuleHandle(NULL), NULL);

    SetWindowSubclass(m_hWnd, Global_Edit_Proc, m_uIdSubclass, reinterpret_cast<DWORD_PTR>(this));
    m_uIdSubclass++;
}


void Line_Number::Paint(HFONT Font) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_LINE_NUMBER);
    Log_IO::wcout() << "Paint" << std::endl;
    InvalidateRect(m_hWnd, NULL, TRUE);
}


LRESULT CALLBACK Global_Edit_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    return reinterpret_cast<Line_Number*>(dwRefData)->Edit_Proc(hWnd, uMsg, wParam, lParam);
}


LRESULT Line_Number::Edit_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_LINE_NUMBER);
    switch (uMsg) {
    case WM_ERASEBKGND:
        Log_IO::wcout() << "WM_Erase_Bkg" << std::endl;
        RECT Rect;
        GetClientRect(m_hWnd, &Rect);
        CHECK_ERR(FillRect((HDC)wParam, &Rect, m_EC.m_Bkg_Brush), ERR_MSG_FILL_RECT);
        return 1;

    case WM_PAINT:
        Log_IO::wcout() << "WM_Paint" << std::endl;
        WM_Paint();
        return 0;
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

void Line_Number::WM_Paint() {
    Log_IO::Start_Log_System obj(SYSTEM_ID_LINE_NUMBER);
    Log_IO::wcout() << "WM_Paint" << std::endl;
    PAINTSTRUCT PS;
    HDC hDC = BeginPaint(m_hWnd, &PS);
    CHECK_ERR(hDC, ERR_MSG_BEGIN_PAINT);
    
    HFONT FontOld = (HFONT)SelectObject(hDC, m_EC.m_Font);
    CHECK_ERR(FontOld, ERR_MSG_SELECT_OBJECT_HFONT);
    auto Bkg_ModeOld = SetBkMode(hDC, TRANSPARENT);
    CHECK_ERR(Bkg_ModeOld, ERR_MSG_SET_BK_MODE);

    if (SetTextColor(hDC, m_EC.m_Line_Number_Clr) == CLR_INVALID) {
        MB_ERR("'SetTextColor' Failed!");
        PostQuitMessage(-1);
    }

    int Screen_Y = 0;
    for (size_t Pos = m_EC.m_Screen_YOff; Pos < m_EC.m_LineNumbers.size(); Pos++) {
        if (m_EC.m_Screen_MaxY < Screen_Y) break;

        int Y = Screen_Y * m_EC.m_Char_Height;
        CHECK_ERR(TextOut(hDC, 0, Y, m_EC.m_LineNumbers.at(Pos).c_str(), m_EC.m_LineNum_Length), ERR_MSG_TEXT_OUT);
        Screen_Y++;
    }

    CHECK_ERR(SetBkMode(hDC, Bkg_ModeOld), ERR_MSG_SET_BK_MODE);
    CHECK_ERR(SelectObject(hDC, FontOld), ERR_MSG_SELECT_OBJECT_HFONT);

    EndPaint(m_hWnd, &PS);
}
