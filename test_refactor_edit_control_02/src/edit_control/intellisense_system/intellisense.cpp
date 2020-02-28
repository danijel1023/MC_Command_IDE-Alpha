#include "pch.h"
#include "edit_control/_edit_control/edit_control.h"

IntelliSense::IntelliSense(Edit_Control& EC)
    :m_EC(EC)
{}

IntelliSense::~IntelliSense() {
    KillTimer(m_Parent, TIMER_ID_TWO_SEC_TIMER);
}


void IntelliSense::Init(HWND Parent) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_INTELLISENSE);
    Log_IO::wcout() << "Init" << std::endl;

    m_InitCalls++;
    if (m_Is_Initialized) {
        MB_ERR("Initialization Function was Called More than 1 Time!");
        return;
    }
    m_Parent = Parent;

    Json::Json File_Loc(L"intellisense_structures\file.json");
    Json::Json Temp(File_Loc.Obj(L"File Location").Str());
    m_Syntax_Obj = a;
}


void IntelliSense::Proc_Msg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    m_EC.m_Dispatcher.Return = false;

    switch (uMsg) {
    case WM_CHAR:
        SetTimer(m_Parent, TIMER_ID_TWO_SEC_TIMER, 2 * (1000), NULL);
        WM_Char(wParam, lParam);
        if (m_EC.m_Dispatcher.Return) return;
        break;

    case WM_TIMER:
        KillTimer(m_Parent, TIMER_ID_TWO_SEC_TIMER);

        switch (wParam) {
        case TIMER_ID_TWO_SEC_TIMER:
            Two_Sec_Timer();
            if (m_EC.m_Dispatcher.Return) return;
            break;
        }

    }

}


void IntelliSense::WM_Char(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System Obj(SYSTEM_ID_INTELLISENSE);
    Log_IO::wcout() << "WM_Char: " << wParam << " - " << lParam << std::endl;



    m_EC.m_Dispatcher.Return = false;
}


void IntelliSense::Two_Sec_Timer() {
    Log_IO::Start_Log_System Obj(SYSTEM_ID_INTELLISENSE);
    Log_IO::wcout() << "Two_Sec_Timer" << std::endl;



    m_EC.m_Dispatcher.Return = false;
}
