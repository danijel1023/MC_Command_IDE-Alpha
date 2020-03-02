#pragma once
#include "pch.h"
#include "control_register.h"
#include "caret_struct.h"

#include "edit_control/dispatcher_system/dispatcher.h"
#include "edit_control/keyboard_system/keyboard.h"
#include "edit_control/mouse_system/mouse.h"
#include "edit_control/render_system/render.h"

#include "edit_control/scroll_bar_system/scroll_bar.h"
#include "edit_control/line_number_system/line_number.h"

#include "edit_control/intellisense_system/intelisense.h"

class Edit_Control {
    struct Line_Len {
        size_t Line_Lenght = 0;
        size_t Line_Pos = 0;
    };

public:
    void Init(HWND Parent);
    void Size(RECT& Rect);
    void Minimized();

    void Set_Text(std::wstring New_Text);
    void Set_Text(std::vector<std::wstring> New_Text);


    //Very dangerous function - allows to break rendering
    std::vector<std::wstring>& UNSAFE_Get_Txt();

    //Very dangerous function - allows to break rendering
    std::vector<std::vector<COLORREF>>& UNSAFE_Get_Clr();


    ~Edit_Control();

private:
    HWND m_hWnd = NULL;
    HWND m_Parent = NULL;
    bool m_Focus = false;
    bool m_Minimised = false;
    RECT m_Rect = {};
    COLORREF m_Default_Text_Color = RGB(170, 170, 170);

    std::vector<std::wstring> m_Text = std::vector<std::wstring>(1);
    std::vector<std::vector<COLORREF>> m_TextColor = std::vector<std::vector<COLORREF>>(1);

    Line_Len m_MaxLine_Length;

    std::vector<std::wstring> m_LineNumbers;
    unsigned char m_LineNum_Length = 5;
    COLORREF m_Line_Number_Clr = m_Default_Text_Color;

    Caret_Struct m_Caret, m_CaretSel;
    size_t m_Caret_Virtual_X = 0;
    bool m_CaretVisible = false;

    int m_Screen_MaxX = 0;
    int m_Screen_MaxY = 0;
    int m_Screen_XOff = 0;
    int m_Screen_YOff = 0;

    LONG m_Char_Width = 0;
    LONG m_Char_Height = 0;
    LONG m_Caret_Width = 0;
    LONG m_Caret_Height = 0;

    HFONT m_Font = NULL;
    int m_Font_Size = 50;
    HBRUSH m_Bkg_Brush = NULL;

public:
    Keyboard    m_Keyboard = Keyboard(*this);
    Mouse       m_Mouse = Mouse(*this);
    Render      m_Render = Render(*this);
    Scroll_Bar  m_Vert_Scrollbar = Scroll_Bar(*this);
    Scroll_Bar  m_Horz_Scrollbar = Scroll_Bar(*this);
    IntelliSense m_IntelliSense = IntelliSense(*this);
    Dispatcher  m_Dispatcher = Dispatcher(*this);
    Line_Number m_Line_Number = Line_Number(*this);
private:

    size_t m_InitCalls = 0;
    bool m_Is_Initialized = false;

    void Corect_Scrolling();
    void Set_Lines(size_t New_Number);
    void Calculate_Text_Metrics();
    
    void Differentiate_Start_End(Caret_Struct* Start, Caret_Struct* End, Caret_Struct& a, Caret_Struct& b);
    COLORREF Get_Last_Color(const Caret_Struct& Caret);

    std::wstring Convert_To_LineNum(size_t Line, unsigned char Num_Size);

    friend class Dispatcher;
    friend class Keyboard;
    friend class Mouse;
    friend class Render;
    friend class Scroll_Bar;
    friend class Line_Number;
    friend class IntelliSense;
};

