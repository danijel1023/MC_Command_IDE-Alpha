#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Block_Predicate(std::wstring& Word) {
    return Generic_ArrElm_Search(m_Minecraft_Block_Predicate_Obj, Word);
}
