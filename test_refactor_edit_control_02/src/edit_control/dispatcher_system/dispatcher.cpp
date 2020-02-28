#include "pch.h"
#include "edit_control/_edit_control/edit_control.h"
//#include "dispatcher.h" --> It's already included in edit_control.h

#define RETURN_CHECK(x)\
x;\
if (Return) { Return = false; return Return_Val; }0


Dispatcher::Dispatcher(Edit_Control & EC)
    :m_EC(EC)
{}

Dispatcher::~Dispatcher() {}


static Edit_Control* EC_ptr_OLD = 0;
static HWND hWnd_OLD = 0;
static UINT uMsg_OLD = 0;
static WPARAM wParam_OLD = 0;
static LPARAM lParam_OLD = 0;
LRESULT Dispatcher::SSC_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (m_Locked) {
        switch (m_Main_System) {
        case SYSTEM_ID_VERT_SCROLLBAR:  m_EC.m_Vert_Scrollbar.Proc_Msg(hWnd, uMsg, wParam, lParam);    break;
        case SYSTEM_ID_HORZ_SCROLLBAR:  m_EC.m_Horz_Scrollbar.Proc_Msg(hWnd, uMsg, wParam, lParam);    break;
        case SYSTEM_ID_INTELLISENSE:    m_EC.m_IntelliSense.Proc_Msg(hWnd, uMsg, wParam, lParam);      break;
        case SYSTEM_ID_KEYBOARD:        m_EC.m_Keyboard.Proc_Msg(hWnd, uMsg, wParam, lParam);          break;
        case SYSTEM_ID_RENDER:          m_EC.m_Render.Proc_Msg(hWnd, uMsg, wParam, lParam);            break;
        case SYSTEM_ID_MOUSE:           m_EC.m_Mouse.Proc_Msg(hWnd, uMsg, wParam, lParam);             break;
        default:
            MB_ERR("Invalid Parameter for 'm_Main_System' - 'm_Lock' is Set!");
            debugbreak();
            PostQuitMessage(-1);
        }

        if ((m_Sub_System & SYSTEM_ID_VERT_SCROLLBAR) != 0)   m_EC.m_Vert_Scrollbar.Proc_Msg(hWnd, uMsg, wParam, lParam);
        if ((m_Sub_System & SYSTEM_ID_HORZ_SCROLLBAR) != 0)   m_EC.m_Horz_Scrollbar.Proc_Msg(hWnd, uMsg, wParam, lParam);
        if ((m_Sub_System & SYSTEM_ID_INTELLISENSE) != 0)     m_EC.m_IntelliSense.Proc_Msg(hWnd, uMsg, wParam, lParam);
        if ((m_Sub_System & SYSTEM_ID_KEYBOARD) != 0)         m_EC.m_Keyboard.Proc_Msg(hWnd, uMsg, wParam, lParam);
        if ((m_Sub_System & SYSTEM_ID_RENDER) != 0)           m_EC.m_Render.Proc_Msg(hWnd, uMsg, wParam, lParam);
        if ((m_Sub_System & SYSTEM_ID_MOUSE) != 0)            m_EC.m_Mouse.Proc_Msg(hWnd, uMsg, wParam, lParam);


        if (m_Repeat_UnlockedMsg) {
            m_Locked = false;
            m_Repeat_UnlockedMsg = false;
            SSC_Proc(hWnd_OLD, uMsg_OLD, wParam_OLD, lParam_OLD);
        }

        //For 'm_Repeat_UnlockedMsg' Params
        hWnd_OLD = hWnd; uMsg_OLD = uMsg;
        wParam_OLD = wParam; lParam_OLD = lParam;

        if (Return) { Return = false; return Return_Val; }
    } else {
        RETURN_CHECK(m_EC.m_Vert_Scrollbar.Proc_Msg(hWnd, uMsg, wParam, lParam));
        RETURN_CHECK(m_EC.m_Horz_Scrollbar.Proc_Msg(hWnd, uMsg, wParam, lParam));
        RETURN_CHECK(m_EC.m_IntelliSense.Proc_Msg(hWnd, uMsg, wParam, lParam));
        RETURN_CHECK(m_EC.m_Keyboard.Proc_Msg(hWnd, uMsg, wParam, lParam));
        RETURN_CHECK(m_EC.m_Render.Proc_Msg(hWnd, uMsg, wParam, lParam));
        RETURN_CHECK(m_EC.m_Mouse.Proc_Msg(hWnd, uMsg, wParam, lParam));
    }

    return CallWindowProc(Control_Register::OLD_STATIC_WND_PROC, hWnd, uMsg, wParam, lParam);
}

void Dispatcher::Set_Sub_Systems(int Sub_Systems) {
    m_Sub_System = Sub_Systems;
}

void Dispatcher::Enable_Sub_Systems(int Sub_Systems) {
    m_Sub_System = m_Sub_System | Sub_Systems;   //OR
}

void Dispatcher::Disable_Sub_Systems(int Sub_Systems) {
    m_Sub_System = m_Sub_System ^ Sub_Systems;   //XOR
}

bool Dispatcher::Sub_Systems_AreActive(int Sub_Systems) {
    return (m_Sub_System & Sub_Systems);
}


/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Wrapper ///////////////////////////////////////

static HWND Last_hWnd = NULL;
static Edit_Control* Last_ECP = NULL;    // Last Edit Control Pointer
LRESULT CALLBACK Global_SSC_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (hWnd == NULL) {
        MB_ERR("Something Went Wrong - hWnd is NULL!");
        PostQuitMessage(-1);
    }

    else if (Last_hWnd != hWnd) {
        Last_ECP = Control_Register::Get_ControlPtr(hWnd);
        if (Last_ECP == NULL) {
            //The instance wasn't yet initialized - call the default proc (NORMAL BEHAVIOR)
            return CallWindowProc(Control_Register::OLD_STATIC_WND_PROC, hWnd, uMsg, wParam, lParam);
        } else Last_hWnd = hWnd;
    }

    return Last_ECP->m_Dispatcher.SSC_Proc(hWnd, uMsg, wParam, lParam);
}
