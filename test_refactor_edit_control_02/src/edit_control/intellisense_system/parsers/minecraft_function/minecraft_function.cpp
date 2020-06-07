#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Function(std::wstring& Word) {
    bool Has_Name_Space = false;
    size_t Seperator_Pos = 0;
    std::wstring Name_Space;
    std::wstring Path;

    size_t Word_Size = Word.size();
    for (size_t i = 0; i < Word_Size; i++) {
        wchar_t ch = Word.at(i);
        if (ch == L':') {
            if (Has_Name_Space) {
                Error_Handler << L"The function path paramater contains more than 1 ':'";
                return false;
            }

            Has_Name_Space = true;
            Name_Space = Word.substr(0, i);
        }


        else if (!Has_Name_Space) {
            if (!(Std::Is_Lowercase(ch) || Std::Is_Number(ch) || ch == L'_' || ch == L'-' || ch == L'.')) {
                Error_Handler << L"The function namespace paramater contains ilegal chars";
                return false;
            }
        } else if (Has_Name_Space) {
            if (!(Std::Is_Lowercase(ch) || Std::Std::Is_Number(ch) || ch == L'_' || ch == L'-' || ch == L'.' || ch == L'/')) {
                Error_Handler << L"The function path paramater contains ilegal chars";
                return false;
            } else if (ch == L'/') {
                if (Seperator_Pos + 1 == i) {
                    Error_Handler << L"The function path paramater contains two or more '/' in a row";
                    return false;
                }
                Seperator_Pos = i;
            }
        }
    }

    if (Seperator_Pos + 1 == Word.size()) {
        Error_Handler << L"The function path paramater doesn't contain function name";
        return false;
    }

    Path = Word.substr(Name_Space.size() + 1);

    if (!Has_Name_Space || Path.size() == 0) {
        Error_Handler << L"The function path paramater doesn't exist";
        return false;
    }

    return true;
}
