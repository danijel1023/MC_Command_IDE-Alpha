#pragma once
#include "pch.h"

class Edit_Control;
class Mouse {
public:
    void Init(HWND hWnd);
    void Proc_Msg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    Mouse() = delete;
    Mouse(Edit_Control& EC);
    ~Mouse();

private:
    Edit_Control& m_EC;
    HWND m_hWnd = NULL;

    void WM_LBtn_Up(WPARAM wParam, LPARAM lParam);
    void WM_LBtn_Down(WPARAM wParam, LPARAM lParam);
    void WM_LBtn_DblClk(WPARAM wParam, LPARAM lParam);
    void WM_Mouse_Move(WPARAM wParam, LPARAM lParam);
    void WM_Mouse_Wheel(WPARAM wParam, LPARAM lParam);

    friend class Edit_Control;
};

