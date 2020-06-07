#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Brigadier_Float(std::wstring& Word, bool Use_Ret, double* Ret_Val, bool Use_Prop, bool Use_Min, double Min, bool Use_Max, double Max) {
    return Brigadier_Double(Word, m_Syntax_Obj, Use_Ret, Ret_Val, Use_Prop, Use_Min, Use_Max, Min, Max);
}

bool IntelliSense::Brigadier_Float(std::wstring& Word, Json& m_Syntax_Obj, bool Use_Ret, double* Ret_Val, bool Use_Prop, bool Use_Min, double Min, bool Use_Max, double Max) {
    return Brigadier_Double(Word, m_Syntax_Obj, Use_Ret, Ret_Val, Use_Prop, Use_Min, Use_Max, Min, Max);
}
