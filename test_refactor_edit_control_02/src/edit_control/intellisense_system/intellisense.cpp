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
    Log_IO::wcout() << L"Init" << std::endl;

    m_InitCalls++;
    if (m_Is_Initialized) {
        MB_ERR("Initialization Function was Called More than 1 Time!");
        return;
    }
    m_Parent = Parent;

    Json IS_Linker(L"intellisense_structures/linker.json");
    auto& Commands = IS_Linker.Root().Obj(L"Commands").Str();
    auto& Minecraft_Color = IS_Linker.Root().Obj(L"Minecraft_Color").Str();


    Log_IO::wcout() << L"Starting Compiling IntelliSence structures" << std::endl;
    m_Syntax_Obj.Init(Commands);
    m_Minecraft_Color_Obj.Init(Minecraft_Color);


    Log_IO::wcout() << L"Completed Compiling IntelliSence structures" << std::endl;

}


void IntelliSense::Proc_Msg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System Obj(SYSTEM_ID_INTELLISENSE);
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

void IntelliSense::Proc_Msg_After(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System Obj(SYSTEM_ID_INTELLISENSE);
    m_EC.m_Dispatcher.Return = false;

    switch (uMsg) {
    case WM_KEYDOWN:
        if (HIBYTE(GetKeyState(VK_CONTROL)) == 0) {
            switch (wParam) {
            case VK_LEFT:
            case VK_DELETE:
            case VK_BACK:
                Analise_Line(m_EC.m_Text.at(m_EC.m_Caret.Y), m_EC.m_TextColor.at(m_EC.m_Caret.Y));
                if (Error_Handler.Has_Error()) {
                    Log_IO::Set_Color::Error();
                    Log_IO::wcout() << Error_Handler.Get_Error() << std::endl;
                    Log_IO::Set_Color::Default();
                }

                CHECK_ERR(InvalidateRect(m_Parent, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
                break;
            }
        }
        if (m_EC.m_Dispatcher.Return) return;
        break;

    case WM_CHAR:
        Analise_Line(m_EC.m_Text.at(m_EC.m_Caret.Y), m_EC.m_TextColor.at(m_EC.m_Caret.Y));
        if (Error_Handler.Has_Error()) {
            Log_IO::Set_Color::Error();
            Log_IO::wcout() << Error_Handler.Get_Error() << std::endl;
            Log_IO::Set_Color::Default();
        }
        CHECK_ERR(InvalidateRect(m_Parent, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
        break;
    }
}


void IntelliSense::WM_KeyDown(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System Obj(SYSTEM_ID_INTELLISENSE);

    m_EC.m_Dispatcher.Return = false;
}


void IntelliSense::WM_Char(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System Obj(SYSTEM_ID_INTELLISENSE);
    Log_IO::wcout() << "WM_Char: " << (wchar_t)wParam << " - " << lParam << std::endl;

    m_EC.m_Dispatcher.Return = false;
}


void IntelliSense::Two_Sec_Timer() {
    Log_IO::Start_Log_System Obj(SYSTEM_ID_INTELLISENSE);
    Log_IO::wcout() << "Two_Sec_Timer" << std::endl;

    m_EC.m_Dispatcher.Return = false;
}


COLORREF IntelliSense::Get_Color(std::wstring Type) {
    if (Type == L"INCOMPLETE") return RGB(255, 0, 255);
    return RGB(0, 255, 40);
}


void IntelliSense::Start_New_Line() {
    Error_Handler.Get_Error();
    m_Syntax_Obj.Root();

    m_Paraser_Func_Name = L"";
    m_Paraser_Set_Lock = false;
    m_Paraser_Func_Counter = 0;
}
