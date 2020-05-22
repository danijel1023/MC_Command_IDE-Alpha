#include "pch.h"
#include "intelisense.h"
#include "pch.h"

bool IntelliSense::Minecraft_Block_Pos(std::wstring& Word) {
    if (m_Paraser_Func_Counter == 0) {
        m_Paraser_Func_Counter = 2;
        m_Paraser_Set_Lock = true;
    }
    else
        m_Paraser_Func_Counter--;

    if (m_Paraser_Func_Counter == 0) {
        m_Paraser_Set_Lock = false;
        return true;
    }

    return false;
}
