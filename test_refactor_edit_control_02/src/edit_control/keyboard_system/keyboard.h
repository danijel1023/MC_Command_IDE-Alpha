#pragma once
#include "pch.h"
#include "edit_control/_edit_control/caret_struct.h"

class Edit_Control;
struct History_Struct {
    unsigned char Mode = 0;

    Caret_Struct Caret, CaretSel;

    std::vector<std::wstring> Text;
    std::vector<std::wstring> Del_Sel_Insert_Text;
};

class Keyboard {
public:
    void Init(HWND hWnd);

    //Interface for dispatcher
    void Proc_Msg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    Keyboard() = delete;
    Keyboard(Edit_Control& EC);
    ~Keyboard();

private:
    //Referance of parent class ('Edit_Control')
    Edit_Control& m_EC;
    HWND m_hWnd = NULL;

    std::vector<History_Struct> m_History;
    size_t m_History_Pointer = 0;

    //-- Functions for dispatcher ('Proc_Msg') ---------
    /**/ void WM_SysKeyDown(WPARAM wParam, LPARAM lParam);
    /**/ void WM_KeyDown(WPARAM wParam, LPARAM lParam);
    /**/ void WM_Char(WPARAM wParam, LPARAM lParam);
    //--------------------------------------------------

    //Func for copy operation
    void Copy();
    //Func for cut operation
    void Cut();
    //Func for paste operation
    void Paste();

    //Func for undo operation
    void UnDo();
    //Func for redo operation
    void ReDo();

    //Call this function to record history
    void Register_History(unsigned char Mode, Caret_Struct& Caret, Caret_Struct& CaretSel,
                          const std::vector<std::wstring>& Text,
                          const std::vector<std::wstring>& Adtitonal = {});

    //In 'Dest' will copy selected text
    void Extract_Selected(const std::vector<std::wstring>& Text,
                          Caret_Struct& Caret, Caret_Struct& CaretSel,
                          std::vector<std::wstring>* Dest);

    //Delete text between 2 points ('Caret' & 'CaretSel') - if the text is deleted, returns true
    bool Delete_Selected_Unordered(Caret_Struct& Caret, Caret_Struct& CaretSel, bool Record_History = true);
    //Delete text from 'Sel_Start' to 'Sel_End' - if the text is deleted, returns true
    bool Delete_Selected(Caret_Struct& Sel_Start, Caret_Struct& Sel_End, bool Record_History = true);

    //Will generate text color vector for given text (it just needs its length)
    //Danger: 'm_EC.m_Caret' needs to point to existing text ('m_EC.m_Text')
    std::vector<COLORREF> Generate_Clr(const std::wstring& Text, const Caret_Struct& Caret);
    
    //Will insert text on position of 'Caret' - automatically moves caret accordingly
    void Insert(const std::vector<std::wstring>& Text, Caret_Struct Caret); //'Caret' is not reference because it changes its value in code

    //Will calculate max line length and update horizontal scrollbar
    void Calculate_Max_Line_Length();


    //Used for clipboard - transfer 'std::wstring' to global memory
    HGLOBAL Convert_Str_To_Global_Object(const std::wstring& String_Copy);
    //Used for clipboard - transfer global memory to 'std::wstring'
    void Convert_Global_Object_To_Str(HGLOBAL hMem, std::wstring* Converted_Str);

    //Used for clipboard - when pasting to clipboard (formats new lines with L"\r\n")
    std::wstring Format_To_External(const std::vector<std::wstring>& Formated_Data);
    //Used for clipboard - when pasting into text editor (in 'UnFormated_Data' new line *MUST* be formated with L"\r\n")
    std::vector<std::wstring> Format_To_Internal(const std::wstring& UnFormated_Data);

    
    friend class Edit_Control;
};


