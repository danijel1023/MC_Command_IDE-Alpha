#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Function(std::wstring Word) {
    std::wstring Error;
    if (!Std::Insert_Namespace(Word, &Error)) {
        Error_Handler << Error.c_str();
        return false;
    }

    bool Pass_NS = false, Correct_Path = false, Namespace = true;
    size_t Word_Size = Word.size();
    for (size_t i = 0; i < Word_Size; i++) {
        wchar_t ch = Word.at(i);
        if (Namespace) {
            if (ch == L':') {
                Namespace = false;
            }
        }

        else if (Correct_Path) {
            if (ch == L'/') {
                Correct_Path = false;
            }

            else if (!Std::Is_Std_ch(ch)) {
                Error_Handler << L"Ilegal char in namespace value";
                return false;
            }
        }

        else {
            if (ch == L'/') {
                Error_Handler << L"Cannot have '/' in a namespace value";
                return false;
            }

            else if (!Std::Is_Std_ch(ch)) {
                Error_Handler << L"Ilegal char in namespace value";
                return false;
            }

            else Correct_Path = true;
        }
    }


    if (!Correct_Path) {
        Error_Handler << L"Missing MC function name";
        return false;
    }

    return true;
}
