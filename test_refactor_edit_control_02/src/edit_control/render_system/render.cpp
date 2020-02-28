#include "pch.h"
#include "edit_control/_edit_control/edit_control.h"
//#include "render.h" --> It's already included in edit_control.h

Render::Render(Edit_Control& EC)
    :m_EC(EC)
{}

Render::~Render() {
    CHECK_ERR(DeleteObject(m_EC.m_Bkg_Brush), ERR_MSG_DELETE_OBJECT);
    CHECK_ERR(DeleteObject(m_Selection_Brush), ERR_MSG_DELETE_OBJECT);
    CHECK_ERR(DeleteObject(m_EC.m_Font), ERR_MSG_DELETE_OBJECT);
    CHECK_ERR(DeleteObject(m_Main_Screen_Bitmap), ERR_MSG_DELETE_OBJECT);
}


void Render::Init(HWND hWnd) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_RENDER);
    Log_IO::wcout() << "Init" << std::endl;
    m_hWnd = hWnd;

    m_EC.m_Font = CreateFont(m_EC.m_Font_Size, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Consolas");
    CHECK_ERR(m_EC.m_Font, ERR_MSG_CREATE_FONT);
    m_EC.Calculate_Text_Metrics();

    m_EC.m_Bkg_Brush = CreateSolidBrush(RGB(45, 45, 50));
    CHECK_ERR(m_EC.m_Bkg_Brush, ERR_MSG_CREATE_SOLID_BRUSH);
    m_Selection_Brush = CreateSolidBrush(RGB(45, 94, 252));
    CHECK_ERR(m_Selection_Brush, ERR_MSG_CREATE_SOLID_BRUSH);


    wchar_t* File_Location = new wchar_t[_MAX_PATH];
    GetModuleFileNameW(NULL, File_Location, _MAX_PATH);
    std::wstring Bkg_Location = File_Location;
    delete[] File_Location;

    size_t Loc = Bkg_Location.find_last_of(L"\\");
    Bkg_Location.erase(Bkg_Location.begin() + Loc, Bkg_Location.end());
    Bkg_Location += L"\\bkg_image.bmp";

    m_Raw_Bkg_Image_Bitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), Bkg_Location.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}


void Render::Proc_Msg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    m_EC.m_Dispatcher.Return = false;

    switch (uMsg) {
    case WM_SETFOCUS:
        WM_Set_Focus(wParam, lParam);
        if (m_EC.m_Dispatcher.Return) return;
        break;
    case WM_KILLFOCUS:
        WM_Kill_Focus(wParam, lParam);
        if (m_EC.m_Dispatcher.Return) return;
        break;
    case WM_ERASEBKGND:
        WM_Erase_Bkg(wParam, lParam);
        if (m_EC.m_Dispatcher.Return) return;
        break;
    case WM_PAINT:
        WM_Paint(wParam, lParam);
        if (m_EC.m_Dispatcher.Return) return;
        break;
    }
}


void Render::Set_Size(RECT Rect) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_RENDER);
    Log_IO::wcout() << "Set_Size" << std::endl;

    Rect.left += ((m_EC.m_LineNum_Length + 1) * m_EC.m_Char_Width) + (m_EC.m_Caret_Width * 2);
    Rect.right -= ((m_EC.m_LineNum_Length + 1) * m_EC.m_Char_Width) + (m_EC.m_Caret_Width * 2);

    RECT Vert_Rect = Rect, Horz_Rect = Rect;
    Vert_Rect.left = Rect.right - Vert_Width;
    Vert_Rect.right = Vert_Width;
    Horz_Rect.left = 0;
    Horz_Rect.right -= Vert_Width;
    Horz_Rect.top = Rect.bottom - Horz_Width;
    Horz_Rect.bottom = Horz_Width;
    m_EC.m_Vert_Scrollbar.Set_Size(Vert_Rect);
    m_EC.m_Horz_Scrollbar.Set_Size(Horz_Rect);

    CHECK_ERR(SetWindowPos(m_hWnd, NULL, Rect.left, Rect.top, Rect.right, Rect.bottom, SWP_NOZORDER), ERR_MSG_SET_WINDOW_POS);
    Rect.right -= Vert_Width;
    Rect.bottom -= Horz_Width;
    m_EC.m_Screen_MaxX = (Rect.right) / m_EC.m_Char_Width;
    m_EC.m_Screen_MaxY = (Rect.bottom) / m_EC.m_Char_Height;

    {
        HDC hDC = GetDC(m_hWnd); CHECK_ERR(hDC, ERR_MSG_GET_DC);
        DeleteObject(m_Main_Screen_Bitmap);
        DeleteObject(m_Bkg_Image_Bitmap);
        m_Main_Screen_Bitmap = CreateCompatibleBitmap(hDC, Rect.right, Rect.bottom);
        m_Bkg_Image_Bitmap = CreateCompatibleBitmap(hDC, Rect.right, Rect.bottom);


        HDC Bkg_BMP_hDC = CreateCompatibleDC(hDC);
        HDC Raw_Bkg_BMP_hDC = CreateCompatibleDC(hDC);
        auto Bkg_BMP_Old = SelectObject(Bkg_BMP_hDC, m_Bkg_Image_Bitmap);
        auto Raw_Bkg_BMP_Old = SelectObject(Raw_Bkg_BMP_hDC, m_Raw_Bkg_Image_Bitmap);

        BITMAP Raw_Bkg_BMP_Size;
        GetObject(m_Raw_Bkg_Image_Bitmap, sizeof(Raw_Bkg_BMP_Size), &Raw_Bkg_BMP_Size);

        SetStretchBltMode(Bkg_BMP_hDC, HALFTONE);
        CHECK_ERR(StretchBlt(Bkg_BMP_hDC, 0, 0, Rect.right, Rect.bottom,
                             Raw_Bkg_BMP_hDC, 0, 0, Raw_Bkg_BMP_Size.bmWidth, Raw_Bkg_BMP_Size.bmHeight,
                             SRCCOPY),
                  ERR_MSG_STRECH_BLT);

        SelectObject(Raw_Bkg_BMP_hDC, Raw_Bkg_BMP_Old);
        SelectObject(Bkg_BMP_hDC, Bkg_BMP_Old);
        CHECK_ERR(DeleteDC(Raw_Bkg_BMP_hDC), ERR_MSG_DELETE_DC);
        CHECK_ERR(DeleteDC(Bkg_BMP_hDC), ERR_MSG_DELETE_DC);

        CHECK_ERR(ReleaseDC(m_hWnd, hDC), ERR_MSG_RELEASE_DC);
    }

    Rect.left = 0;
    Rect.right = ((m_EC.m_LineNum_Length + 1) * m_EC.m_Char_Width) + (m_EC.m_Caret_Width * 2);
    m_EC.m_Line_Number.Set_Size(Rect);

    CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
}


void Render::WM_Set_Focus(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_RENDER);
    Log_IO::wcout() << "WM_Set_Focus" << std::endl;

    CHECK_ERR(CreateCaret(m_hWnd, NULL, m_EC.m_Caret_Width, m_EC.m_Caret_Height), ERR_MSG_CREATE_CARET);
    CHECK_ERR(ShowCaret(m_hWnd), ERR_MSG_SHOW_CARET);
    m_EC.m_CaretVisible = true;
    Set_Caret(m_EC.m_Caret);

    m_EC.m_Focus = true;
    m_EC.m_Dispatcher.Return_Val = 0;
    m_EC.m_Dispatcher.Return = false;
}


void Render::WM_Kill_Focus(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_RENDER);
    Log_IO::wcout() << "WM_Kill_Focus" << std::endl;

    if (m_EC.m_CaretVisible)
        CHECK_ERR(HideCaret(m_hWnd), ERR_MSG_HIDE_CARET);
    CHECK_ERR(DestroyCaret(), ERR_MSG_DESTROY_CARET);
    m_EC.m_CaretVisible = false;

    m_EC.m_Focus = false;
    m_EC.m_Dispatcher.Return_Val = 0;
    m_EC.m_Dispatcher.Return = false;
}


void Render::WM_Erase_Bkg(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_RENDER);
    Log_IO::wcout() << "WM_Erase_Bkg" << std::endl;
    RECT Rect;
    CHECK_ERR(GetClientRect(m_hWnd, &Rect), ERR_MSG_GET_CLIENT_RECT);
    Rect.right -= Vert_Width;
    Rect.bottom -= Horz_Width;

    BITMAP Bitmap_Size;
    GetObject(m_Main_Screen_Bitmap, sizeof(Bitmap_Size), &Bitmap_Size);

    HDC Main_Screen_BMP_hDC = CreateCompatibleDC((HDC)wParam);
    HDC Bkg_Image_BMP_hDC = CreateCompatibleDC((HDC)wParam);
    auto Main_Screen_BMP_Old = SelectObject(Main_Screen_BMP_hDC, m_Main_Screen_Bitmap);
    auto Bkg_Image_BMP_Old = SelectObject(Bkg_Image_BMP_hDC, m_Bkg_Image_Bitmap);

    BitBlt(Main_Screen_BMP_hDC, 0, 0, Bitmap_Size.bmWidth, Bitmap_Size.bmHeight, Bkg_Image_BMP_hDC, 0, 0, SRCCOPY);

    SelectObject(Bkg_Image_BMP_hDC, Bkg_Image_BMP_Old);
    SelectObject(Main_Screen_BMP_hDC, Main_Screen_BMP_Old);
    DeleteDC(Bkg_Image_BMP_hDC);
    DeleteDC(Main_Screen_BMP_hDC);

    m_EC.m_Dispatcher.Return_Val = 1;
    m_EC.m_Dispatcher.Return = true;
}


void Render::WM_Paint(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_RENDER);
    Log_IO::wcout() << "WM_Paint" << std::endl;

    PAINTSTRUCT PS;
    HDC hDC = BeginPaint(m_hWnd, &PS);
    CHECK_ERR(hDC, ERR_MSG_BEGIN_PAINT);

    if (!m_EC.m_Minimised) {
        BITMAP Bitmap_Size;
        GetObject(m_Main_Screen_Bitmap, sizeof(Bitmap_Size), &Bitmap_Size);

        HDC Bitmap_hDC = CreateCompatibleDC(hDC);
        auto Bitmap_Old = SelectObject(Bitmap_hDC, m_Main_Screen_Bitmap);

        Draw_Control(Bitmap_hDC);
        BitBlt(hDC, 0, 0, Bitmap_Size.bmWidth, Bitmap_Size.bmHeight, Bitmap_hDC, 0, 0, SRCCOPY);

        SelectObject(Bitmap_hDC, Bitmap_Old);
        m_EC.m_Line_Number.Paint(m_EC.m_Font);
    }

    EndPaint(m_hWnd, &PS);

    m_EC.m_Dispatcher.Return_Val = 0;
    m_EC.m_Dispatcher.Return = true;
}


void Render::Draw_Control(HDC hDC) {
    bool New_DC = false;
    if (hDC == NULL) {
        New_DC = true;
        hDC = GetDC(m_hWnd);
        CHECK_ERR(hDC, ERR_MSG_GET_DC);
    }

    HFONT FontOld = (HFONT)SelectObject(hDC, m_EC.m_Font);
    CHECK_ERR(FontOld, ERR_MSG_SELECT_OBJECT_HFONT);
    auto Bkg_ModeOld = SetBkMode(hDC, TRANSPARENT);
    CHECK_ERR(Bkg_ModeOld, ERR_MSG_SET_BK_MODE);

    Render_Selected_Area(hDC);

    COLORREF Clr_Old = 0;
    size_t Text_YSize = m_EC.m_Text.size();
    for (size_t Y_Pos = m_EC.m_Screen_YOff; Y_Pos < Text_YSize; Y_Pos++) {

        size_t Text_XSize = m_EC.m_Text.at(Y_Pos).size();
        auto& Str = m_EC.m_Text.at(Y_Pos);
        auto& Clr = m_EC.m_TextColor.at(Y_Pos);

        for (size_t X_Pos = 0; X_Pos < Text_XSize; X_Pos++) {
            if (X_Pos < m_EC.m_Screen_XOff) continue;
            if (X_Pos > (size_t)m_EC.m_Screen_XOff + (size_t)m_EC.m_Screen_MaxX) break;

            if (Clr.at(X_Pos) != Clr_Old) {
                Clr_Old = Clr.at(X_Pos);
                if (SetTextColor(hDC, Clr_Old) == CLR_INVALID) {
                    MB_ERR("'SetTextColor' Failed!");
                    PostQuitMessage(-1);
                }
            }

            int X = (int)(X_Pos - m_EC.m_Screen_XOff) * m_EC.m_Char_Width;
            int Y = (int)(Y_Pos - m_EC.m_Screen_YOff) * m_EC.m_Char_Height;
            CHECK_ERR(TextOut(hDC, X, Y, &Str.at(X_Pos), 1), ERR_MSG_TEXT_OUT);
        }

        if (Y_Pos > (size_t)m_EC.m_Screen_YOff + (size_t)m_EC.m_Screen_MaxY) break;
    }

    Set_Caret(m_EC.m_Caret);

    CHECK_ERR(SetBkMode(hDC, Bkg_ModeOld), ERR_MSG_SET_BK_MODE);
    CHECK_ERR(SelectObject(hDC, FontOld), ERR_MSG_SELECT_OBJECT_HFONT);

    if (New_DC) CHECK_ERR(ReleaseDC(m_hWnd, hDC), ERR_MSG_RELEASE_DC);
}


void Render::Render_Selected_Area(HDC hDC) {
    if (m_EC.m_Caret == m_EC.m_CaretSel) return;
    Caret_Struct SelStart, SelEnd;
    m_EC.Differentiate_Start_End(&SelStart, &SelEnd, m_EC.m_Caret, m_EC.m_CaretSel);

    RECT Rect;
    size_t SelXStart, SelXEnd;
    size_t Y_Start_Pos = (m_EC.m_Screen_YOff > SelStart.Y) ? m_EC.m_Screen_YOff : SelStart.Y;
    for (size_t Y_Pos = Y_Start_Pos; Y_Pos <= SelEnd.Y; Y_Pos++) {
        SelXStart = (Y_Pos == SelStart.Y) ? SelStart.X : 0;
        SelXStart = (SelXStart < m_EC.m_Screen_XOff) ? 0 : (SelXStart - m_EC.m_Screen_XOff);

        SelXEnd = (Y_Pos == SelEnd.Y) ? SelEnd.X : m_EC.m_Text.at(Y_Pos).size();
        SelXEnd = (SelXEnd > ((size_t)m_EC.m_Screen_MaxX + (size_t)m_EC.m_Screen_XOff)) ?
            ((size_t)m_EC.m_Screen_MaxX + (size_t)m_EC.m_Screen_XOff) : (SelXEnd > m_EC.m_Screen_XOff) ?
            (SelXEnd - m_EC.m_Screen_XOff) : 0;


        Rect.left = (LONG)(SelXStart * m_EC.m_Char_Width);
        Rect.right = (LONG)(SelXEnd) * (LONG)m_EC.m_Char_Width;
        Rect.top = (int)(Y_Pos - m_EC.m_Screen_YOff) * (LONG)m_EC.m_Char_Height;
        Rect.bottom = ((int)(Y_Pos - m_EC.m_Screen_YOff) + 1) * (LONG)m_EC.m_Char_Height;
        CHECK_ERR(FillRect(hDC, &Rect, m_Selection_Brush), ERR_MSG_FILL_RECT);

        if (Y_Pos > (size_t)m_EC.m_Screen_YOff + (size_t)m_EC.m_Screen_MaxY) break;
    }
}


void Render::Set_Caret(Caret_Struct& Caret) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_RENDER);
    Log_IO::wcout() << "Set_Caret" << std::endl;

    if (Caret.Y < m_EC.m_Screen_YOff) {
        if (m_EC.m_CaretVisible)
            CHECK_ERR(HideCaret(m_hWnd), ERR_MSG_HIDE_CARET);
        m_EC.m_CaretVisible = false;
        return;
    }

    if (Caret.X < m_EC.m_Screen_XOff) {
        if (m_EC.m_CaretVisible)
            CHECK_ERR(HideCaret(m_hWnd), ERR_MSG_HIDE_CARET);
        Log_IO::wcout() << "Set_Caret - HideCaret" << std::endl;
        m_EC.m_CaretVisible = false;
        return;
    }

    if (m_EC.m_Focus) {
        if (!m_EC.m_CaretVisible) {
            CHECK_ERR(ShowCaret(m_hWnd), ERR_MSG_SHOW_CARET);
            Log_IO::wcout() << "Set_Caret - ShowCaret" << std::endl;
            m_EC.m_CaretVisible = true;
        }

        Log_IO::wcout() << "Set_Caret - SetCaretPos" << std::endl;
        CHECK_ERR(SetCaretPos((int)(Caret.X - m_EC.m_Screen_XOff) * m_EC.m_Char_Width, (int)(Caret.Y - m_EC.m_Screen_YOff) * m_EC.m_Char_Height),
                  ERR_MSG_SET_CARET_POS);
    }
}
