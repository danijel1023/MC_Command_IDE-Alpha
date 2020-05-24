#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Entity_Anchor(std::wstring& Word) {
    return Generic_ArrElm_Search(m_Minecraft_Entity_Anchor_Obj, Word);
}
