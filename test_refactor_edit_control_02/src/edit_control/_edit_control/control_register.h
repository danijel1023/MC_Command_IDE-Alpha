#pragma once
#include "pch.h"

class Edit_Control;
//SSC -> Subclassed(?) SuperClass
LRESULT CALLBACK Global_SSC_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//Used to indentify which control belongs to which instance - when application
//calls 'Global_SC_Proc', based on 'hWnd', using 'Get_ControlPtr' this class returns
//coresponding Instance pointer, so the right 'SSC_Proc' gets called
//0---------------------------
//Once you created window with 'CreateWindowEx', call 'Register_Instance' with given 'hWnd' and 'this': 'Register_Instance(hWnd, this)'
//0---------------------------
//- 'OLD_STATIC_WND_PROC': Send message to this fuction for default behavior:
//      CallWindowProc(Control_Register::OLD_STATIC_WND_PROC, hWnd, uMsg, wParam, lParam);
//- 'Class_Name': Use for second parameter of 'CreateWindowEx' function ('lpClassName')
class Control_Register {
public:
    static void Init();

    static Edit_Control* Get_ControlPtr(HWND hWnd);
    static void Register_Instance(HWND hWnd, Edit_Control* Instance);
    static WNDPROC OLD_STATIC_WND_PROC;
    const static WCHAR* Class_Name;

    //read only - don't change value, might break the code
    static size_t m_InitCalls;
private:
    static bool m_Is_Initialized;
    static std::vector<std::pair<HWND, Edit_Control*>> m_Links;
};
