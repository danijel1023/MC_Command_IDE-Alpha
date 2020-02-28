#pragma once
#include <string>
#include <vector>

namespace Json_Private {
    namespace Json_Type_ {
        enum class _Foo : char {
            _Array,
            _String,
            _Number,
            _Object,
            _Bool,
            _Double,

        };
    }

    namespace Json_Object_ {
        enum class _foo : char {
            _Null,
            _Object,
            _Array,
        };
    }

    struct Bol_element {
        bool Value = false;
    };
    struct Num_element {
        intmax_t Value = 0;
    };
    struct Dbl_element {
        long double Value = 0.0;
    };
    struct Str_element {
        std::wstring Value;
    };


    struct _Data_Struct_Obj {
        //key, type, poiner to type
        std::vector<std::tuple<void*, Json_Private::Json_Type_::_Foo, std::wstring>> Elements;

    };
    struct _Data_Struct_Arr {
        //key, type, poiner to type
        std::vector<std::tuple<void*, Json_Private::Json_Type_::_Foo>> Elements;

    };
}
