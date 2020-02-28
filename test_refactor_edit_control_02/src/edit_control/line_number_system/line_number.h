#pragma once
#include "pch.h"

class Edit_Control;
class Line_Number {
public:
    Line_Number() = delete;
    Line_Number(Edit_Control& EC);
    ~Line_Number();

    void Set_Size(RECT& Rect);
    void Init(HWND hWnd);
    void Paint(HFONT Font);
    LRESULT Edit_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


private:
    HWND m_Parent = NULL;
    HWND m_hWnd = NULL;
    Edit_Control& m_EC;

    size_t m_InitCalls = 0;
    bool m_Is_Initialized = false;
    static UINT m_uIdSubclass;

    void WM_Paint();

    friend class Edit_Control;
};
