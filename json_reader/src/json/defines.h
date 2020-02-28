#pragma once
#include <iostream>
#include <Windows.h>

#define log(x) std::wcout << x << std::endl;

#ifdef __lib_build 
#define log_err(x) errors << x
#define log_fat_err(x) errors << x << std::wstring(L"\n")

#else
#define default_color 7
#define log_err(x) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);\
                     std::wcout << x << std::endl;\
                     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), default_color); }

#define log_fat_err(x) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xCB);\
                         std::wcout << x << std::endl;\
                         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), default_color); }
#endif


#ifdef __RELEASE
#define __debugbreak()
#else
#define __debugbreak() __debugbreak()
#endif

#ifdef __RELEASE
#define log_trace(x)
#define log_info(x)
#define log_warn(x)
#else

#define default_color 7
#define log_trace(x) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);\
                          std::wcout << x << std::endl;\
                          SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), default_color); }

#define log_info(x) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);\
                          std::wcout << x << std::endl;\
                          SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), default_color); }

#define log_warn(x) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);\
                         std::wcout << x << std::endl;\
                         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), default_color); }

#endif

#define JSON_FATAL_ERR(x) { log_fat_err(x); Json::~Json(); m_Halt = true; __debugbreak(); return; }
#define FATAL_ERR(x) { log_fat_err(x); __debugbreak(); return; }

#define logtrace(x) log_trace(x)
#define loginfo(x) log_info(x)
#define logwarn(x) log_warn(x)
#define logerr(x) log_err(x)

#define get_last_err_mberr() \
{\
  wchar_t err[256];\
  memset(err, 0, 256);\
  FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),\
                 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err, 255, NULL);\
  MessageBox(NULL, err, TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);\
}
