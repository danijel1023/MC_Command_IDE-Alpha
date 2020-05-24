#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Block_State(std::wstring& Word) {
    return Generic_ArrElm_Search(m_Minecraft_Block_State_Obj, Word);
}
