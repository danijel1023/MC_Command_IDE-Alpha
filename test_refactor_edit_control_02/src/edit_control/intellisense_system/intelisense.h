#pragma once
#include "pch.h"
#include "../../json_reader/src/json.h"

#define TIMER_ID_TWO_SEC_TIMER 1

#include "is_parser_structs.h"
#include "error_handler.h"

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
    Error_Handler Error_Handler = ::Error_Handler();

    void Analise_Line(std::wstring& Line, std::vector<COLORREF>& Colors);
    COLORREF Get_Color(std::wstring Type, unsigned int Additional_Data = 0);


    bool Entity_Paraser(std::wstring& Word);

    size_t m_InitCalls = 0;
    bool m_Is_Initialized = false;

    friend class Edit_Control;
};
