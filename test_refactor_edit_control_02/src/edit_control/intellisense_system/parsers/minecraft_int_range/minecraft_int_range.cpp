#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Int_Range(std::wstring& Word, bool Use_Prop, bool Use_Min, int Min, bool Use_Max, int Max) {
    return Minecraft_Int_Range(Word, m_Syntax_Obj, Use_Prop, Use_Min, Min, Use_Max, Max);
}

bool IntelliSense::Minecraft_Int_Range(std::wstring& Word, Json& m_Syntax_Obj, bool Use_Prop, bool Use_Min, int Min, bool Use_Max, int Max) {
    if (Use_Prop && m_Syntax_Obj.Has_Name(L"properties")) {
        m_Syntax_Obj.Obj(L"properties");

        if (m_Syntax_Obj.Has_Name(L"min")) {
            m_Syntax_Obj.Obj(L"min");

            Min = m_Syntax_Obj.Num();
            Use_Min = true;

            m_Syntax_Obj.Back();
        }

        if (m_Syntax_Obj.Has_Name(L"max")) {
            m_Syntax_Obj.Obj(L"max");

            Max = m_Syntax_Obj.Num();
            Use_Max = true;

            m_Syntax_Obj.Back();
        }

        m_Syntax_Obj.Back();
    }
    
    size_t First_N_Pos = 0;
    size_t Second_N_Pos = 0;

    size_t Word_Size = Word.size();
    for (size_t i = 0; i < Word_Size; i++) {
        if (Word.at(i) == L'.') {
            if (Word.size() > i + 1 && Word.at(i + 1) == L'.') {
                First_N_Pos = i;
                Second_N_Pos = i + 2;
                break;
            }
        }
    }


    if (Second_N_Pos != 0) {    //Range
        std::wstring Min_Str = Word.substr(0, First_N_Pos);
        std::wstring Max_Str = Word.substr(Second_N_Pos);
        int Min_Result = 0, Max_Result = 0;

        if (Min_Str.size() == 0 && Max_Str.size() == 0) {
            Error_Handler << L"Expected value or range value";
            return false;
        }

        if (Min_Str.size()) {
            if (!Brigadier_Integer(Min_Str, true, &Min_Result, false)) {
                return false;
            }
        }

        if (Max_Str.size()) {
            if (!Brigadier_Integer(Max_Str, true, &Max_Result, false)) {
                return false;
            }
        }

        if (Min_Result > Max_Result) {
            Error_Handler << L"Min value cannot be larger than max value";
            return false;
        }

        if (Use_Min && Min_Result < Min) {
            Error_Handler << std::wstring(L"Min value cannot be smaller than " + std::to_wstring(Min)).c_str();
            return false;
        }

        if (Use_Max && Max_Result > Max) {
            Error_Handler << std::wstring(L"Max value cannot be larger than " + std::to_wstring(Max)).c_str();
            return false;
        }
    }
    
    else {  //Not-Range
        int Result = 0;
        if (!Brigadier_Integer(Word, true, &Result, false)) {
            Error_Handler << L"Number is not right integer format";
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
    }

    return true;
}
