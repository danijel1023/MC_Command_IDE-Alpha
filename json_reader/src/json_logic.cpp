#include "json.h"

#include <Windows.h>
/*#define log_err(x) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);\
                     std::wcout << x << std::endl;\
                     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); }*/

bool Json::Is_Number(wchar_t N, int Exclude) {
    if (((N >= 48) && (N <= 57)) && N != (Exclude + 48)) return true;
    else return false;
}

bool Json::Is_Value(wchar_t ch) {
    switch (ch) {
    case L'S': return true;
    case L'N': return true;
    case L'B': return true;
    case L'P': return true;
    case L'{': return true;
    case L'[': return true;
    default:
        return false;
    }
}

bool Json::Is_Syntax_ch(wchar_t ch) {
    switch (ch) {
    case L'{': return true;
    case L'}': return true;
    case L'[': return true;
    case L']': return true;
    case L':': return true;
    case L',': return true;
    default:
        return false;
    }
}


void Json::Build_Tree(std::wstring& Simp_Data, std::vector<std::wstring>& Simp_Data_Val) {
    //[TODO]: Add error support
    //Simp_Data -> {N:[N,B,N,{S:S,S:S}],S:B}
    //Simp_Data_Val ->    'N'(1) "..."
    //                    'N'(2) "..."
    //                    'B'(3) "..."
    //                    'N'(4) "..."
    //                    'S'(5) "..."
    //                    ...

    bool Is_Root = true;
    std::wstring Object_Tracker;
    size_t SDV_Tracker = 0;

    size_t SD_Size = Simp_Data.size();

    std::wstring Obj_Name_Str;
    bool Enforced_Value = false;
    bool Look_For_Comma = false;

    for (size_t Pos = 0; Pos < Simp_Data.size(); Pos++) {
        wchar_t SD_ch = Simp_Data.at(Pos);

        if (!Is_Root && Object_Tracker.size() == 0) {
            std::wcout << "err32: to many braces..." << std::endl;
            return;
        }

        if (Is_Root) {
            if (SD_ch == L'[') {
                //std::wcout << "Making root - type 'Array'" << std::endl;
                Object_Tracker.push_back(SD_ch);
                std::get<0>(*m_Root) = new Arr_Element;
                std::get<1>(*m_Root) = Data_Value_Type::Array;
                m_Branch->push_back(*m_Root);
            } else if (SD_ch == L'{') {
                //std::wcout << "Making root - type 'Object'" << std::endl;
                Object_Tracker.push_back(SD_ch);
                std::get<0>(*m_Root) = new Obj_Element;
                std::get<1>(*m_Root) = Data_Value_Type::Object;
                m_Branch->push_back(*m_Root);
            }

            else {
                //One-element tree
                //std::wcout << "Making One-element tree" << std::endl;
                if (SD_ch == 'S') {
                    Object_Tracker.push_back(SD_ch);
                    std::get<0>(*m_Root) = new Str_Element;
                    std::get<1>(*m_Root) = Data_Value_Type::String;
                    *((Str_Element*)std::get<0>(*m_Root)) = Simp_Data_Val.at(SDV_Tracker);
                    m_Branch->push_back(*m_Root);
                } else if (SD_ch == 'N') {
                    Object_Tracker.push_back(SD_ch);
                    std::get<0>(*m_Root) = new Num_Element;
                    std::get<1>(*m_Root) = Data_Value_Type::Number;
                    *((Num_Element*)std::get<0>(*m_Root)) = std::stold(Simp_Data_Val.at(SDV_Tracker));
                    m_Branch->push_back(*m_Root);
                } else if (SD_ch == 'B') {
                    Object_Tracker.push_back(SD_ch);
                    std::get<0>(*m_Root) = new Bol_Element;
                    std::get<1>(*m_Root) = Data_Value_Type::Bool;
                    *((Bol_Element*)std::get<0>(*m_Root)) = (Simp_Data_Val.at(SDV_Tracker) == L"true");
                    m_Branch->push_back(*m_Root);
                } else if (SD_ch == 'P') {
                    Object_Tracker.push_back(SD_ch);
                    std::get<0>(*m_Root) = new Ptr_Element;
                    std::get<1>(*m_Root) = Data_Value_Type::Pointer;
                    *((Ptr_Element*)std::get<0>(*m_Root)) = nullptr;
                    m_Branch->push_back(*m_Root);
                }

                SDV_Tracker++;
                return;
            }

            Is_Root = false;
        }

        //( ._.) hmm... ok... ok...
        else if (Object_Tracker.back() == L'[') {
            auto C_Elm = ((Element_Struct_TypeArr*)std::get<0>(m_Branch->back()));

            if (Is_Value(SD_ch) && Look_For_Comma) {
                std::wcout << "err28: missing comma between 2 values" << std::endl;
                return;
            }

            if (SD_ch == L',') {
                Enforced_Value = true;
                Look_For_Comma = false;
            }

            else if (SD_ch == L'[') {
                //std::wcout << "Adding branch from 'Array' - type 'Array'" << std::endl;
                auto New_Elm = new Arr_Element;
                C_Elm->Elements.push_back({ New_Elm, Data_Value_Type::Array });
                m_Branch->push_back({ New_Elm, Data_Value_Type::Array });
                Object_Tracker.push_back(SD_ch);
                Enforced_Value = false;
            }

            else if (SD_ch == L'{') {
                //std::wcout << "Adding branch from 'Array' - type 'Object'" << std::endl;
                auto New_Elm = new Obj_Element;
                C_Elm->Elements.push_back({ New_Elm, Data_Value_Type::Object });
                m_Branch->push_back({ New_Elm, Data_Value_Type::Object });
                Object_Tracker.push_back(SD_ch);
                Enforced_Value = false;
                Look_For_Comma = false;
            }

            else if (SD_ch == L'S') {
                //std::wcout << "Adding Element to 'Array' - type 'String' (" << Simp_Data_Val.at(SDV_Tracker) << ")" << std::endl;
                auto New_Elm = new Str_Element;
                *New_Elm = Simp_Data_Val.at(SDV_Tracker);
                C_Elm->Elements.push_back({ New_Elm, Data_Value_Type::String });
                SDV_Tracker++;
                Enforced_Value = false;
                Look_For_Comma = true;
            }

            else if (SD_ch == L'N') {
                //std::wcout << "Adding Element to 'Array' - type 'Number' (" << Simp_Data_Val.at(SDV_Tracker) << ")" << std::endl;
                auto New_Elm = new Num_Element;
                *New_Elm = std::stold(Simp_Data_Val.at(SDV_Tracker));
                C_Elm->Elements.push_back({ New_Elm, Data_Value_Type::Number });
                SDV_Tracker++;
                Enforced_Value = false;
                Look_For_Comma = true;
            }

            else if (SD_ch == L'B') {
                //std::wcout << "Adding Element to 'Array' - type 'Bool' (" << Simp_Data_Val.at(SDV_Tracker) << ")" << std::endl;
                auto New_Elm = new Bol_Element;
                *New_Elm = (Simp_Data_Val.at(SDV_Tracker) == L"true");
                C_Elm->Elements.push_back({ New_Elm, Data_Value_Type::Bool });
                SDV_Tracker++;
                Enforced_Value = false;
                Look_For_Comma = true;
            }

            else if (SD_ch == L'P') {
                //std::wcout << "Adding Element to 'Array' - type 'Pointer' (" << Simp_Data_Val.at(SDV_Tracker) << ")" << std::endl;
                auto New_Elm = new Ptr_Element;
                *New_Elm = nullptr;
                C_Elm->Elements.push_back({ New_Elm, Data_Value_Type::Pointer });
                SDV_Tracker++;
                Enforced_Value = false;
                Look_For_Comma = true;
            }


            else if (SD_ch == L']') {
                if (Enforced_Value) {
                    std::cout << "err27: you said, next char is some value, but you gave me closing ']'" << std::endl;
                    return;
                }

                else {
                    //std::wcout << "Returning to element in branch from '" << ((Object_Tracker.back() == L'[') ? "Array" : "Object") << "' - type 'Array'" << std::endl;
                    Object_Tracker.pop_back();
                    m_Branch->pop_back();

                }
            }

            else if (SD_ch == L'}') {
                //std::cout << "err28: what the hell is this '}' working in a array?!" << std::endl;
                return;
            }
        }


        //(._. )  hmm... ok... yes... aha-ok...
        else if (Object_Tracker.back() == L'{') {
            auto C_Elm = ((Element_Struct_TypeObj*)std::get<0>(m_Branch->back()));

            if (SD_ch == L'S') {
                if (Pos + 2 < Simp_Data.size()) {
                    Pos++;
                    if (Simp_Data.at(Pos) == L':') {
                        Obj_Name_Str = Simp_Data_Val.at(SDV_Tracker);
                        SDV_Tracker++;

                        Pos++;
                        SD_ch = Simp_Data.at(Pos);

                        if (!Is_Value(SD_ch)) {
                            std::wcout << "err33: Missing value after ':'" << std::endl;
                            return;
                        }
                    } else {
                        std::wcout << "err13: Missing ':'" << std::endl;
                        return;
                    }
                } else {
                    std::wcout << "err30: Unexpected end of the file" << std::endl;
                    return;
                }
            } else if (SD_ch != L',' && SD_ch != L'}') {
                std::cout << "err31: Missing name in object" << std::endl;
                return;
            }


            if (Is_Value(SD_ch) && Look_For_Comma) {
                std::wcout << "err28: missing comma between 2 values" << std::endl;
                return;
            }

            if (SD_ch == L',') {
                Enforced_Value = true;
                Look_For_Comma = false;
            }

            else if (SD_ch == L'[') {
                //std::wcout << "Adding branch from 'Object' - type 'Array'" << std::endl;
                auto New_Elm = new Arr_Element;
                C_Elm->Elements.push_back({ New_Elm, Data_Value_Type::Array, Obj_Name_Str });
                m_Branch->push_back({ New_Elm, Data_Value_Type::Array });
                Object_Tracker.push_back(SD_ch);
                Enforced_Value = false;
                Look_For_Comma = false;
            }

            else if (SD_ch == L'{') {
                //std::wcout << "Adding branch from 'Object' - type 'Object'" << std::endl;
                auto New_Elm = new Obj_Element;
                C_Elm->Elements.push_back({ New_Elm, Data_Value_Type::Object, Obj_Name_Str });
                m_Branch->push_back({ New_Elm, Data_Value_Type::Object });
                Object_Tracker.push_back(SD_ch);
                Enforced_Value = false;
                Look_For_Comma = false;
            }

            else if (SD_ch == L'S') {
                //std::wcout << "Adding Element to 'Object' - type 'String' (" << Simp_Data_Val.at(SDV_Tracker) << ")" << std::endl;
                auto New_Elm = new Str_Element;
                *New_Elm = Simp_Data_Val.at(SDV_Tracker);
                C_Elm->Elements.push_back({ New_Elm, Data_Value_Type::String, Obj_Name_Str });
                SDV_Tracker++;
                Enforced_Value = false;
                Look_For_Comma = true;
            }

            else if (SD_ch == L'N') {
                //std::wcout << "Adding Element to 'Object' - type 'Number' (" << Simp_Data_Val.at(SDV_Tracker) << ")" << std::endl;
                auto New_Elm = new Num_Element;
                *New_Elm = std::stold(Simp_Data_Val.at(SDV_Tracker));
                C_Elm->Elements.push_back({ New_Elm, Data_Value_Type::Number, Obj_Name_Str });
                SDV_Tracker++;
                Enforced_Value = false;
                Look_For_Comma = true;
            }

            else if (SD_ch == L'B') {
                //std::wcout << "Adding Element to 'Object' - type 'Bool' (" << Simp_Data_Val.at(SDV_Tracker) << ")" << std::endl;
                auto New_Elm = new Bol_Element;
                *New_Elm = (Simp_Data_Val.at(SDV_Tracker) == L"true");
                C_Elm->Elements.push_back({ New_Elm, Data_Value_Type::Bool, Obj_Name_Str });
                SDV_Tracker++;
                Enforced_Value = false;
                Look_For_Comma = true;
            }

            else if (SD_ch == L'P') {
                //std::wcout << "Adding Element to 'Object' - type 'Pointer' (" << Simp_Data_Val.at(SDV_Tracker) << ")" << std::endl;
                auto New_Elm = new Ptr_Element;
                *New_Elm = nullptr;
                C_Elm->Elements.push_back({ New_Elm, Data_Value_Type::Pointer, Obj_Name_Str });
                SDV_Tracker++;
                Enforced_Value = false;
                Look_For_Comma = true;
            }


            else if (SD_ch == L'}') {
                if (Enforced_Value) {
                    std::cout << "err27: you said, next char is some value, but you gave me closing '}'" << std::endl;
                    return;
                }

                else {
                    //std::wcout << "Returning to element in branch from '" << ((Object_Tracker.back() == L'[') ? "Array" : "Object") << "' - type 'Object'" << std::endl;
                    Object_Tracker.pop_back();
                    m_Branch->pop_back();

                }
            }

            else if (SD_ch == L']') {
                std::cout << "err28: what the hell is this ']' working in a object?!" << std::endl;
                return;
            }
        }
    }

    if (Object_Tracker.size() > 0) {
        std::wcout << "err29: Not all braces/brackets are closed... wut?! And then you expect me to build you this tree.. gee" << std::endl;
        return;
    }

    m_Error_Building_Tree = false;
}


void Json::Remove_Unnec_Char(std::wstring& Data) {
    bool Is_Str = false;
    bool Is_Comment = false;
    size_t Comment_Start = 0;

    for (size_t Cur = 0; Cur < Data.size(); Cur++) {
        wchar_t CCh = Data.at(Cur);

        if (CCh == '\\' && Cur + 1 >= Data.size() && !Is_Comment) {
            Is_Comment = true;
            continue;
        }

        else if (CCh == L'"' && !Is_Comment) Is_Str = !Is_Str;

        else if (!Is_Str && !Is_Comment) {
            if (CCh == L'/' && Cur + 1 < Data.size() && Data.at(Cur + 1) == L'/') {
                Is_Comment = true;
                Comment_Start = Cur;
            }

            else if (CCh == L' ' || CCh == L'\t' || CCh == L'\n') {
                Data.erase(Data.begin() + Cur);
                Cur--;
            }

        }
        
        else if (Is_Comment && (CCh == L'\n' || Cur + 1 == Data.size())) {
            Is_Comment = false;
            Data.erase(Data.begin() + Comment_Start, Data.begin() + (Cur + 1));
            Cur = Comment_Start - 1;
        }
    }
}


void Json::Format_Data(std::wstring& InputData, std::wstring* Simp_Data_Ptr, std::vector<std::wstring>* Simp_Data_Val_Ptr) {
    auto& Simp_Data = *Simp_Data_Ptr;
    auto& Simp_Data_Val = *Simp_Data_Val_Ptr;

    //Simp_Data -> {S:[N,N,N,{N:V,N:V}],N:V}
    //Simp_Data_Val ->    (N)(1) "..."
    //                    (V)(2) "..."
    //                    (V)(3) "..."
    //                    (V)(4) "..."
    //                    (N)(5) "..."
    //                    ...

    bool Is_Nmb = false;    //Positive & Negative ("null" is just 0 and not type on its own)
    bool Is_Str = false;    //String
    bool Is_Bol = false;    //"true" & "false"
    bool Decimal = false;
    bool Exponent = false;
    bool Negative = false;

    size_t Last_Pos = 0;
    bool Skip_Next = false;
    size_t InDt_Size = InputData.size();
    for (size_t Pos = 0; Pos < InDt_Size; Pos++) {
        if (Skip_Next) { Skip_Next = false; continue; }
        wchar_t ch = InputData.at(Pos);
        wchar_t Next_ch = 0;

        if (Pos + 1 < InDt_Size) Next_ch = InputData.at(Pos + 1);;

        if (Is_Nmb) {
            if (ch == L'.') {
                if (Pos + 1 < InDt_Size && Is_Number(Next_ch) && !Decimal) {
                    Decimal = true;
                    Pos++;
                } else {
                    //std::wcout << InputData.substr(Pos - 15, 30);
                    std::wcout << "err2" << std::endl;
                    return;
                }
            }

            else if (ch == L'e' || ch == L'E') {
                if (Exponent) {
                    std::wcout << "err3" << std::endl;
                    return;
                }
                Exponent = true;

                if (Is_Number(Next_ch)) {
                    Pos++;
                } else if (Pos + 1 < InDt_Size && Next_ch == L'+' || Next_ch == L'-') {
                    Pos++;
                    if (Pos + 1 < InDt_Size && Is_Number(InputData.at(Pos + 1))) {
                        Pos++;
                    } else {
                        std::wcout << "err6" << std::endl;
                        return;
                    }
                } else {
                    std::wcout << "err4" << std::endl;
                    return;
                }
            }

            //Edge case - number root
            else if (!Is_Number(ch)) {
                Is_Nmb = false;
                Decimal = false;
                Exponent = false;
                Negative = false;
                Simp_Data += L'N';

                Simp_Data_Val.push_back(InputData.substr(Last_Pos, Pos - Last_Pos));
                Pos--;
            }
        }

        else if (Is_Str) {
            if (ch == L'\\') {
                InputData.erase(Pos, 1);
                InDt_Size--;
                Skip_Next = true;
                continue;
            }

            if (ch == L'"') {
                Is_Str = false;
                Simp_Data += L'S';
                Simp_Data_Val.push_back(InputData.substr(Last_Pos + 1, (Pos - 1) - Last_Pos));
            }
        }

        else if (ch == L'-') {
            if (Pos + 1 < InDt_Size && Is_Number(Next_ch) && !Negative) {
                Is_Nmb = true;
                Last_Pos = Pos;
                Pos++;
                Negative = true;
            } else {
                std::wcout << L"err1" << std::endl;
                return;
            }
        } else if (Is_Number(ch, 0)) {  // Test For 1-9 without 0
            Is_Nmb = true;
            Last_Pos = Pos;
        } else if (Is_Number(ch)) {
            //If last test failed and 'ch' is number, then it must be 0
            if (Pos + 1 < InDt_Size && (Next_ch == L'.' || Next_ch == L'e' || Next_ch == L'E' || Next_ch == L'}' || Next_ch == L']' || Next_ch == L',')) {
                Is_Nmb = true;
                Last_Pos = Pos;

            } else if (Is_Number(Next_ch)) {
                std::wcout << "err5" << std::endl;
                return;
            } else if (!Is_Syntax_ch(Next_ch)) {
                std::wcout << "err26: !=dsf" << std::endl;
                return;
            }
        }

        else if (ch == L'"') {
            Is_Str = true;
            Last_Pos = Pos;
        }

        else if (Is_Syntax_ch(ch)) Simp_Data += ch;

        else if (Pos + 3 < InDt_Size) {
            if (InputData.substr(Pos, 4) == L"null") {
                Simp_Data += L'P';
                Simp_Data_Val.push_back(L"null");
                Pos += 3;
            }

            else if (InputData.substr(Pos, 4) == L"true") {
                Simp_Data += L'B';
                Simp_Data_Val.push_back(L"true");
                Pos += 3;
            }
        } else if (Pos + 4 < InDt_Size) {
            if (InputData.substr(Pos, 5) == L"false") {
                Simp_Data += L'B';
                Simp_Data_Val.push_back(L"false");
                Pos += 4;
            }
        }
        
        else {
            std::wcout << "err7" << std::endl;
            return;
        }
    }

    m_Error_Building_Tree = false;
}


void Json::Copy_Tree(const Json& From) {
    if (m_Branch->size() == 0) return;
    void* FC_Ptr = std::get<0>(From.m_Branch->back());
    auto FC_Type = std::get<1>(From.m_Branch->back());

    if (FC_Type == Data_Value_Type::Number) {
        *(Num_Element*)std::get<0>(m_Branch->back()) = *(Num_Element*)std::get<0>(From.m_Branch->back());

        From.m_Branch->pop_back();
        m_Branch->pop_back();
        return;
    }
    
    else if (FC_Type == Data_Value_Type::String) {
        *(Str_Element*)std::get<0>(m_Branch->back()) = *(Str_Element*)std::get<0>(From.m_Branch->back());

        From.m_Branch->pop_back();
        m_Branch->pop_back();
        return;
    }
    
    else if (FC_Type == Data_Value_Type::Bool) {
        *(Bol_Element*)std::get<0>(m_Branch->back()) = *(Bol_Element*)std::get<0>(From.m_Branch->back());

        From.m_Branch->pop_back();
        m_Branch->pop_back();
        return;
    }
    
    else if (FC_Type == Data_Value_Type::Pointer) {
        *(Ptr_Element*)std::get<0>(m_Branch->back()) = *(Ptr_Element*)std::get<0>(From.m_Branch->back());

        From.m_Branch->pop_back();
        m_Branch->pop_back();
        return;
    }
    
    
    else if (FC_Type == Data_Value_Type::Array) {
        auto FElm = ((Arr_Element*)FC_Ptr);
        auto Elm = ((Arr_Element*)std::get<0>(m_Branch->back()));

        for (size_t i = 0; i < FElm->Elements.size(); i++) {
            Elm->Elements.emplace_back();

            switch (std::get<1>(FElm->Elements.at(i))) {
            case Data_Value_Type::Object:   std::get<0>(Elm->Elements.at(i)) = new Obj_Element; break;
            case Data_Value_Type::Array:    std::get<0>(Elm->Elements.at(i)) = new Arr_Element; break;
            case Data_Value_Type::Bool:     std::get<0>(Elm->Elements.at(i)) = new Bol_Element; break;
            case Data_Value_Type::Number:   std::get<0>(Elm->Elements.at(i)) = new Num_Element; break;
            case Data_Value_Type::String:   std::get<0>(Elm->Elements.at(i)) = new Str_Element; break;
            case Data_Value_Type::Pointer:  std::get<0>(Elm->Elements.at(i)) = new Ptr_Element; break;
            }
            std::get<1>(Elm->Elements.at(i)) = std::get<1>(FElm->Elements.at(i));

            From.m_Branch->push_back({ std::get<0>(FElm->Elements.at(i)), std::get<1>(FElm->Elements.at(i)) });
            m_Branch->push_back({ std::get<0>(Elm->Elements.at(i)), std::get<1>(Elm->Elements.at(i)) });
            Copy_Tree(From);
        }

        From.m_Branch->pop_back();
        m_Branch->pop_back();
        return;

    } else if (FC_Type == Data_Value_Type::Object) {
        auto FElm = ((Obj_Element*)FC_Ptr);
        auto Elm = ((Obj_Element*)std::get<0>(m_Branch->back()));

        for (size_t i = 0; i < FElm->Elements.size(); i++) {
            Elm->Elements.emplace_back();

            switch (std::get<1>(FElm->Elements.at(i))) {
            case Data_Value_Type::Object:   std::get<0>(Elm->Elements.at(i)) = new Obj_Element; break;
            case Data_Value_Type::Array:    std::get<0>(Elm->Elements.at(i)) = new Arr_Element; break;
            case Data_Value_Type::Bool:     std::get<0>(Elm->Elements.at(i)) = new Bol_Element; break;
            case Data_Value_Type::Number:   std::get<0>(Elm->Elements.at(i)) = new Num_Element; break;
            case Data_Value_Type::String:   std::get<0>(Elm->Elements.at(i)) = new Str_Element; break;
            case Data_Value_Type::Pointer:  std::get<0>(Elm->Elements.at(i)) = new Ptr_Element; break;
            }
            std::get<1>(Elm->Elements.at(i)) = std::get<1>(FElm->Elements.at(i));
            std::get<2>(Elm->Elements.at(i)) = std::get<2>(FElm->Elements.at(i));

            From.m_Branch->push_back({ std::get<0>(FElm->Elements.at(i)), std::get<1>(FElm->Elements.at(i)) });
            m_Branch->push_back({ std::get<0>(Elm->Elements.at(i)), std::get<1>(Elm->Elements.at(i)) });
            Copy_Tree(From);
        }

        From.m_Branch->pop_back();
        m_Branch->pop_back();
        return;
    }

}
