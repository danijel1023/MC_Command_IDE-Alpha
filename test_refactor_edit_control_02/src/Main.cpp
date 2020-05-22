//[TODO]: "Comment whole code :("
#include "pch.h"
#include "edit_control/_edit_control/edit_control.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int nCmdShow) {
    Log_IO Epc;    //Initialize Log Class -> will get cleaned up when the program teminates
    Log_IO::Set_IO(LOG_IO_REDIRECT_TO_CONSOLE, std::wstring(L"e32"));
    Log_IO::Enable_Time();
    Log_IO::Test_Colors();
    Log_IO::Set_Color::Set_Default_Color(Log_IO::Set_Color::Trace);
    Log_IO::Set_Color::Default();
    Log_IO::Print_Entry_Point();

    Log_IO::wcout() << L"pCmdLine = " << pCmdLine << std::endl;

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
        edit_control.Init(hWnd);
        break;

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
