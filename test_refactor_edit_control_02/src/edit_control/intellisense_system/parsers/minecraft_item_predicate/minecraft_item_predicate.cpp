#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Item_Predicate(std::wstring Word) {
    bool Is_Tag = false;
    if (Word.at(0) == L'#') {
        Is_Tag = true;
        Word.erase(0, 1);
    }

    size_t New_Word_Size = Word.size();
    for (size_t i = 0; i < New_Word_Size; i++) {
        wchar_t ch = Word.at(i);
        if (ch == L'{') {
            std::wstring Error;
            if (!SNbt_Formater::Is_Valid_nc(&Word.substr(i), &Error)) {
                Error_Handler << Error.c_str();
                return false;
            }

            Word.erase(i);
            break;
        }
    }

    std::wstring Error;
    if (!Std::Insert_Namespace(Word, &Error)) {
        Error_Handler << Error.c_str();
        return false;
    }
    if (Is_Tag) Word.insert(0, 1, L'#');
    return Generic_ArrElm_Search(m_Minecraft_Item_Predicate_Obj, Word);
}
