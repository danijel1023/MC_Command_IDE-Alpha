#pragma once

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <iomanip>

#define LOG_IO_REDIRECT_TO_FILE 0
#define LOG_IO_REDIRECT_TO_CONSOLE 1
#define LOG_IO_STOP 2

//--Systems IDs---------
#define SYSTEM_ID_VERT_SCROLLBAR    0b00000001
#define SYSTEM_ID_HORZ_SCROLLBAR    0b00000010
#define SYSTEM_ID_INTELLISENSE      0b00000100
#define SYSTEM_ID_KEYBOARD          0b00001000
#define SYSTEM_ID_RENDER            0b00010000
#define SYSTEM_ID_MOUSE             0b00100000
#define SYSTEM_ID_LINE_NUMBER       0b01000000
#define SYSTEM_ID_EDIT_CONTROL      0b10000000
// ---------------------

struct Log_wostream {
    template<class T>
    Log_wostream& operator<<(const T stuff) {
        if (Is_Allowed(m_Mode))
            (*m_Stream_Ptr) << stuff;
        return (*this);
    }

    Log_wostream& operator<< (std::wostream& (*f)(std::wostream&)) {
        if (Is_Allowed(m_Mode))
            f(*m_Stream_Ptr);
        return (*this);
    }

    Log_wostream& operator<< (std::wostream& (*f)(std::wios&)) {
        if (Is_Allowed(m_Mode))
            f(*m_Stream_Ptr);
        return (*this);
    }

    Log_wostream& operator<< (std::wostream& (*f)(std::ios_base&)) {
        if (Is_Allowed(m_Mode))
            f(*m_Stream_Ptr);
        return (*this);
    }


    const bool Is_Allowed(const unsigned int Mode) {
        return m_Allowed_Modes & Mode;
    }


    const void Add_Allowed(const unsigned int Mode) {
        m_Allowed_Modes |= Mode;
    }

    const void Remove_Allowed(const unsigned int Mode) {
        m_Allowed_Modes ^= Mode;
    }


    Log_wostream() = delete;
    Log_wostream(std::wostream* Stream_Ptr)
        :m_Stream_Ptr(Stream_Ptr) {}


    std::wostream* m_Stream_Ptr = 0;
    unsigned int m_Mode = -1;
    unsigned int m_Allowed_Modes = -1; //All modes are allowed
};


struct Log_IO {
    static Log_wostream wcout_obj;

    static Log_wostream& wcout() {
        wcout_obj << Convert_Mode(wcout_obj.m_Mode);
        return wcout_obj;
    }


    static void Add_Allowed(const unsigned int Mode) { wcout_obj.Add_Allowed(Mode); }
    static void Remove_Allowed(const unsigned int Mode) { wcout_obj.Remove_Allowed(Mode); }


    struct Start_Log_System {
        Start_Log_System() = delete;
        Start_Log_System(const Start_Log_System&) = delete;
        Start_Log_System(const unsigned int Mode) { Start_Log_System_Call(Mode); }
        ~Start_Log_System() { End_Log_System_Call(); }
    };


    static void Start_Log_System_Call(const unsigned int Mode) { wcout_obj.m_Mode = Mode; }
    static void End_Log_System_Call() { wcout_obj.m_Mode = -1; }


    static void Normal()        { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); }
    static void Trace()         { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3); }
    static void Info()          { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); }
    static void Warn()          { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); }
    static void Error()         { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); }
    static void Fatal_Error()   { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xCB); }


    static const void Set_IO(const unsigned int Mode, const std::wstring& File_Path = std::wstring(L"Default_Log_File")) {
        switch (Mode) {
        case LOG_IO_REDIRECT_TO_FILE:
            if (Stop_Output) {
                Stop_Output = false;
                wcout_obj.Add_Allowed(-1);
            }
            if (Open_File) break;
            if (Open_Console) {
                fclose(stdin);
                fclose(stdout);
                fclose(stderr);
                CHECK_ERR(FreeConsole(), ERR_MSG_FREE_CONSOLE);
            }

            File_Stream = new std::wofstream;
            File_Stream->open(File_Path, std::ios::out | std::ios::app);
            CHECK_ERR(File_Stream->is_open(), "Couldn't open log file. \r\n-Terminating program.");

            {
                bool Erase_File = false;
                {
                    std::wifstream Temp_Input(File_Path);
                    if (!(Temp_Input.peek() == std::wifstream::traits_type::eof())) {
                        if (MessageBox(NULL, L"The log file has some text in it.\r\nDo you want to clear the log file?", L"Info", MB_YESNO) == IDYES) {
                            Erase_File = true;
                        }
                    }
                    Temp_Input.close();
                }

                if (Erase_File) {
                    File_Stream->close();
                    File_Stream->open(File_Path, std::ios::out | std::ios::trunc);
                    CHECK_ERR(File_Stream->is_open(), "Couldn't open log file. \r\n-Terminating program.");
                }
            }

            Open_File = true;

            wcout_obj.m_Stream_Ptr = File_Stream;
            break;
        case LOG_IO_REDIRECT_TO_CONSOLE:
            if (Stop_Output) {
                Stop_Output = false;
                wcout_obj.Add_Allowed(-1);
            }
            if (Open_Console) break;
            if (Open_File) {
                File_Stream->close();
                delete File_Stream;
                Open_File = false;
            }

            {
                FILE* stream;
                CHECK_ERR(AllocConsole(), ERR_MSG_ALLOC_CONSOLE);
                if (_wfreopen_s(&stream, L"conin$", L"r", stdin) != 0) {
                    MessageBox(NULL, L"Setting conin$ (in) Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
                }
                if (_wfreopen_s(&stream, L"conout$", L"w", stdout) != 0) {
                    MessageBox(NULL, L"Setting conout$ (out) Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
                }
                if (_wfreopen_s(&stream, L"conout$", L"w", stderr) != 0) {
                    MessageBox(NULL, L"Setting conout$ (err) Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
                }
                Conosole_Stream = &std::wcout;
                Open_Console = true;
            }

            wcout_obj.m_Stream_Ptr = Conosole_Stream;
            break;
        case LOG_IO_STOP:
            wcout_obj.Remove_Allowed(-1);
            Stop_Output = true;
            break;
        }
    }


    static void OutputInfo() {
        time_t t = time(0);   // get time now
        struct tm now;
        localtime_s(&now, &t);

        wcout() << std::setfill(L'0');

        wcout() << "|------------------------------------------------------|" << std::endl
            << "|                                                      |" << std::endl
            << "|---------------- | Log Entry Point | -----------------|" << std::endl
            << "|                                                      |" << std::endl;

        wcout() << "|--------------  "
            << '[' << std::setw(2) << (now.tm_year + 1900) << '-' << std::setw(2) << (now.tm_mon + 1) << '-' << std::setw(2) << now.tm_mday << ']'
            << ' ' << std::setw(2) << now.tm_hour << ':' << std::setw(2) << now.tm_min << ':' << std::setw(2) << now.tm_sec
            << "  ---------------|" << std::endl;

        wcout() << "|                                                      |" << std::endl
            << "|------------------------------------------------------|" << std::endl;
    }


    static const wchar_t* Convert_Mode(const unsigned int Mode) {
        switch (Mode) {
        case SYSTEM_ID_VERT_SCROLLBAR:  return L"[Vert Scrollbar System]: ";
        case SYSTEM_ID_HORZ_SCROLLBAR:  return L"[Horz Scrollbar System]: ";
        case SYSTEM_ID_KEYBOARD:        return L"[Keyboard System]: ";
        case SYSTEM_ID_RENDER:          return L"[Render System]: ";
        case SYSTEM_ID_MOUSE:           return L"[Mouse System]: ";
        case SYSTEM_ID_LINE_NUMBER:     return L"[Line Number System]: ";
        case SYSTEM_ID_EDIT_CONTROL:    return L"[Edit Control System]: ";
        case SYSTEM_ID_INTELLISENSE:    return L"[IntelliSense System]: ";
        default:
            return L"";
        }
    }


    Log_IO(const Log_IO&) = delete;
    Log_IO() {
        Init_Calls++;
        Initialised = true;
    }

    ~Log_IO() {
        if (Init_Calls != 0) {
            Init_Calls--;
            return;
        }
        
        if (Open_File) {
            File_Stream->close();
            delete File_Stream;
        }

        if (Open_Console) {
            fclose(stdin);
            fclose(stdout);
            fclose(stderr);
            HWND Console = GetConsoleWindow();
            CHECK_ERR(FreeConsole(), ERR_MSG_FREE_CONSOLE);
            SendMessage(Console, WM_CLOSE, 0, 0);
        }
        Initialised = false;
    }

private:
    static bool Initialised;
    static size_t Init_Calls;

    static bool Open_Console;
    static bool Open_File;
    static bool Stop_Output;

    static std::wostream* Conosole_Stream;
    static std::wofstream* File_Stream;
};


#define DBG_Err(x) {\
  Log_IO::Error();\
  std::cout << x << std::endl;\
  Log_IO::Normal();\
}

#define DBG_Warn(x) {\
  Log_IO::Warning();\
  std::cout << x << std::endl;\
  Log_IO::Normal();\
}

#define DBG_FatErr(x) {\
  Log_IO::Fatal_Error();\
  std::cout << x << std::endl;\
  Log_IO::Normal();\
}

#define DBG_Trace(x) {\
  Log_IO::Trace();\
  std::cout << x << std::endl;\
  Log_IO::Normal();\
}
