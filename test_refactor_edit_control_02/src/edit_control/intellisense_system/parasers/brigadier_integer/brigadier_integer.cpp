#include "pch.h"
#include "intelisense.h"
#include "pch.h"

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
    for (size_t i = 0; i < Word.size(); i++) {
        if (!Is_Number(Word.at(i))) {
            Error_Handler << L"Found non-number chars";
            return false;
        }
    }
    return true;
}
