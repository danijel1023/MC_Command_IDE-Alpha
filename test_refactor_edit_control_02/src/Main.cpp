//[TODO]: "Comment whole code :("
#include "pch.h"
#include "edit_control/_edit_control/edit_control.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

std::vector<std::wstring> CMD_Params;
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int nCmdShow) {
    Log_IO Epc;    //Initialize Log Class -> will get cleaned up when the program teminates
    Log_IO::Set_IO(LOG_IO_REDIRECT_TO_CONSOLE);
    Log_IO::Enable_Time();
    Log_IO::Test_Colors();
    Log_IO::Set_Color::Set_Default_Color(Log_IO::Set_Color::Trace);
    Log_IO::Set_Color::Default();
    Log_IO::Print_Entry_Point();

    Log_IO::wcout() << L"pCmdLine = " << pCmdLine << std::endl;

    bool Is_Str = false;
    size_t Str_Ancor = 0;
    std::wstring StdpCmdLine = pCmdLine;
    for (size_t i = 0; pCmdLine[i]; i++) {
        if (Is_Str) {
            if (pCmdLine[i] == L'"') {
                Is_Str = false;
            }
        }

        else if (pCmdLine[i] == L'"') {
            Str_Ancor = i;
            Is_Str = true;
        }

        if ((pCmdLine[i] == L' ' || !pCmdLine[i + 1]) && !Is_Str) {
            CMD_Params.push_back(StdpCmdLine.substr(Str_Ancor, i - Str_Ancor + !pCmdLine[i + 1]));
            Str_Ancor = i + 1;
        }
    }

    if (CMD_Params.size() > 1)
        Log_IO::wcout() << L"Warning: multiple files supplied as input - ignoring rest of them" << std::endl;

    wchar_t Path_cStr[MAX_PATH];
    if (!GetModuleFileName(NULL, Path_cStr, MAX_PATH)) {
        MB_LAST_ERR();
    }
    std::wstring Path = Path_cStr;

    for (size_t i = Path.size() - 1; i > 0; i--) {
        if (Path.at(i) == L'\\') {
            Path.erase(Path.begin() + i, Path.end());
            break;
        }
    }

    if (!SetCurrentDirectory(Path.c_str())) {
        MB_LAST_ERR();
    }

    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    {
        WNDCLASSEX wc = { };

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = 0;
        wc.lpfnWndProc = WindowProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = CLASS_NAME;
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

        if (!RegisterClassEx(&wc)) {
            MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
            return 0;
        }
    }

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL) {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.message;
}




Edit_Control edit_control;
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
    {
        edit_control.Init(hWnd);
        if (CMD_Params.size() > 0)
            edit_control.Open_File(CMD_Params.at(0));
        break;
    }

    case WM_SIZE:
    {
        if (wParam == SIZE_MINIMIZED) {
            edit_control.Minimized();
        } else {
            RECT Rect;
            GetClientRect(hWnd, &Rect);
            edit_control.Size(Rect);
        }
        break;
    }

    case WM_CLOSE:
        DestroyWindow(hWnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
