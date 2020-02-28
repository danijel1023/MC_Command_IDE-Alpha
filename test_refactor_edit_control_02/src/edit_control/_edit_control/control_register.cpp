#include "pch.h"
#include "control_register.h"

WNDPROC Control_Register::OLD_STATIC_WND_PROC = NULL;
const WCHAR* Control_Register::Class_Name = L"ENHC_STATIC";
size_t Control_Register::m_InitCalls = 0;
bool Control_Register::m_Is_Initialized = false;

std::vector<std::pair<HWND, Edit_Control*>> Control_Register::m_Links;

void Control_Register::Init() {
    m_InitCalls++;
    if (m_Is_Initialized) {
        return;
    }

    CHECK_ERR(Global_SSC_Proc, "Main SC_Proc function is NULL!");

    WNDCLASS wc = {};
    CHECK_ERR(GetClassInfo(NULL, L"STATIC", &wc), "Failed to get L\"STATIC\" info!");

    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = Class_Name;
    OLD_STATIC_WND_PROC = wc.lpfnWndProc;
    wc.lpfnWndProc = Global_SSC_Proc;

    CHECK_ERR(RegisterClass(&wc), "Window Registration Failed!");
    m_Is_Initialized = true;
}

Edit_Control* Control_Register::Get_ControlPtr(HWND hWnd) {
    for (size_t i = 0; i < m_Links.size(); i++) {
        if (m_Links.at(i).first == hWnd)
            return m_Links.at(i).second;
    }
    return NULL;
}

void Control_Register::Register_Instance(HWND hWnd, Edit_Control* Instance) {
    m_Links.push_back(std::make_pair(hWnd, Instance));
}
