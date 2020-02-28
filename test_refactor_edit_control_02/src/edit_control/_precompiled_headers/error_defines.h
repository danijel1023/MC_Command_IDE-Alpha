#pragma once
#include "error_msg_defines.h"

#define RAW_MB_ERR(x) MessageBox(NULL, x, TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK)
#define RAW_MB_WARN(x) MessageBox(NULL, x, TEXT("Warning!"), MB_ICONEXCLAMATION | MB_OK)

#define MB_ERR(x) MessageBox(NULL, TEXT(x), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK)
#define MB_WARN(x) MessageBox(NULL, TEXT(x), TEXT("Warning!"), MB_ICONEXCLAMATION | MB_OK)

#define MB_LAST_ERR(){ \
  LPTSTR messageBuffer = NULL;\
  size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,\
  NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&messageBuffer, 0, NULL);\
  if (messageBuffer != NULL) {\
    RAW_MB_ERR(messageBuffer);\
    LocalFree(messageBuffer);\
  }\
}\
0    //Make sure to put ";" on the end

#define CHECK_ERR(Test_Val, Message) \
if(Test_Val == NULL) {\
  __debugbreak();\
  MB_ERR(Message);\
  MB_LAST_ERR();\
  PostQuitMessage(-1);\
}\
0    //Make sure to put ";" on the end

#define CHECK_ERR_VAL_EMPTY
#define CHECK_ERR_RET(Test_Val, Message, Ret_Val) \
if(Test_Val == NULL) {\
  return Ret_Val;\
}\
0    //Make sure to put ";" on the end

#define __debugbreak() __debugbreak()
#define debugbreak() __debugbreak()
