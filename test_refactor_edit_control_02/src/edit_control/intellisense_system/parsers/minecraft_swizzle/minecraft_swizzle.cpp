#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Swizzle(std::wstring& Word) {
    bool Use_X = false, Use_Y = false, Use_Z = false;
    size_t Word_Size = Word.size();
    for (size_t i = 0; i < Word_Size; i++) {
        wchar_t ch = Word.at(i);

        if (ch == L'x') {
            if (!Use_X) Use_X = true; 
            else {
                Error_Handler << L"Already using 'x'";
                return false;
            }
        }
        else if (ch == L'y') {
            if (!Use_Y) Use_Y = true;
            else {
                Error_Handler << L"Already using 'y'";
                return false;
            }

        }
        else if (ch == L'z') {
            if (!Use_Z) Use_Z = true;
            else {
                Error_Handler << L"Already using 'z'";
                return false;
            }

        }

        else {
            Error_Handler << L"Only allowed combination of letters 'x', 'y' and 'z'";
            return false;
        }
    }


    return true;
}
