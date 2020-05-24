#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Particle(std::wstring& Word) {
    return Generic_ArrElm_Search(m_Minecraft_Particle_Obj, Word);
}
