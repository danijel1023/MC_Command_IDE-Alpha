#include "..//include/Json.h"
#include "defines.h"
#include <Windows.h>
#include <fstream>

inline bool is_number(wchar_t n) {
    if (n >= 48 && n <= 57) return true;
    else return false;
}

bool is_syntax_ch(wchar_t ch) {
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



namespace Json {
    Json::Json(const std::wstring& Path) {
        std::wifstream File(Path);
        if (!File.is_open()) {
            std::wcout << "Error opening the file";
        }

        File.seekg(0, std::ios::end);
        size_t size = File.tellg();
        File.seekg(0);

        std::wstring Str(size, ' ');
        File.read(&Str[0], size);

        size_t Str_Size = Str.size(), Last_i = 0;
        for (size_t i = 0; i < Str_Size; i++) {
            if (Str.at(i) == L'\n') {
                m_Raw_Data.push_back(Str.substr(Last_i, i - Last_i));
                Last_i = i + 1;
            }
        }

        //Run the initialization
        Init(m_Raw_Data);
    }

    Json::Json(const std::vector<std::wstring>& Raw_Data)
        :m_Raw_Data(Raw_Data) { Init(m_Raw_Data); }
    
    void Json::Init(const std::vector<std::wstring>& raw_data) {
        m_Data = Format_Data_Rspace(raw_data);
        std::wstring refactored_object_data = m_Data;
        auto strings = Format_Data_Rstr(refactored_object_data);

        log_info(m_Data);
        log_info(refactored_object_data);

        enum class type : wchar_t {
            Key,
            Value,
            Seperator,
        };

        type next_type = type::Value;
        type force_type = type::Value;
        bool key_has_data = false;
        bool value_has_data = false;

        size_t strings_last = 0;

        std::wstring object_structure;

        //checking only for syntax errors and building data structure/tree

        if (refactored_object_data.size() < 4) {
            m_Halt = true;
        }

        for (size_t wchar_t_i = 0; wchar_t_i < refactored_object_data.size(); wchar_t_i++) {
            wchar_t ch = refactored_object_data.at(wchar_t_i);

            if (next_type == type::Key) {
                if (ch == 'S') {
                    if (force_type == type::Seperator) {
                        JSON_FATAL_ERR(L"[FATAL ERROR] invalid syntax: after key type data must go semicolon \":\" - clearing tree - object halted");
                    }

                    key_has_data = true;
                    force_type = type::Seperator;

                } else if (ch == ':') {
                    if (!key_has_data) {
                        JSON_FATAL_ERR(L"[FATAL ERROR] invalid syntax: key type side does not contain any data - clearing tree - object halted");
                    }
                    key_has_data = false;


                    if (force_type != type::Seperator) {
                        JSON_FATAL_ERR(L"[FATAL ERROR] invalid syntax: ??? - value side - clearing tree - object halted");
                    }
                    force_type = type::Value;

                    next_type = type::Value;

                } else if (ch == '}') {
                    if (object_structure.back() != '{') {
                        JSON_FATAL_ERR(L"[FATAL ERROR] invalid syntax: opening and closing brackets don't match - clearing tree - object halted");
                    }
                    object_structure.pop_back();
                    force_type = type::Seperator;

                    m_Current_Obj_Elm.pop_back();
                } else {
                    JSON_FATAL_ERR(L"[FATAL ERROR] invalid syntax: ilegal wchar_t on key type side - clearing tree - object halted");
                }

            } else {

                if (ch == '{') {
                    if (force_type == type::Seperator) {
                        JSON_FATAL_ERR(L"[FATAL ERROR] invalid syntax: after value type data must go comma or closing bracket , / ] / } - clearing tree - object halted");
                    }
                    object_structure.push_back('{');
                    value_has_data = true;
                    next_type = type::Key;


                    //creating object
                    if (object_structure.size() == 1) {

                        std::get<0>(m_Root) = new ::Json_Private::_Data_Struct_Obj;
                        std::get<1>(m_Root) = ::Json_Private::Json_Type_::_Foo::_Object;

                        m_Current_Obj_Elm.push_back(m_Root);
                        log_info(L"Created root: " << std::get<0>(m_Root) << ", type: object");
                    }

                    else {

                        void* c_ptr = std::get<0>(m_Current_Obj_Elm.back());
                        ::Json_Private::_Data_Struct_Obj* new_elm = new ::Json_Private::_Data_Struct_Obj;

                        if (std::get<1>(m_Current_Obj_Elm.back()) == ::Json_Private::Json_Type_::_Foo::_Object) {
                            ((::Json_Private::_Data_Struct_Obj*)c_ptr)->Elements.push_back({ new_elm, ::Json_Private::Json_Type_::_Foo::_Object, strings.at(strings_last) });
                            log_info(L"Created element: " << new_elm << ", type: object, in: object element, key: \"" << strings.at(strings_last) << '"');
                            strings_last++;
                        } else {
                            ((::Json_Private::_Data_Struct_Arr*)c_ptr)->Elements.push_back({ new_elm, ::Json_Private::Json_Type_::_Foo::_Object });
                            log_info(L"Created element: " << new_elm << ", type: object, in: array element");
                        }
                        m_Current_Obj_Elm.push_back({ new_elm, ::Json_Private::Json_Type_::_Foo::_Object });
                    }

                }

                else if (ch == '[') {
                    if (force_type == type::Seperator) {
                        JSON_FATAL_ERR(L"[FATAL ERROR] invalid syntax: after value type data must go comma or closing bracket , / ] / } - clearing tree - object halted");
                    }
                    object_structure.push_back('[');
                    value_has_data = true;
                    next_type = type::Value;


                    //creating object
                    if (object_structure.size() == 1) {

                        std::get<0>(m_Root) = new ::Json_Private::_Data_Struct_Obj;
                        std::get<1>(m_Root) = ::Json_Private::Json_Type_::_Foo::_Array;

                        m_Current_Obj_Elm.push_back(m_Root);
                        log_info(L"Created root: " << std::get<0>(m_Root) << std::get<0>(m_Root) << ", type: object");
                    }

                    else {

                        void* c_ptr = std::get<0>(m_Current_Obj_Elm.back());
                        ::Json_Private::_Data_Struct_Arr* new_elm = new ::Json_Private::_Data_Struct_Arr;

                        if (std::get<1>(m_Current_Obj_Elm.back()) == ::Json_Private::Json_Type_::_Foo::_Object) {
                            ((::Json_Private::_Data_Struct_Obj*)c_ptr)->Elements.push_back({ new_elm, ::Json_Private::Json_Type_::_Foo::_Array, strings.at(strings_last) });
                            log_info(L"Created element: " << new_elm << ", type: array, in: object element, key: \"" << strings.at(strings_last) << '"');
                            strings_last++;
                        } else {
                            ((::Json_Private::_Data_Struct_Arr*)c_ptr)->Elements.push_back({ new_elm, ::Json_Private::Json_Type_::_Foo::_Array });
                            log_info(L"Created element: " << new_elm << ", type: array, in: array element");
                        }
                        m_Current_Obj_Elm.push_back({ new_elm, ::Json_Private::Json_Type_::_Foo::_Array });

                    }
                }

                else if (ch == '}') {
                    if (object_structure.back() != '{') {
                        JSON_FATAL_ERR(L"[FATAL ERROR] invalid syntax: opening and closing brackets don't match - clearing tree - object halted");
                    }
                    object_structure.pop_back();
                    force_type = type::Seperator;

                    m_Current_Obj_Elm.pop_back();
                }

                else if (ch == ']') {
                    if (object_structure.back() != '[') {
                        JSON_FATAL_ERR(L"[FATAL ERROR] invalid syntax: opening and closing brackets don't match - clearing tree - object halted");
                    }
                    object_structure.pop_back();
                    force_type = type::Seperator;

                    m_Current_Obj_Elm.pop_back();
                }

                else if (ch == ',') {
                    if (!value_has_data) {
                        JSON_FATAL_ERR(L"[FATAL ERROR] invalid syntax: value type side does not contain any data - clearing tree - object halted");
                    }
                    value_has_data = false;

                    if (force_type != type::Seperator) {
                        JSON_FATAL_ERR(L"[FATAL ERROR] invalid syntax: ??? - value side - comma - clearing tree - object halted");
                    }
                    force_type = type::Value;

                    if (object_structure.size() > 0) {
                        if (object_structure.back() == '{') {
                            next_type = type::Key;

                        } else {
                            next_type = type::Value;

                        }
                    }

                }

                else if (ch == 'S') {
                    if (force_type == type::Seperator) {
                        JSON_FATAL_ERR(L"[FATAL ERROR] invalid syntax: after value type data must go comma or closing bracket , / ] / } - clearing tree - object halted");
                    }
                    value_has_data = true;
                    force_type = type::Seperator;

                    if (object_structure.size() == 0) {

                        std::get<0>(m_Root) = new ::Json_Private::Str_element;
                        std::get<1>(m_Root) = ::Json_Private::Json_Type_::_Foo::_String;

                        m_Current_Obj_Elm.push_back(m_Root);
                        log_info(L"Created root: " << std::get<0>(m_Root) << ", type: string");
                    }
                    
                    else {

                        void* c_ptr = std::get<0>(m_Current_Obj_Elm.back());
                        ::Json_Private::Str_element* new_elm = new ::Json_Private::Str_element;

                        if (std::get<1>(m_Current_Obj_Elm.back()) == ::Json_Private::Json_Type_::_Foo::_Object) {
                            ((::Json_Private::_Data_Struct_Obj*)c_ptr)->Elements.push_back({ new_elm, ::Json_Private::Json_Type_::_Foo::_String, strings.at(strings_last) });
                            log_info(L"Created element: " << new_elm << ", type: string, in: object element, key: \"" << strings.at(strings_last) << "\", value: \"" << strings.at(strings_last + 1) << '"');
                            strings_last++;
                        } else {
                            ((::Json_Private::_Data_Struct_Arr*)c_ptr)->Elements.push_back({ new_elm, ::Json_Private::Json_Type_::_Foo::_String });
                            log_info(L"Created element: " << new_elm << ", type: string, in: array element, value: \"" << strings.at(strings_last) << '"');
                        }

                        new_elm->Value = strings.at(strings_last);
                        strings_last++;
                    }
                }

                else if (ch == 'N') {
                    if (force_type == type::Seperator) {
                        JSON_FATAL_ERR(L"[FATAL ERROR] invalid syntax: after value type data must go comma or closing bracket , / ] / } - clearing tree - object halted");
                    }
                    value_has_data = true;
                    force_type = type::Seperator;

                    if (object_structure.size() == 0) {

                        std::get<0>(m_Root) = new ::Json_Private::Num_element;
                        std::get<1>(m_Root) = ::Json_Private::Json_Type_::_Foo::_Number;

                        m_Current_Obj_Elm.push_back(m_Root);
                        log_info(L"Created root: " << std::get<0>(m_Root) << ", type: number");
                    }

                    else {
                        void* c_ptr = std::get<0>(m_Current_Obj_Elm.back());
                        ::Json_Private::Num_element* new_elm = new ::Json_Private::Num_element;

                        if (std::get<1>(m_Current_Obj_Elm.back()) == ::Json_Private::Json_Type_::_Foo::_Object) {
                            ((::Json_Private::_Data_Struct_Obj*)c_ptr)->Elements.push_back({ new_elm, ::Json_Private::Json_Type_::_Foo::_Number, strings.at(strings_last) });
                            log_info(L"Created element: " << new_elm << ", type: number, in: object element, key: \"" << strings.at(strings_last) << "\", value: " << strings.at(strings_last + 1));
                            strings_last++;
                        } else {
                            ((::Json_Private::_Data_Struct_Arr*)c_ptr)->Elements.push_back({ new_elm, ::Json_Private::Json_Type_::_Foo::_Number });
                            log_info(L"Created element: " << new_elm << ", type: number, in: array element, value: " << strings.at(strings_last));
                        }

                        new_elm->Value = To_intmax_t(strings.at(strings_last));
                        strings_last++;

                    }
                }

                else if (ch == 'D') {
                    if (force_type == type::Seperator) {
                        JSON_FATAL_ERR(L"[FATAL ERROR] invalid syntax: after value type data must go comma or closing bracket , / ] / } - clearing tree - object halted");
                    }
                    value_has_data = true;
                    force_type = type::Seperator;

                    if (object_structure.size() == 0) {

                        std::get<0>(m_Root) = new ::Json_Private::Dbl_element;
                        std::get<1>(m_Root) = ::Json_Private::Json_Type_::_Foo::_Double;

                        m_Current_Obj_Elm.push_back(m_Root);
                        log_info(L"Created root: " << std::get<0>(m_Root) << ", type: double");
                    }

                    else {
                        void* c_ptr = std::get<0>(m_Current_Obj_Elm.back());
                        ::Json_Private::Dbl_element* new_elm = new ::Json_Private::Dbl_element;

                        if (std::get<1>(m_Current_Obj_Elm.back()) == ::Json_Private::Json_Type_::_Foo::_Object) {
                            ((::Json_Private::_Data_Struct_Obj*)c_ptr)->Elements.push_back({ new_elm, ::Json_Private::Json_Type_::_Foo::_Double, strings.at(strings_last) });
                            log_info(L"Created element: " << new_elm << ", type: double, in: object element, key: \"" << strings.at(strings_last) << "\", value: " << strings.at(strings_last + 1));
                            strings_last++;
                        }
                        else {
                            ((::Json_Private::_Data_Struct_Arr*)c_ptr)->Elements.push_back({ new_elm, ::Json_Private::Json_Type_::_Foo::_Double });
                            log_info(L"Created element: " << new_elm << ", type: double, in: array element, value: " << strings.at(strings_last));
                        }

                        new_elm->Value = To_Double(strings.at(strings_last));
                        strings_last++;

                    }
                }

                else if (ch == 'B') {
                    if (force_type == type::Seperator) {
                        JSON_FATAL_ERR(L"[FATAL ERROR] invalid syntax: after value type data must go comma or closing bracket , / ] / } - clearing tree - object halted");
                    }
                    value_has_data = true;
                    force_type = type::Seperator;

                    if (object_structure.size() == 0) {

                        std::get<0>(m_Root) = new ::Json_Private::Bol_element;
                        std::get<1>(m_Root) = ::Json_Private::Json_Type_::_Foo::_Bool;

                        m_Current_Obj_Elm.push_back(m_Root);
                        log_info(L"Created root: " << std::get<0>(m_Root) << ", type: bool");
                    }

                    else {
                        void* c_ptr = std::get<0>(m_Current_Obj_Elm.back());
                        ::Json_Private::Bol_element* new_elm = new ::Json_Private::Bol_element;

                        if (std::get<1>(m_Current_Obj_Elm.back()) == ::Json_Private::Json_Type_::_Foo::_Object) {
                            ((::Json_Private::_Data_Struct_Obj*)c_ptr)->Elements.push_back({ new_elm, ::Json_Private::Json_Type_::_Foo::_Bool, strings.at(strings_last) });
                            log_info(L"Created element: " << new_elm << ", type: bool, in: object element, key: \"" << strings.at(strings_last) << "\", value: \"" << strings.at(strings_last + 1) << '"');
                            strings_last++;
                        } else {
                            ((::Json_Private::_Data_Struct_Arr*)c_ptr)->Elements.push_back({ new_elm, ::Json_Private::Json_Type_::_Foo::_Bool });
                            log_info(L"Created element: " << new_elm << ", type: bool, in: array element, value: \"" << strings.at(strings_last) << '"');
                        }

                        if (strings.at(strings_last) == L"true") {
                            new_elm->Value = true;
                        } else  if (strings.at(strings_last) == L"false") {
                            new_elm->Value = false;

                        } else {
                            JSON_FATAL_ERR(L"[FATAL ERROR] Invalid syntax: Trying to write non bool value to bool. - clearing tree - object halted");
                        }

                        strings_last++;
                    }
                }

                else {
                    JSON_FATAL_ERR(L"[FATAL ERROR] Invalid syntax: Ilegal wchar_t on value type side. - clearing tree - object halted");
                }
            }
        }

        if (object_structure.size() != 0) {
            JSON_FATAL_ERR(L"[FATAL ERROR] invalid syntax: opening and closing brackets don't match - clearing tree - object halted");
        }

        Root();
    }

    Json& Json::Obj(std::wstring index) {
        if (m_Halt) return *this;
        if (m_Error_Reading_Tree) return *this;

        m_get_str_tree_is_root = false;
        m_get_str_tree_last_type = ::Json_Private::Json_Type_::_Foo::_Object;

        void* l_elm = std::get<0>(m_Current_Obj_Elm.back());

        if (std::get<1>(m_Current_Obj_Elm.back()) == ::Json_Private::Json_Type_::_Foo::_Object) {
            auto c_elm = ((::Json_Private::_Data_Struct_Obj*)l_elm);
            void* n_elm = nullptr;
            for (size_t i = 0; i < c_elm->Elements.size(); i++) {
                if (std::get<2>(c_elm->Elements.at(i)) == index) {
                    n_elm = std::get<0>(c_elm->Elements.at(i));
                    m_Current_Obj_Elm.push_back({ n_elm, std::get<1>(c_elm->Elements.at(i)) });
                    m_get_str_tree_last_ptr = c_elm;
                    m_get_str_tree_last_index = i;
                    break;
                }
            }

            if (n_elm == nullptr) {
                log_err(L"[Tree search - ERROR]: Could not find element: " << index);
                m_Error_Reading_Tree = true;
                //[TODO]: could not find element
            }

        } else {
            log_err(L"[Tree search - ERROR]: Trying to acces non-object data.");
            m_Error_Reading_Tree = true;
            //[TODO]: error: trying to acces non-object data
        }

        return *this;
    }

    Json& Json::Arr(size_t index) {
        if (m_Halt) return *this;
        if (m_Error_Reading_Tree) return *this;

        m_get_str_tree_is_root = false;
        m_get_str_tree_last_type = ::Json_Private::Json_Type_::_Foo::_Array;

        void* l_elm = std::get<0>(m_Current_Obj_Elm.back());

        if (std::get<1>(m_Current_Obj_Elm.back()) == ::Json_Private::Json_Type_::_Foo::_Array) {
            auto obj_ = ((::Json_Private::_Data_Struct_Arr*)l_elm);
            void* c_elm = nullptr;

            if (index < ((::Json_Private::_Data_Struct_Arr*)l_elm)->Elements.size()) {
                c_elm = std::get<0>(((::Json_Private::_Data_Struct_Arr*)l_elm)->Elements.at(index));
                m_Current_Obj_Elm.push_back({ c_elm, std::get<1>(((::Json_Private::_Data_Struct_Arr*)l_elm)->Elements.at(index)) });
            }
            
            if (c_elm == nullptr) {
                log_err(L"[Tree search - ERROR]: Trying to acces nonexistent data.");
                m_Error_Reading_Tree = true;
                //[TODO]: trying to acces nonexistent data
            }

        } else {
            log_err(L"[Tree search - ERROR]: Trying to acces non-array data.");
            m_Error_Reading_Tree = true;
            //[TODO]: error: trying to acces non-array data
        }

        return *this;
    }

    size_t Json::Size() {
        if (m_Halt) return 0;
        if (m_Error_Reading_Tree) {
            m_Error_Reading_Tree = false;
            return 0;
        }

        void* c_elm = std::get<0>(m_Current_Obj_Elm.back());
        auto c_type = std::get<1>(m_Current_Obj_Elm.back());

        if (c_type == ::Json_Private::Json_Type_::_Foo::_Array) {
            auto elm = ((::Json_Private::_Data_Struct_Arr*)c_elm);
            
            return elm->Elements.size();
        }
        else if (c_type == ::Json_Private::Json_Type_::_Foo::_Object) {
            auto elm = ((::Json_Private::_Data_Struct_Obj*)c_elm);
            
            return elm->Elements.size();
        }
        else {
            return 0;
        }
    }

    std::wstring Json::Str() {
        if (m_Halt) return L"";
        if (m_Error_Reading_Tree) {
            m_Error_Reading_Tree = false;
            return L"";
        }

        void* l_elm = std::get<0>(m_Current_Obj_Elm.back());

        if (std::get<1>(m_Current_Obj_Elm.back()) == ::Json_Private::Json_Type_::_Foo::_String) {
            log_info(L"Getting string");
            auto str = ((::Json_Private::Str_element*)l_elm);

            return str->Value;
        } else {
            //[TODO]: error: element isn't string type
            log_err(L"[ERROR]: Element that you're trying to acces isn't string type. No string returned.");
            __debugbreak();
            return L"";
        }
    }

    intmax_t Json::Num() {
        if (m_Halt) return 0;
        if (m_Error_Reading_Tree) {
            m_Error_Reading_Tree = false;
            return 0;
        }

        void* l_elm = std::get<0>(m_Current_Obj_Elm.back());

        if (std::get<1>(m_Current_Obj_Elm.back()) == ::Json_Private::Json_Type_::_Foo::_Number) {
            log_info(L"Getting number");
            auto num = ((::Json_Private::Num_element*)l_elm);

            return num->Value;
        } else {
            //[TODO]: error: element isn't number type
            log_err(L"[ERROR]: Element that you're trying to acces isn't number type. Returned default.");
            __debugbreak();
            return 0;
        }
    }

    long double Json::Dbl() {
        if (m_Halt) return 0.0;
        if (m_Error_Reading_Tree) {
            m_Error_Reading_Tree = false;
            return 0.0;
        }

        void* l_elm = std::get<0>(m_Current_Obj_Elm.back());

        if (std::get<1>(m_Current_Obj_Elm.back()) == ::Json_Private::Json_Type_::_Foo::_Double) {
            log_info(L"Getting double");
            auto num = ((::Json_Private::Dbl_element*)l_elm);

            return num->Value;
        }
        else {
            //[TODO]: error: element isn't number type
            log_err(L"[ERROR]: Element that you're trying to acces isn't double type. Returned default.");
            __debugbreak();
            return 0.0;
        }
    }

    bool Json::Bol() {
        if (m_Halt) return false;
        if (m_Error_Reading_Tree) {
            m_Error_Reading_Tree = false;
            return false;
        }

        void* l_elm = std::get<0>(m_Current_Obj_Elm.back());

        if (std::get<1>(m_Current_Obj_Elm.back()) == ::Json_Private::Json_Type_::_Foo::_Bool) {
            log_info(L"Getting bool");
            auto num = ((::Json_Private::Bol_element*)l_elm);

            return num->Value;
        } else {
            //[TODO]: error: element isn't number type
            log_err(L"[ERROR]: Element that you're trying to acces isn't bool type. No bool returned.");
            __debugbreak();
            return false;
        }
    }

    std::vector<std::wstring> Json::Keys() {
        std::vector<std::wstring> ret;

        if (m_Halt) return ret;
        if (m_Error_Reading_Tree) {
            m_Error_Reading_Tree = false;
            return ret;
        }

        void* l_elm = std::get<0>(m_Current_Obj_Elm.back());
        auto l_type = std::get<1>(m_Current_Obj_Elm.back());

        if (l_type == ::Json_Private::Json_Type_::_Foo::_Object) {
            log_trace(L"Getting keys");

            size_t obj_size = Size();
            for (size_t i = 0; i < obj_size; i++) {
                auto key = std::get<2>(((::Json_Private::_Data_Struct_Obj*)l_elm)->Elements.at(i));
                ret.push_back(key);

                log_trace(L"Key: " << key);
            }

            return ret;
        }
        else {
            //[TODO]: error: element isn't number type
            log_err(L"[ERROR]: Element that you're trying to acces isn't bool type. No bool returned.");
            __debugbreak();
            return ret;
        }
    }


    std::wstring Json::Format_Data_Rspace(const std::vector<std::wstring>& raw_data) {
        std::wstring ret;

        for (size_t line_i = 0; line_i < raw_data.size(); line_i++) {
            bool skip_wchar_t = false;
            bool is_str = false;
            int fs = 0;

            std::wstring line_str = raw_data.at(line_i);
            for (size_t ch_i = 0; ch_i < line_str.size(); ch_i++) {
                wchar_t ch = line_str.at(ch_i);
                
                if (ch == '/' && !is_str) {
                    if (ch_i + 1 < line_str.size()) {
                        if (line_str.at(ch_i + 1) == '/') {
                            break; //start of comment - skip to next line
                        }
                    }
                }
                if (skip_wchar_t) {
                    if (is_str) {
                        skip_wchar_t = false;
                        ret += ch;
                    }
                } else if (ch == '\\') {
                    if (is_str) {
                        skip_wchar_t = true;
                        ret += ch;
                    }
                    else {
                        //[TODO]: invalid syntax
                        log_err(L"[ERROR]: invalid syntax: found ilegal wchar_t outside string");
                        __debugbreak();
                    }
                } else if (ch == '"') {
                    is_str = !is_str;
                    ret += ch;

                } else if ((ch == ' ' || ch == '    ') && !is_str) {
                    //do nothing (ignore)
                } else {
                    ret += ch;
                }
            }

            if (is_str) {
                log_err(L"[ERROR]: invalid syntax: string is incomplete");
                __debugbreak();
            }
        }

        return ret;
    }

    std::vector<std::wstring> Json::Format_Data_Rstr(std::wstring& data) {
        bool is_str = false;
        bool is_num = false;
        bool is_double = false;
        size_t is_str_count = 0;
        size_t is_num_count = 0;
        bool ignore_ch = false;
        std::vector<std::wstring> strings;



        for (size_t i = 0; i < data.size(); i++) {
            wchar_t ch = data.at(i);

            if (is_str) {
                is_str_count++;
                if (ignore_ch) {
                    ignore_ch = false;
                    strings.back() += ch;

                } else if (ch == '\\') {
                    ignore_ch = true;

                } else if (ch == '"') {
                    is_str = false;

                    size_t offset = i - is_str_count + 1;
                    data.erase(offset, is_str_count);
                    data.insert(offset,L"S");
                    i = offset;

                    is_str_count = 0;

                } else {
                    strings.back() += ch;
                }
            }

            else if (is_num) {
                if (ch == ',' || ch == ']' || ch == '}') {
                    size_t offset = i - is_num_count;
                    data.erase(offset, is_num_count);
                    if (!is_double) data.insert(offset, L"N");
                    else data.insert(offset, L"D");
                    i = offset + 1;

                    is_num_count = 0;

                    is_num = false;
                    is_double = false;

                } else if (is_number(ch)) {
                    is_num_count++;
                    strings.back() += ch;
                } else if (ch == '.' && !is_double) {
                    is_double = true;
                    is_num_count++;
                    strings.back() += ch;
                } else {
                    log_err(L"[ERROR] Invalid syntax: number ends with ilegal wchar_t: " << ch);
                    size_t i3 = 0;
                    for (size_t i2 = 0; i2 < i + 1; i2++) {
                        if (data.at(i2) == 'S' || data.at(i3) == 'N') {
                            if (data.at(i2) == 'S') std::cout << '"';
                            //std::cout << strings.at(i3).substr(1, strings.at(i3).size() - 1);
                            if (data.at(i2) == 'S') std::cout << '"';
                            i3++;
                        } else {
                            std::cout << data.at(i2);
                        }
                    }
                    std::cout << std::endl;
                    __debugbreak();
                }
            }

            else if (ch == '"') {
                is_str_count++;
                strings.emplace_back();
                is_str = true;
            }

            else if (is_number(ch)) {
                is_num_count++;
                strings.emplace_back();
                strings.back() += ch;
                is_num = true;
            } else if (!is_syntax_ch(ch)) {
                if (intmax_t(data.size()) - i >= 5) {
                    if (data.substr(i, 5) == L"false") {
                        data.erase(i, 5);
                        data.insert(i, L"B");

                        strings.push_back(L"false");
                        continue;
                    } else if (data.substr(i, 4) == L"true") {
                        data.erase(i, 4);
                        data.insert(i, L"B");

                        strings.push_back(L"true");
                        continue;
                    }
                } else if (intmax_t(data.size()) - i >= 5) {
                    if (data.substr(i, 4) == L"true") {
                        data.erase(i, 4);
                        data.insert(i, L"B");

                        strings.push_back(L"true");
                        continue;
                    }
                }


                log_err(L"[ERROR] Invalid syntax: ilegal wchar_tacter outside of string: " << ch);

                size_t i3 = 0;
                for (size_t i2 = 0; i2 < i + 1; i2++) {
                    if (data.at(i2) == 'S' || data.at(i3) == 'N') {
                        if (data.at(i2) == 'S') std::cout << '"';
                        //std::cout << strings.at(i3).substr(1, strings.at(i3).size() - 1);
                        if (data.at(i2) == 'S') std::cout << '"';
                        i3++;
                    } else {
                        std::cout << data.at(i2);
                    }
                }
                std::cout << std::endl;

                __debugbreak();
            }
        }

        return strings;
    }


    intmax_t Json::To_intmax_t(const std::wstring& str_number) {
        intmax_t ret = 0;

        for (size_t i = 0; i < str_number.size(); i++) {
            if (!is_number(str_number.at(i))) {
                log_err(L"[ERROR] invalid syntax: foud non-number wchar_t in number string (L" << str_number << "): " << str_number.at(i));
                __debugbreak();
                return 0;
            }

            intmax_t to_num = (intmax_t)(str_number.at(i)) - 48;

            ret = ret * 10 + to_num;
        }

        return ret;
    }

    long double Json::To_Double(const std::wstring& str_number) {
        long double ret = 0;
        bool is_decimal = false;
        long double decimal = 0.0;
        size_t dec_offset = 0;

        for (size_t i = 0; i < str_number.size(); i++) {
            size_t inverted_i = str_number.size() - i + dec_offset;     // -1
            if (!is_number(str_number.at(i))) {
                if (str_number.at(i) == '.') {
                    is_decimal = true;
                    dec_offset = i;     // +1
                }

                else {
                    log_err(L"[ERROR] invalid syntax: foud non-number wchar_t in number string (L" << str_number << "): " << str_number.at(i));
                    __debugbreak();
                    return 0;
                }
            }

            else {
                if (!is_decimal) {
                    intmax_t to_num = (intmax_t)(str_number.at(i)) - 48;
                    ret = ret * 10 + to_num;

                }

                else {
                    intmax_t to_num = (intmax_t)(str_number.at(inverted_i)) - 48;
                    decimal = decimal / 10 + to_num;

                }
            }
        }

        ret += decimal / 10;

        return ret;
    }

    Json::~Json() {
        if (!m_Deleted_Tree) {
            Root();
            m_Current_Obj_Elm.push_back(m_Root);

            Delete_Tree();
            m_Deleted_Tree = true;
        } else {
            log_warn(L"[WARNING] the Json object has been already deleted: no action taken anymore.");
        }
    }

    void Json::Delete_Tree(size_t index) {
        void* c_ptr = std::get<0>(m_Current_Obj_Elm.back());
        auto c_type = std::get<1>(m_Current_Obj_Elm.back());
        if (c_ptr == nullptr) return;

        if (c_type == ::Json_Private::Json_Type_::_Foo::_Number) {
            m_Current_Obj_Elm.pop_back();
            auto elm = ((::Json_Private::Num_element*)c_ptr);

            if (m_Current_Obj_Elm.size() != 0) {
                void* last_ptr = std::get<0>(m_Current_Obj_Elm.back());
                auto last_type = std::get<1>(m_Current_Obj_Elm.back());

                if (last_type == ::Json_Private::Json_Type_::_Foo::_Object) {
                    auto last_elm = ((::Json_Private::_Data_Struct_Obj*)last_ptr);
                    log_info(L"Deleting number element: " << elm << ", value: " << elm->Value << ", key: \"" << std::get<2>(last_elm->Elements.at(index)) << '"');
                } else {
                    log_info(L"Deleting number element: " << elm << ", value: " << elm->Value << ", index:" << index);
                }

            } else log_info(L"Deleting number element: " << elm << ", value: " << elm->Value);
            
            delete elm;
            return;

        } else if (c_type == ::Json_Private::Json_Type_::_Foo::_Double) {
            m_Current_Obj_Elm.pop_back();
            auto elm = ((::Json_Private::Dbl_element*)c_ptr);

            if (m_Current_Obj_Elm.size() != 0) {
                void* last_ptr = std::get<0>(m_Current_Obj_Elm.back());
                auto last_type = std::get<1>(m_Current_Obj_Elm.back());

                if (last_type == ::Json_Private::Json_Type_::_Foo::_Object) {
                    auto last_elm = ((::Json_Private::_Data_Struct_Obj*)last_ptr);
                    log_info(L"Deleting double element: " << elm << ", value: " << elm->Value << ", key: \"" << std::get<2>(last_elm->Elements.at(index)) << '"');
                }
                else {
                    log_info(L"Deleting double element: " << elm << ", value: " << elm->Value << ", index:" << index);
                }

            }
            else log_info(L"Deleting double element: " << elm << ", value: " << elm->Value);

            delete elm;
            return;

        } else if (c_type == ::Json_Private::Json_Type_::_Foo::_String) {
            m_Current_Obj_Elm.pop_back();
            auto elm = ((::Json_Private::Str_element*)c_ptr);

            if (m_Current_Obj_Elm.size() != 0) {
                void* last_ptr = std::get<0>(m_Current_Obj_Elm.back());
                auto last_type = std::get<1>(m_Current_Obj_Elm.back());

                if (last_type == ::Json_Private::Json_Type_::_Foo::_Object) {
                    auto last_elm = ((::Json_Private::_Data_Struct_Obj*)last_ptr);
                    log_info(L"Deleting string element: " << elm << ", value: " << elm->Value << ", key: \"" << std::get<2>(last_elm->Elements.at(index)) << '"');
                } else {
                    log_info(L"Deleting string element: " << elm << ", value: " << elm->Value << ", index:" << index);
                }

            }
            else log_info(L"Deleting string element: " << elm << ", value: " << elm->Value);

            delete elm;
            return;

        } else if (c_type == ::Json_Private::Json_Type_::_Foo::_Bool) {
            m_Current_Obj_Elm.pop_back();
            auto elm = ((::Json_Private::Bol_element*)c_ptr);

            if (m_Current_Obj_Elm.size() != 0) {
                void* last_ptr = std::get<0>(m_Current_Obj_Elm.back());
                auto last_type = std::get<1>(m_Current_Obj_Elm.back());

                if (last_type == ::Json_Private::Json_Type_::_Foo::_Object) {
                    auto last_elm = ((::Json_Private::_Data_Struct_Obj*)last_ptr);
                    log_info(L"Deleting bool element: " << elm << ", value: " << ((elm->Value) ? "true" : "false") << ", key: \"" << std::get<2>(last_elm->Elements.at(index)) << '"');
                } else {
                    log_info(L"Deleting bool element: " << elm << ", value: " << ((elm->Value) ? "true" : "false") << ", index:" << index);
                }

            } else log_info(L"Deleting bool element: " << elm << ", value: " << ((elm->Value) ? "true" : "false"));

            delete elm;
            return;

        }

        else if (c_type == ::Json_Private::Json_Type_::_Foo::_Object) {
            auto elm = ((::Json_Private::_Data_Struct_Obj*)c_ptr);

            for (size_t i = 0; i < elm->Elements.size(); i++) {
                m_Current_Obj_Elm.push_back({ (std::get<0>(elm->Elements.at(i))), (std::get<1>(elm->Elements.at(i))) });
                Delete_Tree(i);
            }
            m_Current_Obj_Elm.pop_back();
            log_info(L"Deleting object element: " << c_ptr);
            delete elm;
            return;

        }
        else  if (c_type == ::Json_Private::Json_Type_::_Foo::_Array) {
            auto elm = ((::Json_Private::_Data_Struct_Arr*)c_ptr);

            for (size_t i = 0; i < elm->Elements.size(); i++) {
                m_Current_Obj_Elm.push_back({ (std::get<0>(elm->Elements.at(i))), (std::get<1>(elm->Elements.at(i))) });
                Delete_Tree(i);
            }
            m_Current_Obj_Elm.pop_back();
            log_info(L"Deleting array element: " << c_ptr);
            delete elm;
            return;

        }
    }

    std::wstring Json::Get_Tree() {
        if (m_Halt) return L"";
        if (m_Error_Reading_Tree) {
            m_Error_Reading_Tree = false;
            return L"";
        }

        std::wstring a = Get_Str_Tree().str();

        //reset after printing
        m_print_tree_index_S.clear();
        m_print_tree_index = 0;
        m_get_str_tree_disable_is_root = false;

        return a;
    }

//#define __trial

#ifdef __trial
    #define ret_cout ret
#else
    #ifdef __RELEASE
        #define ret_cout ret
    #else
        #define ret_cout std::wcout
    #endif
#endif

    std::wstringstream Json::Get_Str_Tree() {
        std::wstringstream ret;

        void* c_ptr = std::get<0>(m_Current_Obj_Elm.back());
        auto c_type = std::get<1>(m_Current_Obj_Elm.back());

        if (!m_get_str_tree_is_root && !m_get_str_tree_disable_is_root) {
            if (m_get_str_tree_last_type == ::Json_Private::Json_Type_::_Foo::_Object) {
                auto elm = ((::Json_Private::_Data_Struct_Obj*)m_get_str_tree_last_ptr);
                ret_cout << "Key: \"" << (std::get<2>(elm->Elements.at(m_get_str_tree_last_index))) << '"' << std::endl;

            } else {
                ret_cout << "Index: " << m_get_str_tree_last_index << std::endl;

            }
            m_get_str_tree_disable_is_root = true;
        }

        if (m_Current_Obj_Elm.size() == 1) {
            ret_cout << "Root: ";

            switch (c_type) {
            case ::Json_Private::Json_Type_::_Foo::_Object: ret_cout << "object"; break;
            case ::Json_Private::Json_Type_::_Foo::_Array: ret_cout << "array"; break;
            case ::Json_Private::Json_Type_::_Foo::_Bool: ret_cout << "bool"; break;
            case ::Json_Private::Json_Type_::_Foo::_Double: ret_cout << "double"; break;
            case ::Json_Private::Json_Type_::_Foo::_Number: ret_cout << "number"; break;
            case ::Json_Private::Json_Type_::_Foo::_String: ret_cout << "string"; break;
            }

            ret_cout << std::endl;
        }


        if (c_type == ::Json_Private::Json_Type_::_Foo::_Object) {
            auto elm = ((::Json_Private::_Data_Struct_Obj*)c_ptr);

            for (size_t i = 0; i < elm->Elements.size(); i++) {
                m_Current_Obj_Elm.push_back({ (std::get<0>(elm->Elements.at(i))), (std::get<1>(elm->Elements.at(i))) });

                for (size_t i = 0; i < m_print_tree_index * 2; i++) {
                    if (i % 2 == 0) {
                        ret_cout << m_print_tree_index_S.at(i / 2);
                    }

                    else {
                        ret_cout << "   ";
                    }
                }

                m_print_tree_index++;
                if (i + 1 == elm->Elements.size()) {
                    ret_cout << wchar_t(192);
                    m_print_tree_index_S.push_back(' ');
                } else {
                    ret_cout << wchar_t(195);
                    m_print_tree_index_S.push_back(wchar_t(179));
                }

                for (wchar_t i = 0; i < 3; i++) ret_cout << wchar_t(196);
                ret_cout << "Key: \"" << (std::get<2>(elm->Elements.at(i))) << '"' << std::endl;

                ret_cout << Get_Str_Tree().str();
            }
            m_print_tree_index--;
            if (m_print_tree_index_S.size() > 0) m_print_tree_index_S.pop_back();
            m_Current_Obj_Elm.pop_back();

            return ret;
        }

        else if (c_type == ::Json_Private::Json_Type_::_Foo::_Array) {
            auto elm = ((::Json_Private::_Data_Struct_Arr*)c_ptr);

            for (size_t i = 0; i < elm->Elements.size(); i++) {
                m_Current_Obj_Elm.push_back({ (std::get<0>(elm->Elements.at(i))), (std::get<1>(elm->Elements.at(i))) });

                for (size_t i = 0; i < m_print_tree_index * 2; i++) {
                    if (i % 2 == 0) {
                        ret_cout << m_print_tree_index_S.at(i / 2);
                    }

                    else {
                        ret_cout << "   ";
                    }
                }

                m_print_tree_index++;
                if (i + 1 == elm->Elements.size()) {
                    ret_cout << wchar_t(192);
                    m_print_tree_index_S.push_back(' ');
                } else {
                    ret_cout << wchar_t(195);
                    m_print_tree_index_S.push_back(wchar_t(179));
                }

                for (wchar_t i = 0; i < 3; i++) ret_cout << wchar_t(196);
                ret_cout << "Index: " << i << std::endl;

                ret_cout << Get_Str_Tree().str();
            }
            m_print_tree_index--;
            if (m_print_tree_index_S.size() > 0) m_print_tree_index_S.pop_back();
            m_Current_Obj_Elm.pop_back();

            return ret;
        }

        else if (c_type == ::Json_Private::Json_Type_::_Foo::_Number) {
            m_Current_Obj_Elm.pop_back();
            auto elm = ((::Json_Private::Num_element*)c_ptr);

            for (size_t i = 0; i < m_print_tree_index * 2; i++) {
                if (i % 2 == 0) {
                    ret_cout << m_print_tree_index_S.at(i / 2);
                }

                else {
                    ret_cout << "   ";
                }
            }

            ret_cout << wchar_t(192);
            for (wchar_t i = 0; i < 3; i++) ret_cout << wchar_t(196);
            ret_cout << "Number: " << elm->Value << std::endl;

            m_print_tree_index--;
            if (m_print_tree_index_S.size() > 0) m_print_tree_index_S.pop_back();
            return ret;

        }

        else if (c_type == ::Json_Private::Json_Type_::_Foo::_Double) {
            m_Current_Obj_Elm.pop_back();
            auto elm = ((::Json_Private::Dbl_element*)c_ptr);

            for (size_t i = 0; i < m_print_tree_index * 2; i++) {
                if (i % 2 == 0) {
                    ret_cout << m_print_tree_index_S.at(i / 2);
                }

                else {
                    ret_cout << "   ";
                }
            }

            ret_cout << wchar_t(192);
            for (wchar_t i = 0; i < 3; i++) ret_cout << wchar_t(196);
            ret_cout << "Double: " << (elm->Value) << std::endl;

            m_print_tree_index--;
            if (m_print_tree_index_S.size() > 0) m_print_tree_index_S.pop_back();
            return ret;

        }

        else if (c_type == ::Json_Private::Json_Type_::_Foo::_String) {
            m_Current_Obj_Elm.pop_back();
            auto elm = ((::Json_Private::Str_element*)c_ptr);

            for (size_t i = 0; i < m_print_tree_index * 2; i++) {
                if (i % 2 == 0) {
                    ret_cout << m_print_tree_index_S.at(i / 2);
                }

                else {
                    ret_cout << "   ";
                }
            }

            ret_cout << wchar_t(192);
            for (wchar_t i = 0; i < 3; i++) ret_cout << wchar_t(196);
            ret_cout << "String: \"" << elm->Value << '\"' << std::endl;

            m_print_tree_index--;
            if (m_print_tree_index_S.size() > 0) m_print_tree_index_S.pop_back();
            return ret;

        } else {
            m_Current_Obj_Elm.pop_back();
            auto elm = ((::Json_Private::Bol_element*)c_ptr);

            for (size_t i = 0; i < m_print_tree_index * 2; i++) {
                if (i % 2 == 0) {
                    ret_cout << m_print_tree_index_S.at(i / 2);
                }

                else {
                    ret_cout << "   ";
                }
            }

            ret_cout << wchar_t(192);
            for (wchar_t i = 0; i < 3; i++) ret_cout << wchar_t(196);
            ret_cout << "Bool: " << ((elm->Value) ? "true" : "false") << std::endl;

            m_print_tree_index--;
            if (m_print_tree_index_S.size() > 0) m_print_tree_index_S.pop_back();
            return ret;

        }

    }

}
