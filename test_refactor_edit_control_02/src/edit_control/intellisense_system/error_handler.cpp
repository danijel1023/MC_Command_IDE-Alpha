#include "pch.h"
#include "error_handler.h"


Error_Handler& Error_Handler::operator<<(const wchar_t* Err) {
    Register_Error(Err);
    return *this;
}

void Error_Handler::Register_Error(std::wstring Err) {
    if (Error.size() == 0) {
        Error = Err;
    } else {
        Error.push_back(L'\n');
        Error += Err;
    }
}


std::wstring Error_Handler::Get_Error() {
    std::wstring Ret = Error;
    Error = L"";
    return Ret;
}


bool Error_Handler::Has_Error() {
    return Error.size();
}
