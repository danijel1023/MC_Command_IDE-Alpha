#pragma once
#include "pch.h"

class Edit_Control;
class Scroll_Bar {
public:
    void Init(HWND Parent, int* Pos_Referance, DWORD Bar, UINT Page, int Range);
    void Proc_Msg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void Set_Scroll_Info();
    void Set_Range(int Pos);
    void Set_Page(UINT Pos);
    void Set_Pos(int Pos);
    int Get_Pos();

    void Set_Size(RECT& Rect);

    Scroll_Bar() = delete;
    Scroll_Bar(Edit_Control& EC);
    ~Scroll_Bar();

private:
    Edit_Control& m_EC;
    SCROLLINFO m_SI = {};
    HWND m_Parent = 0;
    HWND m_hWnd = 0;
    UINT m_Message_Notification_Code = 0;

    DWORD m_Bar = 0;
    UINT m_Page = 0;
    int m_Range = 0;
    int* m_Pos = 0;

    bool m_Initialised = false;
    size_t m_Init_Calls = 0;
};

