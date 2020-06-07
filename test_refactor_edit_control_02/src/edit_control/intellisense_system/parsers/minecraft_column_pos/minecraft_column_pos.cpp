#include "pch.h"
#include "intelisense.h"
#include "pch.h"

bool IntelliSense::Minecraft_Column_Pos(std::wstring& Word) {
    if (m_Paraser_Func_Counter == 0) {
        m_Paraser_Func_Counter = 1;
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

    bool Using_Caret = false;
    if (Word.at(0) == L'^') Using_Caret = true;

    size_t Words_Size = Words.size();
    for (size_t i = 0; i < Words_Size; i++) {
        auto& Word = Words.at(i);
        if (Word.size() == 0) {
            Error_Handler << L"Missing argument";
            m_Paraser_Set_Lock = false;
            return false;
        }


        if (Using_Caret) {
            if (Word.at(0) != L'^') {
                Error_Handler << L"Cannot mix world and local coords (everithing must use '^' or not)";
                m_Paraser_Set_Lock = false;
                return false;
            }


            Word.erase(0, 1);
            if (Word.size() != 0 && !Brigadier_Double(Word)) {
                m_Paraser_Set_Lock = false;
                return false;
            }
        }


        else {
            if (Word.at(0) == L'^') {
                Error_Handler << L"Cannot mix world and local coords (everithing must use '^' or not)";
                m_Paraser_Set_Lock = false;
                return false;
            }

            if (Word.at(0) == L'~') {
                Word.erase(0, 1);
            }

            if (Word.size() != 0 && !Brigadier_Double(Word)) {
                m_Paraser_Set_Lock = false;
                return false;
            }
        }
    }

    if (m_Paraser_Func_Counter == 0) return true;
    return false;
}
