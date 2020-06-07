#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Objective(std::wstring& Word) {
    return Brigadier_String(Word, L"word");
}
