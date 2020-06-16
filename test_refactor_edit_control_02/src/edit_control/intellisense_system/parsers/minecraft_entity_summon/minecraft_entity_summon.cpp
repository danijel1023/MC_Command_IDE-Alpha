#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Entity_Summon(std::wstring& Word) {
    std::wstring Error;
    if (!Std::Insert_Namespace(Word, &Error)) {
        Error_Handler << Error.c_str();
        return false;
    }

    return Generic_ArrElm_Search(m_Minecraft_Entity_Summon_Obj, Word);
}
