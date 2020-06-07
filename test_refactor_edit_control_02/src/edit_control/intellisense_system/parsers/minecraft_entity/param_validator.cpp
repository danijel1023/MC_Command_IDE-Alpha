#include "pch.h"
#include "intelisense.h"
#include "entity_struct.h"

bool IntelliSense::Validate_Name(std::wstring Name) {
    if (Name.size() == 0) return true;

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
