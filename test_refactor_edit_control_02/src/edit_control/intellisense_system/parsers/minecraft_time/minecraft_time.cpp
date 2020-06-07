#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Time(std::wstring& Word) {
    bool End = false;
    size_t Word_Size = Word.size();
    for (size_t i = 0; i < Word_Size; i++) {
        wchar_t ch = Word.at(i);
        if (End) {
            Error_Handler << L"Wrong time format";
            return false;
        }

        else if (!Std::Is_Number(ch)) {
            if (End) {
                Error_Handler << L"Wrong time format";
                return false;
            } else {
                if (ch == L'd' || ch == L's' || ch == L't') {
                    End = true;
                } else {
                    Error_Handler << L"Wrong time format";
                    return false;
                }
            }
        }
    }

    return true;
}
