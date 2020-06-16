#include "pch.h"
#include "intelisense.h"


bool IntelliSense::Brigadier_Double(std::wstring& Word, bool Use_Ret, double* Ret_Val, bool Use_Prop, bool Use_Min, double Min, bool Use_Max, double Max) {
    return Brigadier_Double(Word, m_Syntax_Obj, Use_Ret, Ret_Val, Use_Prop, Use_Min, Min, Use_Max, Max);
}

bool IntelliSense::Brigadier_Double(std::wstring& Word, Json& m_Syntax_Obj, bool Use_Ret, double* Ret_Val, bool Use_Prop, bool Use_Min, double Min, bool Use_Max, double Max) {
    if (Use_Prop && m_Syntax_Obj.Has_Name(L"properties")) {
        m_Syntax_Obj.Obj(L"properties");

        if (m_Syntax_Obj.Has_Name(L"min")) {
            Min = m_Syntax_Obj.Obj(L"min").Num();
            Use_Min = true;
            m_Syntax_Obj.Back();
        }

        if (m_Syntax_Obj.Has_Name(L"max")) {
            Max = m_Syntax_Obj.Obj(L"max").Num();
            Use_Max = true;
            m_Syntax_Obj.Back();
        }

        m_Syntax_Obj.Back();
    }

    if (!Std::Is_Valid_Double(Word)) {
        Error_Handler << L"Invalid double paramater";
        return false;
    }

    double Result;
    try {
        Result = std::stod(Word);
    }
    catch (...) {
        Error_Handler << L"Invalid double paramater";
        return false;
    }

    if (Use_Min && Result < Min) {
        Error_Handler << L"Out of range (less than min)";
        return false;
    }
    
    if (Use_Max && Result > Max) {
        Error_Handler << L"Out of range (more than max)";
        return false;
    }

    if (Use_Ret) *Ret_Val = Result;

    return true;
}
