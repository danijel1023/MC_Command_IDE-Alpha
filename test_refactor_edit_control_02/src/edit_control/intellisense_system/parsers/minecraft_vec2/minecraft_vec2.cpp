#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Vec2(std::wstring& Word) {
    if (m_Paraser_Func_Counter == 0) {
        m_Paraser_Func_Counter = 2;
        m_Paraser_Set_Lock = true;
    } else m_Paraser_Func_Counter--;

    if (m_Paraser_Func_Counter == 0) {
        m_Paraser_Set_Lock = false;
    }

    std::vector<std::wstring> Words;
    if (!Generic_Split(Word, &Words)) {
        m_Paraser_Set_Lock = false;
        return false;
    }

    size_t Words_Size = Words.size();
    for (size_t i = 0; i < Words_Size; i++) {
        auto& Word = Words.at(i);
        if (Word.size() == 0) {
            Error_Handler << L"Missing argument";
            m_Paraser_Set_Lock = false;
            return false;
        }


        if (Word.at(0) == L'~') {
            Word.erase(0, 1);
        }

        if (Word.size() != 0 && !Brigadier_Double(Word)) {
            //'Error_Handler' not necessary because 'Brigadier_Double' has it's own 'Error_Handler'
            m_Paraser_Set_Lock = false;
            return false;
        }
    }

    if (m_Paraser_Func_Counter == 0) return true;
    return false;
}
