#include "pch.h"
#include "intelisense.h"

bool IntelliSense::Minecraft_Resource_Location(std::wstring Word) {
    static std::vector<std::pair<std::wstring, std::wstring>> IDValue;

    size_t Word_Size = Word.size();
    for (size_t i = 0; i < Word_Size; i++) {
        wchar_t ch = Word.at(i);
        if (!(Std::Is_Std_ch(ch) || ch == L':')) {
            Error_Handler << L"Ilegal char in namespace ID";
            return false;
        }
    }

    std::wstring Error;
    if (!Std::Insert_Namespace(Word, &Error)) {
        Error_Handler << Error.c_str();
        return false;
    }

    auto Old_Branch = m_Syntax_Obj.Get_Branch();
    auto ID_vec = m_Syntax_Obj.Back().Get_Names();
    m_Syntax_Obj.Set_Branch(Old_Branch);
    auto ID = ID_vec.at(0);

    bool Found_Match = false;
    size_t IDValue_Size = IDValue.size();
    for (size_t i = 0; i < IDValue_Size; i++) {
        if (IDValue.at(i).first == ID) {
            Found_Match = true;
            break;
        }
    }

    if (Found_Match) return true;

    IDValue.push_back(std::make_pair(ID, Word));
    return true;
}
