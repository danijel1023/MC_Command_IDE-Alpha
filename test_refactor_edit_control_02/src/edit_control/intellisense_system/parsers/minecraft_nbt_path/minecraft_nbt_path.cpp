#include "pch.h"
#include "intelisense.h"

static bool Extract_Elements(std::wstring& Word, std::vector<std::wstring>* Elements, std::wstring* Error);
static bool Check_Element(std::wstring& Word, std::wstring* Error, bool Is_Obj = false);

bool IntelliSense::Minecraft_Nbt_Path(std::wstring& Word) {
    std::vector<std::wstring> Elements;
    std::wstring Error;
    if (!Extract_Elements(Word, &Elements, &Error)) {
        Error_Handler << Error.c_str();
        return false;
    }

    size_t Elements_Size = Elements.size();
    if (Elements_Size == 0) {
        Error_Handler << L"Invalid NBT path";
        return false;
    }

    bool Allow_Root_Obj = true;
    for (size_t i = 0; i < Elements_Size; i++) {
        std::wstring Element = Elements.at(i);

        size_t Element_Size = Element.size();
        if (Element_Size == 0) {
            Error_Handler << L"Unexpected '.'";
            return false;
        }

        if (!Check_Element(Element, &Error, Allow_Root_Obj)) {
            Error_Handler << Error.c_str();
            return false;
        }

        Allow_Root_Obj = false;
    }

    return true;
}


bool Check_Element(std::wstring& Word, std::wstring* Error, bool Is_Obj) {
    size_t Obj_Ancor = 0;
    bool Is_UnQStr = false, Is_QStr = false, Is_Str = false, Is_Arr = false, Is_ArrNum = false;
    bool Skip = false;

    wchar_t ch = Word.at(0);
    if (ch == L'{' && !Is_Obj) {
        *Error = L"Unexpected '{'";
        return false;
    }

    else if (ch == L'[') {
        Is_Arr = true;
    }

    else if (ch == L'"') {
        Is_Str = true;
        Is_QStr = true;
    }

    else {
        Is_Str = true;
        Is_UnQStr = true;
    }


    size_t Word_Size = Word.size();
    for (size_t i = 0; i < Word_Size; i++) {
        if (Skip) continue;
        wchar_t ch = Word.at(i);

        if (Is_Str) {
            if (Is_QStr) {
                if (ch == L'\\') Skip = true;
                else if (ch == L'"') Is_QStr = false;

                if (i + 1 != Word_Size) {
                    *Error = L"Expected '.'";
                    return false;
                }
            }

            else {
                if (ch == L'[' || ch == L']' || ch == L'"') {
                    *Error = L"Ilegal char in unqouted string";
                    return false;
                }

                if (ch == L'{') {
                    Obj_Ancor = i;
                    Is_Obj = true;
                    Is_Str = false;
                }
            }
        }

        else if (Is_Arr) {
            if (ch == L'{') {
                if (Is_ArrNum) {
                    *Error = L"Invalid NBT path";
                    return false;
                }

                if (!SNbt_Formater::Is_Valid(Word.substr(1, Word_Size - 1), Error)) {
                    return false;
                }

                if (Word.back() != L']') {
                    *Error = L"Invalid NBT path";
                    return false;
                }

                Is_Arr = false;
            }
            
            else if (Std::Is_Number(ch)) {
                Is_ArrNum = true;
            }

            else if (ch == L']') {
                Is_Arr = false;
            }
        }

        else if (Is_Obj) {
            if (!SNbt_Formater::Is_Valid(Word.substr(Obj_Ancor), Error)) {
                return false;
            }

            Is_Obj = false;
        }

        else {
            *Error = L"Invalid NBT path";
            return false;
        }
    }

    if (Is_Obj) {
        *Error = L"Invalid NBT path";
        return false;
    }

    if (Is_Arr) {
        *Error = L"Invalid NBT path";
        return false;
    }

    if (Is_QStr) {
        *Error = L"Missing closing \"";
        return false;
    }

    return true;
}


static bool Extract_Elements(std::wstring& Word, std::vector<std::wstring>* Elements, std::wstring* Error) {
    std::wstring Brackets;
    bool Is_Str = false, Single_Q = false, Skip = false;
    size_t Ancor = 0;
    size_t Word_Size = Word.size();
    for (size_t i = 0; i < Word_Size; i++) {
        if (Skip) { Skip = false; continue; }
        wchar_t ch = Word.at(i);

        if (Is_Str) {
            if (ch == L'\\') { Skip = true; } else if ((!Single_Q && ch == L'"') || (Single_Q && ch == L'\'')) { Is_Str = false; }
        }

        else if (ch == L'"' || ch == L'\'') {
            Single_Q = (ch == L'\'');
            Is_Str = true;
        }

        else if (ch == L'[' || ch == L'{') {
            Brackets.push_back(ch);
        }

        else if (ch == L']' || ch == L'}') {
            if (ch == L']' && Brackets.back() == L'[' ||
                ch == L'}' && Brackets.back() == L'{') {
                Brackets.pop_back();
            } else {
                *Error =  L"Brackets don't match up";
                return false;
            }
        }

        else if (Brackets.size() == 0) {
            if (ch == L'.') {
                (*Elements).push_back(Word.substr(Ancor, i - Ancor));
                Ancor = i + 1;
            }

            else if (Word_Size == i + 1) {
                (*Elements).push_back(Word.substr(Ancor, (i + 1) - Ancor));
            }
        }
    }

    if (Brackets.size() != 0) {
        *Error = L"Brackets don't match up";
        return false;
    }

    if (Is_Str) {
        *Error = L"Missing closing quote";
        return false;
    }

    return true;
}
