#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Dimension(std::wstring& Word) {
    return Generic_ArrElm_Search(m_Minecraft_Dimension_Obj, Word);
}
