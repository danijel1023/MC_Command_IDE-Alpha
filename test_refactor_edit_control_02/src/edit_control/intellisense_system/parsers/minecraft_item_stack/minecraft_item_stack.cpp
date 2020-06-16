#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Item_Stack(std::wstring Word) {
    size_t Word_Size = Word.size();
    for (size_t i = 0; i < Word_Size; i++) {
        wchar_t ch = Word.at(i);
        if (ch == L'{') {
            std::wstring Error;
            if (!SNbt_Formater::Is_Valid_nc(&Word.substr(i), &Error)) {
                Error_Handler << Error.c_str();
                return false;
            }

            Word.erase(i);
        }
    }

    std::wstring Error;
    if (!Std::Insert_Namespace(Word, &Error)) {
        Error_Handler << Error.c_str();
        return false;
    }
    return Generic_ArrElm_Search(m_Minecraft_Item_Stack_Obj, Word);
}
