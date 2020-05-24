#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Color(std::wstring& Word) {
    return Generic_ArrElm_Search(m_Minecraft_Color_Obj, Word);
}
