#include "pch.h"
#include "intelisense.h"
#include "entity_struct.h"

#define END_ITER(i, max) (i + 1 == max)

bool Is_Syntax_Ch(wchar_t ch);
bool Is_Bracket(wchar_t ch);
bool Is_Open_Bracket(wchar_t ch);
bool Is_Closing_Bracket(wchar_t ch);
wchar_t Matching_Bracket(wchar_t ch);

static std::wstring AllowedChars =  L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";


bool IntelliSense::Entity_Paraser(std::wstring& Word) {
    if (Word.size() == 0) {
        Log_IO::wcout() << "Unknown Error - 'Check_Minecraft_Entity', 'Word.size() == 0'" << std::endl;
        return false;
    }

    
    MC_Entity MCE;
    if (m_Syntax_Obj.Has_Name(L"properties")) {
        m_Syntax_Obj.Obj(L"properties");

        if (m_Syntax_Obj.Has_Name(L"type")) {
            m_Syntax_Obj.Obj(L"type");
            std::wstring Type = m_Syntax_Obj.Str();

            if (Type == L"players") {
                MCE.Ent_Prop.Type = MCE_Prop::TYPE::Players;
            } else if (Type == L"entities") {
                MCE.Ent_Prop.Type = MCE_Prop::TYPE::Entities;
            } else {
                Error_Handler << L"Expected value 'players' or 'entities', got: " << Type.c_str();
            }

            m_Syntax_Obj.Back();
        }

        if (m_Syntax_Obj.Has_Name(L"amount")) {
            m_Syntax_Obj.Obj(L"amount");
            std::wstring Amount = m_Syntax_Obj.Str();

            if (Amount == L"single") {
                MCE.Ent_Prop.Amount = MCE_Prop::AMOUNT::Single;
            } else if (Amount == L"multiple") {
                MCE.Ent_Prop.Amount = MCE_Prop::AMOUNT::Multiple;
            } else {
                Error_Handler << L"Expected value 'single' or 'multiple', got: " << Amount.c_str();
            }

            m_Syntax_Obj.Back();
        }
        m_Syntax_Obj.Back();
        
        if (Word.at(0) != L'@') {
            if (Word.find_first_not_of(AllowedChars) != std::wstring::npos) {
                Error_Handler << L"Name containing ilegal char(s)";
                return false;
            }
            
            if (Word.size() > 16) {
                Error_Handler << L"Too long Name: " << std::to_wstring(Word.size()).c_str();
                return false;
            }
            
            return true;
        } else {
            if (Word.size() == 1) {
                Error_Handler << L"Name containing ilegal char(s)";
                return false;
            } else {
                switch (Word.at(1)) {
                case L'a':
                    MCE.Ent_Prop.Type = MCE_Prop::TYPE::Players;
                    MCE.Ent_Prop.Amount = MCE_Prop::AMOUNT::Multiple;
                    MCE.Sort = SORT::Unset;
                    break;

                case L'e':
                    MCE.Ent_Prop.Type = MCE_Prop::TYPE::Entities;
                    MCE.Ent_Prop.Amount = MCE_Prop::AMOUNT::Multiple;
                    MCE.Sort = SORT::Unset;
                    break;

                case L'p':
                    MCE.Ent_Prop.Type = MCE_Prop::TYPE::Players;
                    MCE.Ent_Prop.Amount = MCE_Prop::AMOUNT::Single;
                    MCE.Sort = SORT::Nearest;
                    break;

                case L'r':
                    MCE.Ent_Prop.Type = MCE_Prop::TYPE::Players;
                    MCE.Ent_Prop.Amount = MCE_Prop::AMOUNT::Single;
                    MCE.Sort = SORT::Random;
                    break;

                case L's':
                    MCE.Ent_Prop.Type = MCE_Prop::TYPE::Entities;
                    MCE.Ent_Prop.Amount = MCE_Prop::AMOUNT::Single;
                    MCE.Sort = SORT::Unset;
                    break;

                default:
                    Error_Handler << L"Name containing ilegal char(s)";
                    return false;
                }

                if (Word.size() > 2) {
                    if (Word.at(2) != L'[') {
                        Error_Handler << L"Selector must be followed by space or '['";
                        return false;
                    }

                    if (Word.back() != L']') {
                        Error_Handler << L"Expected ']' on the end of the selector";
                        return false;
                    }



                    //@x[TEXT IN HERE]
                    enum class Modes { String, Operator, Value, Seperator };
                    enum class Sub_Modes { Space, Char };

                    std::wstring Brackets;
                    Modes Mode = Modes::String;
                    Sub_Modes Sub_Mode = Sub_Modes::Space;

                    std::vector<std::wstring> Params;
                    Params.emplace_back();

                    bool Element_Start = false;
                    std::wstring Paramater_str = Word.substr(3, Word.size() - 4);
                    for (size_t Pos = 0; Pos < Paramater_str.size(); Pos++) {
                        wchar_t ch = Paramater_str.at(Pos);

                        if (Mode == Modes::String) {
                            if (Sub_Mode == Sub_Modes::Space) {
                                if (Is_Syntax_Ch(ch)) {
                                    Error_Handler << L"Expected non-syntax char";
                                    return false;
                                }

                                else if (ch != L' ') {
                                    Element_Start = true;
                                    Sub_Mode = Sub_Modes::Char;
                                }
                            }

                            if (Sub_Mode != Sub_Modes::Space) {
                                if (ch == L' ') {
                                    Mode = Modes::Operator;
                                    Sub_Mode = Sub_Modes::Space;
                                } else if (ch == L'=') {
                                    Element_Start = false;
                                    Mode = Modes::Value;
                                    Sub_Mode = Sub_Modes::Space;
                                    Params.emplace_back();
                                }

                                else {
                                    Params.back().push_back(ch);
                                }
                            }
                        }

                        else if (Mode == Modes::Operator) {
                            if (ch == L'=') {
                                Element_Start = false;
                                Mode = Modes::Value;
                                Sub_Mode = Sub_Modes::Space;
                                Params.emplace_back();
                            }

                            else if (ch != L' ') {
                                Error_Handler << L"Expected L' ' or L'='";
                                return false;
                            }
                        }

                        else if (Mode == Modes::Value) {
                            if (Is_Open_Bracket(ch)) {
                                Brackets.push_back(ch);
                                Params.back().push_back(ch);
                            } else if (Is_Closing_Bracket(ch)) {
                                if (ch == Matching_Bracket(Brackets.back())) {
                                    Brackets.pop_back();
                                    Params.back().push_back(ch);
                                } else {
                                    Error_Handler << L"Unmached brackets";
                                    return false;
                                }
                            }

                            else if (Brackets.size() == 0) {
                                if (Sub_Mode == Sub_Modes::Space) {
                                    if (ch == L',') {
                                        Element_Start = true;
                                        Mode = Modes::String;
                                        Sub_Mode = Sub_Modes::Space;
                                        Params.emplace_back();
                                    }

                                    else if (ch != L' ') {
                                        Sub_Mode = Sub_Modes::Char;
                                    }
                                }

                                if (Sub_Mode != Sub_Modes::Space) {
                                    if (ch == L' ') {
                                        Mode = Modes::Seperator;
                                        Sub_Mode = Sub_Modes::Space;
                                    } else if (ch == L',') {
                                        Element_Start = true;
                                        Mode = Modes::String;
                                        Sub_Mode = Sub_Modes::Space;
                                        Params.emplace_back();
                                    }

                                    else {
                                        Params.back().push_back(ch);
                                    }
                                }
                            }

                            else {
                                Params.back().push_back(ch);
                            }
                        }

                        else if (Mode == Modes::Seperator) {
                            if (ch == L',') {
                                Element_Start = true;
                                Mode = Modes::String;
                                Sub_Mode = Sub_Modes::Space;
                                Params.emplace_back();
                            }

                            else if (ch != L' ') {
                                Error_Handler << L"Expected L' ' or L','";
                                return false;
                            }
                        }


                        if (END_ITER(Pos, Paramater_str.size())) {
                            if (Element_Start) {
                                Error_Handler << L"Unexpected end of selector";
                                return false;
                            }
                        }

                    }

                    if (Params.size() == 1) return true;
                    Log_IO::Set_Color::Info();
                    for (size_t Word = 0; Word < Params.size(); Word++) {
                        Log_IO::wcout() << Params.at(Word) << std::endl;
                        if (Params.at(Word) == L"name") {
                            Word++;

                            if (Params.at(Word).find_first_not_of(AllowedChars) != std::wstring::npos) {
                                Error_Handler << L"Name containing ilegal char(s)";
                                return false;
                            }

                            if (Params.at(Word).size() > 16) {
                                Error_Handler << L"Too long Name: " << std::to_wstring(Params.at(Word).size()).c_str();
                                return false;
                            }

                            MCE.Name.Name = Params.at(Word);
                            MCE.Name.This = true;
                        }


                    }
                    Log_IO::Set_Color::Default();







                }
            }
        }
    }

    return true;
}


bool Is_Syntax_Ch(wchar_t ch) {
    switch (ch) {
    case L'=': return true;
    case L',': return true;
    }

    return false;
}


bool Is_Bracket(wchar_t ch) {
    switch (ch) {
    case L'(': return true;
    case L'[': return true;
    case L'{': return true;
    case L')': return true;
    case L']': return true;
    case L'}': return true;
    }

    return false;
}

bool Is_Open_Bracket(wchar_t ch) {
    switch (ch) {
    case L'(': return true;
    case L'[': return true;
    case L'{': return true;
    }

    return false;
}

bool Is_Closing_Bracket(wchar_t ch) {
    switch (ch) {
    case L')': return true;
    case L']': return true;
    case L'}': return true;
    }

    return false;
}


wchar_t Matching_Bracket(wchar_t ch) {
    switch (ch) {
    case L'(': return L')';
    case L'[': return L']';
    case L'{': return L'}';
    case L')': return L'(';
    case L']': return L'[';
    case L'}': return L'{';
    }

    return false;
}
