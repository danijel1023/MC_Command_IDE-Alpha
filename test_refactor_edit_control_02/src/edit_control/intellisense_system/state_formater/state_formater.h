#pragma once
#include "pch.h"

namespace State_Formater {
    bool Seperate_State(std::wstring& Word, std::vector<std::wstring>* Params, std::wstring* Error, wchar_t&& Open, wchar_t&& Close);

};
