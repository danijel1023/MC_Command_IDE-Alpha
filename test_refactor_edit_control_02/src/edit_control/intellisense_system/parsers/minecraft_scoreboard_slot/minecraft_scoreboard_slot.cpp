#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Scoreboard_Slot(std::wstring& Word) {
    return Generic_ArrElm_Search(m_Minecraft_Scoreboard_Slot_Obj, Word);
}
