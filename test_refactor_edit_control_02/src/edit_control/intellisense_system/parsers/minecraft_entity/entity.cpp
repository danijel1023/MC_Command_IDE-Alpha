#include "pch.h"
#include "intelisense.h"
#include "entity_struct.h"


bool IntelliSense::Minecraft_Entity(std::wstring& Word) {
    if (Word.size() < 2) {
        Error_Handler << L"Entity has to have at leas 2 characters";
        return false;
    }

    bool Needs_Players = false, Needs_Single = false;
    bool Players = false, Single = false;

    if (m_Syntax_Obj.Has_Name(L"properties")) {
        m_Syntax_Obj.Obj(L"properties");

        if (m_Syntax_Obj.Has_Name(L"type")) {
            m_Syntax_Obj.Obj(L"type");
            std::wstring& Type = m_Syntax_Obj.Str();

            if (Type == L"players") {
                Needs_Players = true;
            } else if (Type == L"entities") {
                Needs_Players = false;
            } else {
                Error_Handler << L"Expected value 'players' or 'entities', got: " << Type.c_str();
            }

            m_Syntax_Obj.Back();
        }

        if (m_Syntax_Obj.Has_Name(L"amount")) {
            m_Syntax_Obj.Obj(L"amount");
            std::wstring& Amount = m_Syntax_Obj.Str();

            if (Amount == L"single") {
                Needs_Single = true;
            } else if (Amount == L"multiple") {
                Needs_Single = false;
            } else {
                Error_Handler << L"Expected value 'single' or 'multiple', got: " << Amount.c_str();
            }

            m_Syntax_Obj.Back();
        }
        m_Syntax_Obj.Back();

        if (Word.at(0) != L'@') {
            size_t Word_Size = Word.size();
            if (Word_Size < 3 || Word_Size > 16) {
                Error_Handler << L"Entity name must be from 3 to 16 characters long";
                return false;
            }

            for (size_t i = 0; i < Word_Size; i++) {
                wchar_t ch = Word.at(i);
                if (!(Std::Is_Lowercase(ch) || Std::Is_Number(ch) || Std::Is_Uppercase(ch) || ch == L'_')) {
                    Error_Handler << L"Entity name contains ilegal characters";
                    return false;
                }
            }

            //Doesn't require players/single testing bc players name is 'type:player' and 'amount:single'

            return true;
        }

        else {
            std::vector<std::wstring> Used_Params;
            switch (Word.at(1)) {
            case L'a':
                Used_Params.push_back(L"type");
                Players = true;
                Single = false;
                break;

            case L'e':
                Players = false;
                Single = false;
                break;

            case L'p':
                Used_Params.push_back(L"type");
                Players = true;
                Single = true;
                break;

            case L'r':
                Used_Params.push_back(L"type");
                Players = true;
                Single = true;
                break;

            case L's':
                Players = false;
                Single = true;
                break;

            default:
                Error_Handler << L"Invalid selector";
                return false;
            }

            if (Word.size() > 2) {
                Word.erase(0, 2);

                std::wstring Error;
                std::vector<std::wstring> Params;
                if (!State_Formater::Seperate_State(Word, &Params, &Error, L'[', L']')) {
                    Error_Handler << Error.c_str();
                    return false;
                }

                std::vector<std::wstring> Used_Invert_Params;
                size_t Params_Size = Params.size();
                for (size_t i = 0; i < Params_Size; i++) {
                    std::wstring State = Params.at(i);
                    std::wstring Value = Params.at(i + 1);

                    Std::Clear_Spaces(&State);
                    Std::Clear_Spaces(&Value);

                    if (!m_Minecraft_Entity_Obj.Has_Name(State)) {
                        Error_Handler << std::wstring(L"Invalid option: " + State).c_str();
                        return false;
                    }

                    m_Minecraft_Entity_Obj.Obj(State);


                    bool Found_Invert = false;
                    size_t Value_Size = Value.size();
                    for (size_t i = 0; i < Value_Size; i++) {
                        wchar_t ch = Value.at(i);
                        if (ch != L' ') {
                            Value.erase(0, i);
                            if (ch == L'!') {
                                Found_Invert = true;
                                Value.erase(0, 1);
                            }

                            break;
                        }
                    }

                    if (Found_Invert && !m_Minecraft_Entity_Obj.Has_Name(L"can_invert")) {
                        Error_Handler << L"Didn't expect '!'";
                        return false;
                    }

                    if (m_Minecraft_Entity_Obj.Obj(L"amount").Str() == L"single") {
                        m_Minecraft_Entity_Obj.Back();

                        if (Found_Invert) {
                            if (std::find(Used_Invert_Params.begin(), Used_Invert_Params.end(), State) != Used_Invert_Params.end()) {
                                if (std::find(Used_Params.begin(), Used_Params.end(), State) != Used_Params.end()) {
                                    Error_Handler << std::wstring(L"Cannot mix 'inverted' match with 'non-inverted' for " + State).c_str();
                                    return false;
                                }
                                Used_Invert_Params.push_back(State);
                            }
                        }
                        
                        else {
                            if (std::find(Used_Params.begin(), Used_Params.end(), State) != Used_Params.end()) {
                                Error_Handler << std::wstring(L"Allowed only one: " + State).c_str();
                                return false;
                            }
                            Used_Params.push_back(State);
                        }
                    }


                    if (m_Minecraft_Entity_Obj.Has_Name(L"parser")) {
                        m_Minecraft_Entity_Obj.Obj(L"parser");
                        if (Parser_Matcher(Value, m_Minecraft_Entity_Obj) == L"ERROR") {
                            return false;
                        }

                        m_Minecraft_Entity_Obj.Back();
                    }
                    
                    else if (m_Minecraft_Entity_Obj.Has_Name(L"value")) {
                        bool Match_Found = false;
                        size_t Match_Value_Size = m_Minecraft_Entity_Obj.Obj(L"value").Size();
                        for (size_t i = 0; i < Match_Value_Size; i++) {
                            if (m_Minecraft_Entity_Obj.Arr(i).Str() == Value) {
                                m_Minecraft_Entity_Obj.Back();
                                Match_Found = true;
                            }
                            m_Minecraft_Entity_Obj.Back();
                        }
                        m_Minecraft_Entity_Obj.Back();

                        if (!Match_Found) {
                            Error_Handler << std::wstring(L"Invalid paramater: " + Value).c_str();
                            return false;
                        }
                    }
                    
                    else {
                        Error_Handler << L"FATAL INTERNAL ERROR: Could not locate 'parser' nor 'value'";
                        return false;
                    }


                    if (State == L"limit") {
                        if (Value == L"1") Single = true;
                        else Single = false;
                    } else if (State == L"type") {
                        if (Value == L"player" || Value == L"minecraft:player") Players = true;
                        else Players = false;
                    }
                }
            }

            if (Needs_Players && !Players) {
                Error_Handler << L"Needs only players";
                return false;
            }

            if (Needs_Single && !Single) {
                Error_Handler << L"Needs only one entity";
                return false;
            }
        }
    }

    return true;
}
