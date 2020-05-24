#include "pch.h"
#include "intelisense.h"
#include "pch.h"

bool IntelliSense::Minecraft_Column_Pos(std::wstring& Word) {
    if (m_Paraser_Func_Counter == 0) {
        m_Paraser_Func_Counter = 1;
        m_Paraser_Set_Lock = true;
    } else
        m_Paraser_Func_Counter--;

    if (m_Paraser_Func_Counter == 0) {
        m_Paraser_Set_Lock = false;
    }

    std::vector<std::wstring> Words;
    if (!Generic_Split(Word, &Words)) {
        m_Paraser_Set_Lock = false;
        return false;
    } else {
        size_t Words_Size = Words.size();
        for (size_t i = 0; i < Words_Size; i++) {
            auto& Word = Words.at(i);

            if (!Brigadier_Double(Word)) {
                m_Paraser_Set_Lock = false;
                return false;
            }
        }
        if (m_Paraser_Func_Counter == 0) return true;
    }

    return false;
}
