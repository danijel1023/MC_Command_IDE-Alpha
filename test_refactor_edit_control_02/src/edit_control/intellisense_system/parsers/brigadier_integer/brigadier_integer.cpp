#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Brigadier_Integer(std::wstring& Word, bool Use_Ret, int* Ret_Val, bool Use_Prop, bool Use_Min, int Min, bool Use_Max, int Max) {
    return Brigadier_Integer(Word, m_Syntax_Obj, Use_Ret, Ret_Val, Use_Prop, Use_Min, Min, Use_Max, Max);
}

bool IntelliSense::Brigadier_Integer(std::wstring& Word, Json& m_Syntax_Obj, bool Use_Ret = false, int* Ret_Val = nullptr, bool Use_Prop = true, bool Use_Min = false, int Min = 0, bool Use_Max = false, int Max = 0) {
    bool Use_Min = false, Use_Max = false;
    int Min = 0, Max = 0;

    if (Use_Prop && m_Syntax_Obj.Has_Name(L"properties")) {
        m_Syntax_Obj.Obj(L"properties");

        if (m_Syntax_Obj.Has_Name(L"min")) {
            Min = (int) m_Syntax_Obj.Obj(L"min").Num();
            Use_Min = true;
            m_Syntax_Obj.Back();
        }

        if (m_Syntax_Obj.Has_Name(L"max")) {
            Max = (int) m_Syntax_Obj.Obj(L"max").Num();
            Use_Max = true;
            m_Syntax_Obj.Back();
        }

        m_Syntax_Obj.Back();
    }

    if (!Std::Is_Valid_Integer(Word)) {
        Error_Handler << L"Invalid integer paramater";
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

    if (Use_Min && Result < Min) {
        Error_Handler << std::wstring(L"Value cannot be smaller than " + std::to_wstring(Min)).c_str();
        return false;
    }
    
    if (Use_Max && Result > Max) {
        Error_Handler << std::wstring(L"Value cannot be larger than " + std::to_wstring(Max)).c_str();
        return false;
    }
    
    return true;
}
