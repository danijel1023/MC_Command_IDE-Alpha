#include "pch.h"
#include "intelisense.h"
#include "pch.h"

static bool Is_Number(wchar_t ch) {
    switch (ch) {
    case L'0': break;
    case L'1': break;
    case L'2': break;
    case L'3': break;
    case L'4': break;
    case L'5': break;
    case L'6': break;
    case L'7': break;
    case L'8': break;
    case L'9': break;
    default: return false;
    }
    return true;
}

bool IntelliSense::Brigadier_Double(std::wstring& Word) {
    bool Has_Decimal = true;

    bool Using_Min = false, Using_Max = false;
    double Min = 0.0, Max = 0.0;

    if (m_Syntax_Obj.Has_Name(L"properties")) {
        m_Syntax_Obj.Obj(L"properties");

        if (m_Syntax_Obj.Has_Name(L"min")) {
            Min = m_Syntax_Obj.Obj(L"min").Num();
            Using_Min = true;
            m_Syntax_Obj.Back();
        }

        if (m_Syntax_Obj.Has_Name(L"max")) {
            Max = m_Syntax_Obj.Obj(L"max").Num();
            Using_Max = true;
            m_Syntax_Obj.Back();
        }

        m_Syntax_Obj.Back();
    }

    for (size_t i = 0; i < Word.size(); i++) {
        if (Word.at(i) == L'.')
            if (Has_Decimal) {
                Error_Handler << L"Found extra '.'";
            } else {
                Has_Decimal = true;
            }

        else if (!Is_Number(Word.at(i))) {
            Error_Handler << L"Found non-number chars";
            return false;
        }
    }

    double Result = std::stod(Word);

    if (Using_Min && Result < Min) {
        Error_Handler << L"Out of range (less than min)";
        return false;
    }

    if (Using_Max && Result > Max) {
        Error_Handler << L"Out of range (more than max)";
        return false;
    }


    return true;
}
