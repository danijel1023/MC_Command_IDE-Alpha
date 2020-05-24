#include "pch.h"
#include "intelisense.h"

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

bool IntelliSense::Brigadier_Integer(std::wstring& Word) {
    bool Using_Min = false, Using_Max = false;
    int Min = 0, Max = 0;

    if (m_Syntax_Obj.Has_Name(L"properties")) {
        m_Syntax_Obj.Obj(L"properties");

        if (m_Syntax_Obj.Has_Name(L"min")) {
            Min = (int) m_Syntax_Obj.Obj(L"min").Num();
            Using_Min = true;
            m_Syntax_Obj.Back();
        }

        if (m_Syntax_Obj.Has_Name(L"max")) {
            Max = (int) m_Syntax_Obj.Obj(L"max").Num();
            Using_Max = true;
            m_Syntax_Obj.Back();
        }

        m_Syntax_Obj.Back();
    }

    if (!Is_Valid_Integer(Word)) {
        Error_Handler << L"Invalid double paramater";
        return false;
    }

    int Result;
    try {
        Result = std::stoi(Word);
    }
    catch (...) {
        Error_Handler << L"Invalid integer paramater";
        return false;
    }

    if (Using_Min && Result < Min) {
        Error_Handler << L"Out of range (less than min)";
        return false;
    } else if (Using_Max && Result > Max) {
        Error_Handler << L"Out of range (more than max)";
        return false;
    }
    
    return true;
}
