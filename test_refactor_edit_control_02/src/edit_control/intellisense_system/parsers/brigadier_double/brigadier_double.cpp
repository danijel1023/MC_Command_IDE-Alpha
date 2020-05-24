#include "pch.h"
#include "intelisense.h"
#include "pch.h"

bool IntelliSense::Brigadier_Double(std::wstring& Word) {
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

    double Result;
    try {
        Result = std::stod(Word);
    }
    catch (const std::invalid_argument& ia) {
        Error_Handler << L"Invalid double paramater";
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
