#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Block_Predicate(std::wstring Word) {
    std::wstring Params;
    size_t Word_Size = Word.size();
    for (size_t i = 0; i < Word_Size; i++) {
        wchar_t ch = Word.at(i);

        if (ch == L'[' || ch == L'{') {
            Params = Word.substr(i);
            Word.erase(i);
            break;
        }
    }

    std::wstring Error;
    if (!Std::Insert_Namespace(Word, &Error)) {
        Error_Handler << Error.c_str();
        return false;
    }

    if (!Generic_ArrElm_Search(m_Minecraft_Block_Predicate_Obj, Word)) {
        Error_Handler << std::wstring(L"Cannot find match for (" + Word + L") in blocks and blocks tags").c_str();
        return false;
    }


    std::wstring Prop_Params;
    std::wstring SNbt_Params;

    size_t Ancor = 0;
    size_t Params_Size = Params.size();
    if (Params_Size != 0) {
        bool Has_Properties = (Params.at(0) == L'[');

        if (Has_Properties) {
            for (size_t i = 0; i < Params_Size; i++) {
                wchar_t ch = Params.at(i);

                if (ch == L']') {
                    Ancor = i;
                    break;
                }
            }

            Prop_Params = Params.substr(0, Ancor);
            
            //Check properties params
        }

        SNbt_Params = Params.substr(Ancor);

        std::wstring Error;
        if (!SNbt_Formater::Is_Valid_nc(&SNbt_Params, &Error)) {
            Error_Handler << Error.c_str();
            return false;
        }
    }

    return true;
}
