#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Brigadier_String(std::wstring& Word, std::wstring Properties) {
    return Brigadier_String(Word, m_Syntax_Obj, Properties);
}

bool IntelliSense::Brigadier_String(std::wstring& Word, Json& m_Syntax_Obj, std::wstring Properties) {
    if (Properties.size() == 0 && m_Syntax_Obj.Has_Name(L"properties")) {
        m_Syntax_Obj.Obj(L"properties");

        if (m_Syntax_Obj.Has_Name(L"type")) {
            m_Syntax_Obj.Obj(L"type");

            Properties = m_Syntax_Obj.Str();

            m_Syntax_Obj.Back();
        }

        m_Syntax_Obj.Back();
    }

    if (Properties == L"word") {
        size_t Word_Size = Word.size();
        for (size_t i = 0; i < Word_Size; i++) {
            wchar_t ch = Word.at(i);
            if (!(Std::Is_Lowercase(ch) || Std::Is_Uppercase(ch) || Std::Is_Number(ch) ||
                  ch == L'.' || ch == L'_' || ch == L'-' || ch == L'+')) {
                Error_Handler << L"String containing ilegal chars";
                return false;
            }
        }

        return true;
    }
    
    else if (Properties == L"phrase") {
        size_t Word_Size = Word.size();

        if (Word_Size < 2) {
            Error_Handler << L"Wrongly formated string";
            return false;
        }

        if (Word.at(0) != L'"' || Word.back() != L'"') {
            Error_Handler << L"Wrongly formated string";
            return false;
        }

        Word_Size--;
        for (size_t i = 1; i < Word_Size; i++) {
            wchar_t ch = Word.at(i);
            if (ch == L'"') {
                Error_Handler << L"Wrongly formated string";
                return false;
            }
        }

        return true;
    }
    
    else if (Properties == L"greedy") {
        m_Paraser_Set_Lock = true;
        return true;
    }
    
    else {
        Error_Handler << L"Unknown paramater for 'Brigadier_String'";
        return false;
    }

    return true;
}
