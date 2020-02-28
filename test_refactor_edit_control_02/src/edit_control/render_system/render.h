#pragma once
#include "pch.h"

class Edit_Control;
class Render {
public:
    void Init(HWND hWnd);
    void Proc_Msg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    Render() = delete;
    Render(Edit_Control& EC);
    ~Render();

private:
    Edit_Control& m_EC;
    HWND m_hWnd = NULL;
    LONG Vert_Width = 50, Horz_Width = 20;

    HBITMAP m_Main_Screen_Bitmap = NULL;
    HBITMAP m_Bkg_Image_Bitmap = NULL;
    HBITMAP m_Raw_Bkg_Image_Bitmap = NULL;
    HBRUSH m_Selection_Brush = NULL;

    void WM_Paint(WPARAM wParam, LPARAM lParam);
    void WM_Set_Focus(WPARAM wParam, LPARAM lParam);
    void WM_Kill_Focus(WPARAM wParam, LPARAM lParam);
    void WM_Erase_Bkg(WPARAM wParam, LPARAM lParam);

    void Draw_Control(HDC hDC);
    void Render_Selected_Area(HDC hDC);

    void Set_Caret(Caret_Struct& Caret_Pos);
    void Set_Size(RECT Rect);

    friend class Edit_Control;
    friend class Line_Number;
};

