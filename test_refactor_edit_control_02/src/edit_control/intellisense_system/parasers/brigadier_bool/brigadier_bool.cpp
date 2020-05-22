#include "pch.h"
#include "intelisense.h"
#include "pch.h"

bool IntelliSense::Brigadier_Bool(std::wstring& Word) {
    return (Word == L"true" || Word == L"false");
}
