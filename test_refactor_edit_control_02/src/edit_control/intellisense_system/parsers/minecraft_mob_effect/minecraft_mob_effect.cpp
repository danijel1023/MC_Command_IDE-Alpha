#include "pch.h"
#include "intelisense.h"
#include "pch.h"

bool IntelliSense::Minecraft_Mob_Effect(std::wstring& Word) {
    return Generic_ArrElm_Search(m_Minecraft_Mob_Effect_Obj, Word);
}
