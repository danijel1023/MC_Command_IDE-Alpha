#pragma once
#include <string>

namespace Std {
    inline bool Is_Lowercase(wchar_t& ch);
    inline bool Is_Uppercase(wchar_t& ch);
    inline bool Is_Number(wchar_t& ch);
    inline bool Is_Valid_Double(std::wstring& Word);
    inline bool Is_Valid_Integer(std::wstring& Word);
    inline void Clear_Spaces(std::wstring* Word_Ptr);
    inline bool Check_Spaces(std::wstring& Word);
    inline bool Is_Std_Str(std::wstring Str);
    inline bool Is_Std_ch(wchar_t ch);
    inline bool Has_Namespace(std::wstring& Word);
    inline bool Insert_Namespace(std::wstring& Word, std::wstring* Error);
};

#include "standard.inl"
