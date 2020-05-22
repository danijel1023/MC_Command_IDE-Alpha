#include "pch.h"
#include "intelisense.h"
#include "entity_struct.h"


static bool Has_Allowed_Chars(std::wstring Name);

bool IntelliSense::Validate_Name(std::wstring Name) {
    if (Name.size() == 0) return true;
    if (Name.at(1) == L'!') Name.erase(1);

    if (!Has_Allowed_Chars(Name)) {
        Error_Handler << L"Name containing ilegal char(s)";
        return false;
    }

    if (Name.size() > 16) {
        Error_Handler << L"Too long Name: " << std::to_wstring(Name.size()).c_str();
        return false;
    }

    return true;
}

bool IntelliSense::Validate_NBT(std::wstring Word) {
    return true;
}

bool IntelliSense::Validate_Advancements(std::wstring Word) {
    return true;
}

bool IntelliSense::Validate_Tags(std::wstring Word) {
    return true;
}

bool IntelliSense::Validate_Scores(std::wstring Word) {
    return true;
}

bool IntelliSense::Get_Num(std::wstring Word, MC_NType* Num) {
    return true;
}

bool IntelliSense::Get_Range(std::wstring Word, RANGE* Num) {
    return true;
}






static bool Has_Allowed_Chars(std::wstring Name) {
    size_t Name_Size = Name.size();
    for (size_t i = 0; i < Name_Size; i++) {
        switch (Name.at(i)) {
        case L'a': break;
        case L'b': break;
        case L'c': break;
        case L'd': break;
        case L'e': break;
        case L'f': break;
        case L'g': break;
        case L'h': break;
        case L'i': break;
        case L'j': break;
        case L'k': break;
        case L'l': break;
        case L'm': break;
        case L'n': break;
        case L'o': break;
        case L'p': break;
        case L'q': break;
        case L'r': break;
        case L's': break;
        case L't': break;
        case L'u': break;
        case L'v': break;
        case L'w': break;
        case L'x': break;
        case L'y': break;
        case L'z': break;
        case L'A': break;
        case L'B': break;
        case L'C': break;
        case L'D': break;
        case L'E': break;
        case L'F': break;
        case L'G': break;
        case L'H': break;
        case L'I': break;
        case L'J': break;
        case L'K': break;
        case L'L': break;
        case L'M': break;
        case L'N': break;
        case L'O': break;
        case L'P': break;
        case L'Q': break;
        case L'R': break;
        case L'S': break;
        case L'T': break;
        case L'U': break;
        case L'V': break;
        case L'W': break;
        case L'X': break;
        case L'Y': break;
        case L'Z': break;
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
        case L'_': break;
        default: return false;
        }
    }
    return true;
}