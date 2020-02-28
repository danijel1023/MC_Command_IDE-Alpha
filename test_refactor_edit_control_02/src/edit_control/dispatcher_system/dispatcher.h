#pragma once
#include "pch.h"

class Edit_Control;
class Dispatcher {
public:
    Dispatcher() = delete;
    Dispatcher(Edit_Control& EC);
    ~Dispatcher();

    Edit_Control& m_EC;

    //huge function distributing messages to specific system (a bit slow... but it'll do)
    LRESULT SSC_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    bool m_Locked = false;
    bool m_Repeat_UnlockedMsg = false;
    int m_Main_System = 0;
    int m_Sub_System = 0;

    void Set_Sub_Systems(int Sub_System);
    void Enable_Sub_Systems(int Sub_System);
    void Disable_Sub_Systems(int Sub_System);
    bool Sub_Systems_AreActive(int Sub_System);

    bool Return = false;
    LRESULT Return_Val = NULL;
    friend class Edit_Control;
};

