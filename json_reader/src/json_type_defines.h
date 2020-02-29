#pragma once

#include <string>
#include <vector>
#include <tuple>


enum class Data_Value_Type : char {
    Array,
    Object,
    String,
    Number,
    Bool,
    Pointer     //Only for "null"
};

//Structure for basic type
using Element_Struct = std::tuple<void*, Data_Value_Type>;

//Structure for type 'array'
struct Element_Struct_TypeArr {
    std::vector<std::tuple<void*, Data_Value_Type>> Elements;
};

//Structure for type 'object'
struct Element_Struct_TypeObj {
    std::vector<std::tuple<void*, Data_Value_Type, std::wstring>> Elements;
};


using Arr_Element = Element_Struct_TypeArr;
using Obj_Element = Element_Struct_TypeObj;
using Str_Element = std::wstring;
using Num_Element = long double;
using Bol_Element = bool;
using Ptr_Element = std::nullptr_t;
