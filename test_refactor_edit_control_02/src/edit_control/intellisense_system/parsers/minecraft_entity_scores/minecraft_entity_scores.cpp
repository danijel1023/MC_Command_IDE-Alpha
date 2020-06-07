#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Entity_Scores(std::wstring& Word) {
    std::vector<std::wstring> Params;
    std::wstring Error;
    if (!State_Formater::Seperate_State(Word, &Params, &Error, L'{', L'}')) {
        Error_Handler << Error.c_str();
        return false;
    }


    size_t Params_Size = Params.size();
    if (Params_Size % 2 == 1) {
        Error_Handler << L"Unknown Error; Params_Size is uneven - Problem with 'Seperate_State'";
        return false;
    }

    for (size_t i = 0; i < Params_Size; i++) {
        if (i % 2 == 0) {
            if (!Brigadier_String(Params.at(i), L"word")) {
                return false;
            }
        }

        else {
            if (!Minecraft_Int_Range(Params.at(i), false)) {
                return false;
            }
        }
    }

    return true;
}
