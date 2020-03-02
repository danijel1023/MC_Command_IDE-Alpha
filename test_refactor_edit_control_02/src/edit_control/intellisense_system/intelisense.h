#pragma once
#include "pch.h"
#include "../../json_reader/src/json.h"

#define TIMER_ID_TWO_SEC_TIMER 1

class Edit_Control;
class IntelliSense {
public:
    void Init(HWND Parent);
    void Proc_Msg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void Proc_Msg_After(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    IntelliSense() = delete;
    IntelliSense(Edit_Control& EC);
    ~IntelliSense();

    void Two_Sec_Timer();

    void WM_KeyDown(WPARAM wParam, LPARAM lParam);
    void WM_Char(WPARAM wParam, LPARAM lParam);

private:
    Edit_Control& m_EC;
    HWND m_Parent = NULL;
    Json m_Syntax_Obj = Json();

    void Analise_Line(std::wstring& Line, std::vector<COLORREF>& Colors);

    size_t m_InitCalls = 0;
    bool m_Is_Initialized = false;

    friend class Edit_Control;
};