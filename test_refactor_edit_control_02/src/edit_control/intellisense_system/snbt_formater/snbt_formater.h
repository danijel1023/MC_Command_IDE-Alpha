#pragma once
#include "pch.h"

namespace SNbt_Formater {
    bool Is_Valid_c(std::wstring Word, std::wstring* Error);
    bool Is_Valid(std::wstring* Word_Ptr, std::wstring* Error);

    //*** Private ***
    bool Compress_Str(std::wstring* Word_Ptr, std::wstring* Error);
    //*** Private ***
    bool Is_Syntax_ch(wchar_t& ch);
    //*** Private ***
    bool Is_Value(wchar_t ch);

};
