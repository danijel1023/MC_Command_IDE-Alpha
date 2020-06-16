#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Brigadier_Bool(std::wstring& Word) {
    if (Word != L"true" && Word != L"false") {
        Error_Handler << L"Word must be 'true' or 'false'";
        return false;
    }
    return true;
}
