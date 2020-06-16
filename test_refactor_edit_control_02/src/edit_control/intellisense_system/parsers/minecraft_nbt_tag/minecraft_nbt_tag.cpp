#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Nbt_Tag(std::wstring& Word) {
    std::wstring Error;
    if (!SNbt_Formater::Is_Valid(Word, &Error)) {
        Error_Handler << Error.c_str();
        return false;
    }

    return true;
}
