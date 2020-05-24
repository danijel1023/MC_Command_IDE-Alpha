#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Item_Stack(std::wstring& Word) {
    return Generic_ArrElm_Search(m_Minecraft_Item_Stack_Obj, Word);
}
