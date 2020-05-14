#pragma once
#include "pch.h"

class Error_Handler {
public:
    Error_Handler& operator<<(const wchar_t* Err);
    void Register_Error(std::wstring Err);

    std::wstring Get_Error();
    bool Has_Error();

private:
    std::wstring Error;
};
