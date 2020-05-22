#include "pch.h"
#include "intelisense.h"
#include "pch.h"

bool IntelliSense::Minecraft_Color(std::wstring& Word) {
    m_Minecraft_Color_Obj.Root();

    size_t Size = m_Minecraft_Color_Obj.Size();
    for (size_t i = 0; i < Size; i++) {
        if (m_Minecraft_Color_Obj.Arr(i).Str() == Word) return true;
        m_Minecraft_Color_Obj.Back();
    }
    
    return false;
}
