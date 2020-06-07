#include "pch.h"
#include "snbt_formater.h"


bool SNbt_Formater::Is_Syntax_ch(wchar_t& ch) {
    switch (ch) {
    case L'[': break;
    case L']': break;
    case L'{': break;
    case L'}': break;
    case L':': break;
    case L',': break;
    default: return false;
    }

    return true;
}


bool SNbt_Formater::Is_Value(wchar_t ch) {
    switch (ch) {
    case L'U': break;
    case L'Q': break;
    case L'{': break;
    case L'[': break;
    default:
        return false;
    }
    return true;
}


bool SNbt_Formater::Compress_Str(std::wstring* Word_Ptr, std::wstring* Error) {
    auto& Word = *Word_Ptr;

    size_t Ancor = 0;
    bool Is_Str = false, Is_Un_QStr = false, Skip = false;
    size_t Word_Size = Word.size();
    for (size_t i = 0; i < Word_Size; i++) {
        wchar_t ch = Word.at(i);
        if (Skip) { Skip = false; }

        else if (Is_Str) {
            if (ch == L'\\') { Skip = true; }

            else if (ch == L'"') {
                size_t Size = i - Ancor;
                Word.erase(Ancor, Size);
                Word.at(Ancor) = L'Q';
                i -= Size;
                Word_Size -= Size;

                Ancor = i + 1;
                Is_Str = false;
            }
        }

        else if (ch == L'"') {
            if (Is_Un_QStr) {
                *Error = L"Ilegal char in unquoted string";
                return false;
            }

            Ancor = i;
            Is_Str = true;
        }

        else if (Is_Syntax_ch(ch) || i + 1 == Word_Size) {
            if (Is_Un_QStr || (i + 1 == Word_Size && !Is_Syntax_ch(ch))) {
                if (!(Std::Is_Lowercase(ch) || Std::Is_Number(ch) || Std::Is_Uppercase(ch)
                      || ch == L'+' || ch == L'-' || ch == L'_' || ch == L'.'
                      || Is_Syntax_ch(ch))) {
                    *Error = L"Ilegal char in unquoted string";
                    return false;
                }

                size_t Size = i - Ancor - 1 + (i + 1 == Word_Size && !Is_Syntax_ch(ch));
                if (!Std::Check_Spaces(Word.substr(Ancor, Size + 1))) {
                    *Error = L"Wrongly formated unquoted string";
                    return false;
                }

                Word.erase(Ancor, Size);
                Word.at(Ancor) = L'U';
                i -= Size;
                Word_Size -= Size;

                Is_Un_QStr = false;
            }

            Ancor = i + 1;
        }

        else {
            if (!(Std::Is_Lowercase(ch) || Std::Is_Number(ch) || Std::Is_Uppercase(ch)
                  || ch == L'+' || ch == L'-' || ch == L'_' || ch == L'.')) {
                *Error = L"Ilegal char in unquoted string";
                return false;
            }

            Is_Un_QStr = true;
        }
    }

    return true;
}
