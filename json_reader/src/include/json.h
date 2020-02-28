#pragma once
#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <sstream>

#include "..//json//defines.h"

#include "..//json//json_private.h"

namespace Json {

    class Json {
    public:
        Json(const std::vector<std::wstring>& Raw_Data);
        Json(const std::wstring& Path);
        Json() {}
        ~Json();

        Json& Obj(std::wstring index);
        Json& Arr(size_t index);
        Json& Root() {
            if (m_Halt) return *this;
            m_Current_Obj_Elm.clear();
            m_Current_Obj_Elm.push_back(m_Root);
            m_get_str_tree_is_root = true;
            return *this;
        }
        Json& Back() {
            if (m_Halt) return *this;
            if (m_Current_Obj_Elm.size() > 1) m_Current_Obj_Elm.pop_back();
            if (m_Current_Obj_Elm.size() == 1) {
                m_get_str_tree_is_root = true;
            }
            return *this;
        }

        size_t Size();
        std::wstring Str();
        intmax_t Num();
        long double Dbl();
        bool Bol();
        std::vector<std::wstring> Keys();

        bool Is_Str() { return Is_type(::Json_Private::Json_Type_::_Foo::_String); }
        bool Is_Num() { return Is_type(::Json_Private::Json_Type_::_Foo::_Number); }
        bool Is_Dbl() { return Is_type(::Json_Private::Json_Type_::_Foo::_Double); }
        bool Is_Bol() { return Is_type(::Json_Private::Json_Type_::_Foo::_Bool); }
        bool Is_Arr() { return Is_type(::Json_Private::Json_Type_::_Foo::_Array); }
        bool Is_Obj() { return Is_type(::Json_Private::Json_Type_::_Foo::_Object); }

        bool Is_type(::Json_Private::Json_Type_::_Foo type) {
            if (m_Current_Obj_Elm.size() > 0) {
                if (std::get<1>(m_Current_Obj_Elm.back()) == type)
                    return true;
            }
            return false;
        }

        std::wstring Get_Tree();
        std::wstring Get_Err() {
            std::wstring Err = Errors.str();
            Errors.str(L"");
            return Err;
        }

        std::wstring Format_Data_Rspace(const std::vector<std::wstring>& raw_data);
        std::vector<std::wstring> Format_Data_Rstr(std::wstring& data);

        long double To_Double(const std::wstring& str_number);
        intmax_t To_intmax_t(const std::wstring& str_number);
    private:
        void Init(const std::vector<std::wstring>& raw_data);

        bool m_Halt = false;
        bool m_Error_Reading_Tree = false;

        std::tuple<void*, Json_Private::Json_Type_::_Foo> m_Root;  //root of file (contains pointer to first obj)
        std::vector<std::tuple<void*, Json_Private::Json_Type_::_Foo>> m_Current_Obj_Elm;  //contains tree list of pointers

        std::vector<std::wstring> m_Raw_Data;  //data from file
        std::wstring m_Data;  //formated data


        void Delete_Tree(size_t index = 0);
        bool m_Deleted_Tree = false;
        
        
        std::wstringstream Get_Str_Tree();
        bool m_get_str_tree_is_root = true;
        bool m_get_str_tree_disable_is_root = false;
        void* m_get_str_tree_last_ptr = nullptr;
        size_t m_get_str_tree_last_index = 0;
        Json_Private::Json_Type_::_Foo m_get_str_tree_last_type = Json_Private::Json_Type_::_Foo::_Array;
        size_t m_print_tree_index = 0;
        std::vector<char> m_print_tree_index_S;

        std::wstringstream Errors;
    };

}
