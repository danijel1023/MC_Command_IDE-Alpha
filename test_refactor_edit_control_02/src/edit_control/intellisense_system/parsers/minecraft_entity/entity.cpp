#include "pch.h"
#include "intelisense.h"
#include "entity_struct.h"

#define END_ITER(i, max) (i + 1 == max)

bool Is_Syntax_Ch(wchar_t ch);
bool Is_Bracket(wchar_t ch);
bool Is_Open_Bracket(wchar_t ch);
bool Is_Closing_Bracket(wchar_t ch);
wchar_t Matching_Bracket(wchar_t ch);



bool IntelliSense::Minecraft_Entity(std::wstring& Word) {
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
            return Validate_Name(Word);
        } else {
            if (Word.size() == 1) {
                Error_Handler << L"Name containing ilegal char(s)";
                return false;
            } else {
                switch (Word.at(1)) {
                case L'a':
                    MCE.Ent_Prop.Type = MCE_Prop::TYPE::Players;
                    MCE.Ent_Prop.Amount = MCE_Prop::AMOUNT::Multiple;
                    MCE.Sort = SORT::Arbitrary;
                    break;

                case L'e':
                    MCE.Ent_Prop.Type = MCE_Prop::TYPE::Entities;
                    MCE.Ent_Prop.Amount = MCE_Prop::AMOUNT::Multiple;
                    MCE.Sort = SORT::Arbitrary;
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
                    MCE.Sort = SORT::Arbitrary;
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
                    for (size_t Word = 0; Word < Params.size(); Word++) {
                        if (Params.at(Word) == L"name") {
                            if (MCE.Name.Use) {
                                Error_Handler << L"Already using 'name'";
                                return false;
                            }
                            Word++;
                            if (!Validate_Name(Params.at(Word))) return false;
                            MCE.Name = Params.at(Word);
                        }

                        else if (Params.at(Word) == L"nbt") {
                            if (MCE.NBT.Use) {
                                Error_Handler << L"Already using 'nbt'";
                                return false;
                            }
                            Word++;
                            if (!Validate_NBT(Params.at(Word))) return false;
                            MCE.Name = Params.at(Word);
                        }

                        else if (Params.at(Word) == L"team") {
                            if (MCE.Team.Use) {
                                Error_Handler << L"Already using 'team'";
                                return false;
                            }
                            Word++;
                            if (!Validate_Name(Params.at(Word))) return false;
                            MCE.Team = Params.at(Word);
                        }

                        else if (Params.at(Word) == L"advancements") {
                            if (MCE.Advancements.Use) {
                                Error_Handler << L"Already using 'advancements'";
                                return false;
                            }
                            Word++;
                            if (!Validate_Advancements(Params.at(Word))) return false;
                            MCE.Advancements = Params.at(Word);
                        }

                        else if (Params.at(Word) == L"scores") {
                            if (MCE.Scores.Use) {
                                Error_Handler << L"Already using 'scores'";
                                return false;
                            }
                            Word++;
                            if (!Validate_Scores(Params.at(Word))) return false;
                            MCE.Scores = Params.at(Word);
                        }


                        else if (Params.at(Word) == L"x") {
                            if (MCE.Coordinates.X.Use) {
                                Error_Handler << L"Already using 'x'";
                                return false;
                            }
                            Word++;
                            if (!Get_Num(Params.at(Word), &MCE.Coordinates.X)) return false;
                        }
                        else if (Params.at(Word) == L"y") {
                            if (MCE.Coordinates.Y.Use) {
                                Error_Handler << L"Already using 'y'";
                                return false;
                            }
                            Word++;
                            if (!Get_Num(Params.at(Word), &MCE.Coordinates.Y)) return false;
                        }
                        else if (Params.at(Word) == L"z") {
                            if (MCE.Coordinates.Z.Use) {
                                Error_Handler << L"Already using 'z'";
                                return false;
                            }
                            Word++;
                            if (!Get_Num(Params.at(Word), &MCE.Coordinates.Z)) return false;
                        }


                        else if (Params.at(Word) == L"distance") {
                            if (MCE.Select_Area.Use_Volume) {
                                Error_Handler << L"Already using volume";
                                return false;
                            }

                            if (MCE.Select_Area.Radius.Use) {
                                Error_Handler << L"Already using 'distance'";
                                return false;
                            }
                            Word++;
                            MCE.Select_Area.Use_Radius = true;
                            if (!Get_Range(Params.at(Word), &MCE.Select_Area.Radius)) return false;
                        }

                        else if (Params.at(Word) == L"dx") {
                            if (MCE.Select_Area.Use_Radius) {
                                Error_Handler << L"Already using radius";
                                return false;
                            }
                            if (MCE.Select_Area.DX.Use) {
                                Error_Handler << L"Already using 'dx'";
                                return false;
                            }
                            Word++;
                            MCE.Select_Area.Use_Volume = true;
                            if (!Get_Num(Params.at(Word), &MCE.Select_Area.DX)) return false;
                        }
                        else if (Params.at(Word) == L"dy") {
                            if (MCE.Select_Area.Use_Radius) {
                                Error_Handler << L"Already using radius";
                                return false;
                            }
                            if (MCE.Select_Area.DY.Use) {
                                Error_Handler << L"Already using 'dy'";
                                return false;
                            }
                            Word++;
                            MCE.Select_Area.Use_Volume = true;
                            if (!Get_Num(Params.at(Word), &MCE.Select_Area.DY)) return false;
                        }
                        else if (Params.at(Word) == L"dz") {
                            if (MCE.Select_Area.Use_Radius) {
                                Error_Handler << L"Already using radius";
                                return false;
                            }
                            if (MCE.Select_Area.DZ.Use) {
                                Error_Handler << L"Already using 'dz'";
                                return false;
                            }
                            Word++;
                            MCE.Select_Area.Use_Volume = true;
                            if (!Get_Num(Params.at(Word), &MCE.Select_Area.DZ)) return false;
                        }


                        else if (Params.at(Word) == L"x_rotation") {
                            if (MCE.Rotation.HRotation.Use) {
                                Error_Handler << L"Already using 'x_rotation'";
                                return false;
                            }
                            Word++;
                            if (!Get_Range(Params.at(Word), &MCE.Rotation.HRotation)) return false;
                        }

                        else if (Params.at(Word) == L"y_rotation") {
                            if (MCE.Rotation.HRotation.Use) {
                                Error_Handler << L"Already using 'y_rotation'";
                                return false;
                            }
                            Word++;
                            if (!Get_Range(Params.at(Word), &MCE.Rotation.VRotation)) return false;
                        }


                        else if (Params.at(Word) == L"level") {
                            if (MCE.XP_Level.Use) {
                                Error_Handler << L"Already using 'level'";
                                return false;
                            }
                            Word++;
                            if (!Get_Range(Params.at(Word), &MCE.XP_Level)) return false;
                        }


                        else if (Params.at(Word) == L"limit") {
                            if (MCE.Limit.Use) {
                                Error_Handler << L"Already using 'limit'";
                                return false;
                            }
                            Word++;
                            if (!Get_Num(Params.at(Word), &MCE.Limit)) return false;
                        }


                        else if (Params.at(Word) == L"tags") {
                            Word++;
                            if (!Validate_Tags(Params.at(Word))) return false;
                            MCE.Tags = Params.at(Word);
                        }


                        else {
                            Error_Handler << L"Unindentified parameter";
                            return false;
                        }
                    }
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
