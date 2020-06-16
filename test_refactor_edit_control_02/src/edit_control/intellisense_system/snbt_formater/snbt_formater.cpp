#include "pch.h"
#include "snbt_formater.h"

bool SNbt_Formater::Is_Valid(std::wstring Word, std::wstring* Error) {
    return Is_Valid_nc(&Word, Error);
}

bool SNbt_Formater::Is_Valid_nc(std::wstring* Word_Ptr, std::wstring* Error) {
    auto& Word = *Word_Ptr;
    if (!Compress_Str(&Word, Error)) {
        return false;
    }

    bool Is_Root = true;
    std::wstring Object_Tracker;
    size_t SDV_Tracker = 0;

    size_t SD_Size = Word.size();

    std::wstring Obj_Name_Str;
    bool Enforced_Value = false;
    bool Look_For_Comma = false;

    for (size_t Pos = 0; Pos < Word.size(); Pos++) {
        wchar_t SD_ch = Word.at(Pos);

        if (!Is_Root && Object_Tracker.size() == 0) {
            *Error = L"Too many braces...";
            return false;
        }

        if (Is_Root) {
            if (SD_ch == L'[') {
                Object_Tracker.push_back(SD_ch);
            } else if (SD_ch == L'{') {
                Object_Tracker.push_back(SD_ch);
            }

            else {
                //One-element tree
                //Error_Handler << "Making One-element tree" << std::endl;
                if (SD_ch == 'Q') {
                    Object_Tracker.push_back(SD_ch);
                } else if (SD_ch == 'U') {
                    Object_Tracker.push_back(SD_ch);
                }

                SDV_Tracker++;
                return true;
            }

            Is_Root = false;
        }

        //( ._.) hmm... ok... ok...
        else if (Object_Tracker.back() == L'[') {

            if (Is_Value(SD_ch) && Look_For_Comma) {
                *Error = L"Missing comma between 2 values";
                return false;
            }

            if (SD_ch == L',') {
                Enforced_Value = true;
                Look_For_Comma = false;
            }

            else if (SD_ch == L'[') {
                Object_Tracker.push_back(SD_ch);
                Enforced_Value = false;
            }

            else if (SD_ch == L'{') {
                Object_Tracker.push_back(SD_ch);
                Enforced_Value = false;
                Look_For_Comma = false;
            }

            else if (SD_ch == L'U' || SD_ch == L'Q') {
                SDV_Tracker++;
                Enforced_Value = false;
                Look_For_Comma = true;
            }


            else if (SD_ch == L']') {
                if (Enforced_Value) {
                    *Error = L"You said, next char is some value, but you gave me closing ']'";
                    return false;
                }

                else {
                    Object_Tracker.pop_back();
                }
            }

            else if (SD_ch == L'}') {
                *Error = L"What the hell is this '}' working in a array?!";
                return false;
            }
        }


        //(._. )  hmm... ok... yes... aha-ok...
        else if (Object_Tracker.back() == L'{') {
            if (SD_ch == L'U' || SD_ch == L'Q') {
                if (Pos + 2 < Word.size()) {
                    Pos++;
                    if (Word.at(Pos) == L':') {
                        SDV_Tracker++;
                        Pos++;
                        SD_ch = Word.at(Pos);

                        if (!Is_Value(SD_ch)) {
                            *Error = L"Missing value after ':'";
                            return false;
                        }
                    } else {
                        *Error = L"Missing ':'";
                        return false;
                    }
                } else {
                    *Error = L"Unexpected end of the file";
                    return false;
                }
            } else if (SD_ch != L',' && SD_ch != L'}') {
                *Error = L"Missing name in object";
                return false;
            }


            if (Is_Value(SD_ch) && Look_For_Comma) {
                *Error = L"Missing comma between 2 values";
                return false;
            }

            if (SD_ch == L',') {
                Enforced_Value = true;
                Look_For_Comma = false;
            }

            else if (SD_ch == L'[') {
                Object_Tracker.push_back(SD_ch);
                Enforced_Value = false;
                Look_For_Comma = false;
            }

            else if (SD_ch == L'{') {
                Object_Tracker.push_back(SD_ch);
                Enforced_Value = false;
                Look_For_Comma = false;
            }

            else if (SD_ch == L'Q' || SD_ch == L'U') {
                SDV_Tracker++;
                Enforced_Value = false;
                Look_For_Comma = true;
            }

            else if (SD_ch == L'}') {
                if (Enforced_Value) {
                    *Error = L"You said, next char is some value, but you gave me closing '}'";
                    return false;
                }

                else {
                    Object_Tracker.pop_back();
                }
            }

            else if (SD_ch == L']') {
                *Error = L"What the hell is this ']' doing in a object?!";
                return false;
            }
        }
    }

    if (Object_Tracker.size() > 0) {
        *Error = L"Not all braces/brackets are closed... wut?!";
        return false;
    }

    return true;
}
