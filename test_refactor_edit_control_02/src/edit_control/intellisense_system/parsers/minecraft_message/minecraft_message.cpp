#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Message(std::wstring& Word) {
    //return Brigadier_String(Word, L"word");   //optimisation
    m_Paraser_Set_Lock = true;
    return true;
}
