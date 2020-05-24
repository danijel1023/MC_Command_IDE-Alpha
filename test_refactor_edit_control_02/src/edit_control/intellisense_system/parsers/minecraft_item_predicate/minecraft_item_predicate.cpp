#include "pch.h"
#include "intelisense.h"
#include "pch.h"

bool IntelliSense::Minecraft_Item_Predicate(std::wstring& Word) {
    return Generic_ArrElm_Search(m_Minecraft_Item_Predicate_Obj, Word);
}
