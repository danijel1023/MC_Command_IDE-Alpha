#include "pch.h"
#include "intelisense.h"
#include "pch.h"

bool IntelliSense::Minecraft_Operation(std::wstring& Word) {
    return Generic_ArrElm_Search(m_Minecraft_Operation_Obj, Word);
}
