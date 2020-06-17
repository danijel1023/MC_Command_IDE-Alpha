#include "pch.h"
#include "edit_control.h"

Edit_Control::~Edit_Control() {}


void Edit_Control::Init(HWND Parent) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_EDIT_CONTROL);
    Log_IO::wcout() << "Init" << std::endl;
    m_InitCalls++;
    if (m_Is_Initialized) {
        MB_ERR("Initialization Function was Called More than 1 Time!");
        return;
    }
    Control_Register::Init();

    m_Parent = Parent;
    m_hWnd = CreateWindowEx(0, Control_Register::Class_Name, NULL,
                               WS_CHILD | WS_VISIBLE | WS_TABSTOP | SS_LEFT | SS_NOTIFY,
                               0, 0, 0, 0,
                               m_Parent, NULL, GetModuleHandle(NULL), NULL);

    CHECK_ERR(m_hWnd, ERR_MSG_CREATE_WINDOW_EX);
    Control_Register::Register_Instance(m_hWnd, this);

    m_Render.Init(m_hWnd);
    m_Keyboard.Init(m_hWnd);
    m_Mouse.Init(m_hWnd);
    m_Vert_Scrollbar.Init(m_hWnd, &m_Screen_YOff, SB_VERT, 1, 1);
    m_Horz_Scrollbar.Init(m_hWnd, &m_Screen_XOff, SB_HORZ, 1, 1);
    m_IntelliSense.Init(m_hWnd);
    m_Line_Number.Init(m_Parent);
    Set_Lines(m_Text.size());

    m_Is_Initialized = true;
}


bool Edit_Control::Open_File(const std::wstring& Path) {
    File_Path = Path;
    std::wifstream File(Path);
    if (!File.is_open()) {
        Log_IO::wcout() << "Error opening the file" << std::endl;
        return false;
    }

    std::wstring Text;
    if (File.is_open()) {
        std::wstringstream Buffer;
        Buffer << File.rdbuf();
        Text = Buffer.str();

        File.close();
    }

    Set_Text(Text);
    return true;
}


void Edit_Control::Set_Text(const std::wstring& New_Text) {
    Clear_Text();

    std::vector<std::wstring> Text;
    size_t Ancor = 0;
    size_t N_Text_Size = New_Text.size();
    for (size_t i = 0; i < N_Text_Size; i++) {
        if (New_Text.at(i) == L'\n' || i + 1 == N_Text_Size) {
            Text.push_back(New_Text.substr(Ancor, i - Ancor + (New_Text.at(i) != L'\n')));
            Ancor = i + 1;
        }

        if (New_Text.at(i) == L'\n' && i + 1 == N_Text_Size) {
            Text.push_back(L"");
        }
    }

    Set_Text(Text);
}


void Edit_Control::Set_Text(const std::vector<std::wstring>& New_Text) {
    m_Keyboard.Insert(New_Text, m_Caret);
}


void Edit_Control::Corect_Scrolling() {
    Log_IO::Start_Log_System obj(SYSTEM_ID_EDIT_CONTROL);
    int Screen_CaretY = (int)m_Caret.Y;
    if (Screen_CaretY < m_Screen_YOff) {
        Log_IO::wcout() << "Corect_Scrolling - scroll up" << std::endl;
        m_Screen_YOff = Screen_CaretY;

    } else if (Screen_CaretY > m_Screen_YOff + m_Screen_MaxY - 1) {
        Log_IO::wcout() << "Corect_Scrolling - scroll down" << std::endl;
        m_Screen_YOff = Screen_CaretY + 1 - m_Screen_MaxY;
    }

    int Screen_CaretX = (int)m_Caret.X;
    if (Screen_CaretX < m_Screen_XOff) {
        Log_IO::wcout() << "Corect_Scrolling - scroll up" << std::endl;
        m_Screen_XOff = Screen_CaretX;

    } else if (Screen_CaretX > m_Screen_XOff + m_Screen_MaxX - 1) {
        Log_IO::wcout() << "Corect_Scrolling - scroll down" << std::endl;
        m_Screen_XOff = Screen_CaretX + 1 - m_Screen_MaxX;
    }

    m_Horz_Scrollbar.Set_Scroll_Info();
    m_Vert_Scrollbar.Set_Scroll_Info();
}


std::wstring Edit_Control::Convert_To_LineNum(size_t Line, unsigned char Num_Size) {
    std::wstring LineStr;
    LineStr = std::to_wstring(Line);
    for (size_t i = LineStr.size(); i < Num_Size; i++) {
        LineStr.insert(LineStr.begin(), ' ');
    }

    return LineStr;
}


void Edit_Control::Size(RECT& Rect) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_EDIT_CONTROL);
    Log_IO::wcout() << "Size" << std::endl;
    m_Minimised = false;
    m_Rect = Rect;
    m_Render.Set_Size(Rect);

    m_Vert_Scrollbar.Set_Page(m_Screen_MaxY);
    m_Vert_Scrollbar.Set_Range((int)m_Text.size() + m_Screen_MaxY - 2);

    m_Horz_Scrollbar.Set_Page(m_Screen_MaxX);
    m_Horz_Scrollbar.Set_Range((int)m_MaxLine_Length.Line_Lenght + (int)m_Screen_MaxX - 20);
}


void Edit_Control::Minimized() {
    Log_IO::Start_Log_System obj(SYSTEM_ID_EDIT_CONTROL);
    Log_IO::wcout() << "Minimized" << std::endl;
    m_Minimised = true;
}


void Edit_Control::Set_Lines(size_t New_Number) {
    if (m_LineNumbers.size() > New_Number) {
        m_LineNumbers.erase(m_LineNumbers.begin() + New_Number, m_LineNumbers.begin() + m_LineNumbers.size());
        m_LineNumbers.shrink_to_fit();
        m_Vert_Scrollbar.Set_Range((int)New_Number + m_Screen_MaxY - 2);
    } else {
        for (size_t i = m_LineNumbers.size(); i < New_Number; i++) {
            m_LineNumbers.push_back(Convert_To_LineNum(i, m_LineNum_Length));
        }
        m_Vert_Scrollbar.Set_Range((int)New_Number + m_Screen_MaxY - 2);
    }
}


void Edit_Control::Calculate_Text_Metrics() {
    HDC hDC = GetDC(m_hWnd);
    CHECK_ERR(hDC, ERR_MSG_GET_DC);

    HFONT hfOld = (HFONT)SelectObject(hDC, m_Font);
    CHECK_ERR(hfOld, ERR_MSG_SELECT_OBJECT_HFONT);

    TEXTMETRIC tm;
    CHECK_ERR(GetTextMetrics(hDC, &tm), ERR_MSG_GET_TEXT_METRICS);
    m_Char_Width = tm.tmAveCharWidth;
    m_Char_Height = tm.tmHeight;
    m_Caret_Width = m_Char_Width / 10;
    m_Caret_Height = m_Char_Height;
    CHECK_ERR(SelectObject(hDC, hfOld), ERR_MSG_SELECT_OBJECT_HFONT);
    CHECK_ERR(ReleaseDC(m_hWnd, hDC), ERR_MSG_RELEASE_DC);
}

void Edit_Control::Clear_Text() {
    m_Text.erase(m_Text.begin(), m_Text.end());
    m_Text.emplace_back();

    m_TextColor.erase(m_TextColor.begin(), m_TextColor.end());
    m_TextColor.emplace_back();

    m_Caret.X = 0;
    m_Caret.Y = 0;
    m_CaretSel = m_Caret;
}


void Edit_Control::Differentiate_Start_End(Caret_Struct* Start, Caret_Struct* End, Caret_Struct& a, Caret_Struct& b) {
    if (a > b) {
        (*Start) = b;
        (*End) = a;
    } else {
        (*Start) = a;
        (*End) = b;
    }
}


COLORREF Edit_Control::Get_Last_Color(const Caret_Struct& Caret) {
    if (Caret.X != 0) {
        return m_TextColor.at(Caret.Y).at(Caret.X - 1);
    } else {
        if (Caret.Y == 0) {
            return m_Default_Text_Color;
        } else {
            if (m_TextColor.at(Caret.Y - 1).size() != 0) {
                return m_TextColor.at(Caret.Y - 1).back();
            } else {
                return m_Default_Text_Color;
            }
        }
    }
}
