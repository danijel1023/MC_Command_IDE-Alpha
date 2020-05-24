#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Objective_Criteria(std::wstring& Word) {
    return Generic_ArrElm_Search(m_Minecraft_Objective_Criteria_Obj, Word);
}
