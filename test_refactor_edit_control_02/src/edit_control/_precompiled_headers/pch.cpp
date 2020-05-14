#include "pch.h"

bool Log_IO::Initialised = false;
size_t Log_IO::Init_Calls = 0;

bool Log_IO::Open_Console = false;
bool Log_IO::Open_File = false;
bool Log_IO::Stop_Output = true;
void (*Log_IO::Default_Color)() = Log_IO::Set_Color::Normal;

std::wostream* Log_IO::Conosole_Stream = NULL;
std::wofstream* Log_IO::File_Stream = NULL;

Log_wostream Log_IO::wcout_obj(&std::wcout);
