#pragma once

#pragma comment(lib, "comctl32.lib")

#ifndef UNICODE
#define UNICODE
#endif

#include <vector>
template <typename T>
std::vector<T>& operator+=(std::vector<T>& v1, const std::vector<T>& v2) {
    v1.insert(v1.end(), v2.begin(), v2.end());
    return v1;
}

#include <string>

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>   //SetWindowSubclass

#include "error_defines.h"

// -- IO ---------------
#include "debugg_defines.h"
// ---------------------
