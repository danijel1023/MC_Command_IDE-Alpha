#include "json.h"

Json::~Json() {
    m_Branch->erase(m_Branch->begin(), m_Branch->end());
    m_Branch->push_back(*m_Root);
    Delete_Tree();

    delete m_Raw_Data;
    delete Simp_Data;
    delete Simp_Data_Val;
    delete m_Root;
    delete m_Branch;
    delete m_print_tree_Index_S;
}

Json::Json() {}

Json::Json(const std::wstring& Path) {
    Init(Path);
}


Json& Json::operator=(Json& Right) {
    if (Right.m_Branch->size() == 0) Right.Root();
    switch (std::get<1>(Right.m_Branch->back())) {
    case Data_Value_Type::Object:   std::get<0>(*m_Root) = new Obj_Element; break;
    case Data_Value_Type::Array:    std::get<0>(*m_Root) = new Arr_Element; break;
    case Data_Value_Type::Bool:     std::get<0>(*m_Root) = new Bol_Element; break;
    case Data_Value_Type::Number:   std::get<0>(*m_Root) = new Num_Element; break;
    case Data_Value_Type::String:   std::get<0>(*m_Root) = new Str_Element; break;
    case Data_Value_Type::Pointer:   std::get<0>(*m_Root) = new Ptr_Element; break;
    }

    std::get<1>(*m_Root) = std::get<1>(Right.m_Branch->back());
    m_Branch->push_back(*m_Root);

    Copy_Tree(Right);
    return *this;
}


void Json::Init(const std::wstring& Path) {
    m_InitCalls++;
    if (m_Initialized) {
        //std::wcout << L"Init called to many times" << std::endl;
        return;
    }

    std::wifstream File(Path);
    if (!File.is_open()) {
        std::wcout << "Error opening the file";
        return;
    }

    File.seekg(0, std::ios::end);
    size_t size = File.tellg();
    File.seekg(0);

    std::wstring Str(size, ' ');
    File.read(&Str[0], size);

    RD_Init(Str);
}


void Json::RD_Init(const std::wstring& Raw_Data) {
    *m_Raw_Data = Raw_Data;
    // -- Converting raw file data ----
    Remove_Unnec_Char(*m_Raw_Data);

    m_Error_Building_Tree = true;
    Format_Data(*m_Raw_Data, Simp_Data, Simp_Data_Val);
    if (m_Error_Building_Tree) {
        m_Raw_Data->erase(m_Raw_Data->begin(), m_Raw_Data->end());
        Simp_Data->erase(Simp_Data->begin(), Simp_Data->end());
        Simp_Data_Val->erase(Simp_Data_Val->begin(), Simp_Data_Val->end());
        m_Halted = true;
        return;
    }
    // --------------------------------


    m_Error_Building_Tree = true;
    Build_Tree(*Simp_Data, *Simp_Data_Val);
    if (m_Error_Building_Tree) {
        m_Raw_Data->erase(m_Raw_Data->begin(), m_Raw_Data->end());
        Simp_Data->erase(Simp_Data->begin(), Simp_Data->end());
        Simp_Data_Val->erase(Simp_Data_Val->begin(), Simp_Data_Val->end());
        Delete_Tree();
        m_Halted = true;
        return;
    }

    Root();
    m_Initialized = true;
}


Json& Json::Root() {
    if (m_Halted) return *this;
    m_Branch->erase(m_Branch->begin(), m_Branch->end());
    m_Branch->push_back(*m_Root);

    return *this;
}

Json& Json::Back() {
    if (m_Halted) return *this;
    if (m_Branch->size() > 1) {
        m_Branch->pop_back();
    }

    return *this;
}

Json& Json::Obj(std::wstring Name) {
    if (m_Halted) return *this;
    if (m_Error_Reading_Tree) return *this;

    auto C_Elm = (Obj_Element*)std::get<0>(m_Branch->back());

    if (std::get<1>(m_Branch->back()) == Data_Value_Type::Object) {
        for (size_t Pos = 0; Pos < C_Elm->Elements.size(); Pos++) {
            if (std::get<2>(C_Elm->Elements.at(Pos)) == Name) {
                m_Branch->push_back({ std::get<0>(C_Elm->Elements.at(Pos)), std::get<1>(C_Elm->Elements.at(Pos)) });
                return *this;
            }
        }
    }

    m_Error_Reading_Tree = true;
    return *this;
}

Json& Json::Arr(size_t Index) {
    if (m_Halted) return *this;
    if (m_Error_Reading_Tree) return *this;

    auto C_Elm = (Arr_Element*)std::get<0>(m_Branch->back());

    if (std::get<1>(m_Branch->back()) == Data_Value_Type::Array) {
        if (C_Elm->Elements.size() >= Index + 1) {
            m_Branch->push_back({ std::get<0>(C_Elm->Elements.at(Index)), std::get<1>(C_Elm->Elements.at(Index)) });
            return *this;
        }
    }

    m_Error_Reading_Tree = true;
    return *this;
}


Str_Element Json::Str() {
    if (m_Halted) return Str_Element();
    if (m_Error_Reading_Tree) {
        m_Error_Reading_Tree = false;
        return Str_Element();
    }

    if (std::get<1>(m_Branch->back()) == Data_Value_Type::String) {
        return (*(Str_Element*)std::get<0>(m_Branch->back()));
    }

    return Str_Element();
}

Num_Element Json::Num() {
    if (m_Halted) return Num_Element();
    if (m_Error_Reading_Tree) {
        m_Error_Reading_Tree = false;
        return Num_Element();
    }

    if (std::get<1>(m_Branch->back()) == Data_Value_Type::Number) {
        return (*(Num_Element*)std::get<0>(m_Branch->back()));
    }

    return Num_Element();
}

Bol_Element Json::Bol() {
    if (m_Halted) return Bol_Element();
    if (m_Error_Reading_Tree) {
        m_Error_Reading_Tree = false;
        return Bol_Element();
    }

    if (std::get<1>(m_Branch->back()) == Data_Value_Type::Bool) {
        return (*(Bol_Element*)std::get<0>(m_Branch->back()));
    }

    return Bol_Element();
}

Ptr_Element Json::Ptr() {
    if (m_Halted) return Ptr_Element();
    if (m_Error_Reading_Tree) {
        m_Error_Reading_Tree = false;
        return Ptr_Element();
    }

    if (std::get<1>(m_Branch->back()) == Data_Value_Type::Pointer) {
        return nullptr;
    }

    return Ptr_Element();
}

size_t Json::Size() {
    if (m_Halted) return 0;
    if (m_Error_Reading_Tree) {
        m_Error_Reading_Tree = false;
        return 0;
    }

    if (std::get<1>(m_Branch->back()) == Data_Value_Type::Object) {
        return ((Obj_Element*)std::get<0>(m_Branch->back()))->Elements.size();
    }
    
    else if (std::get<1>(m_Branch->back()) == Data_Value_Type::Array) {
        return ((Arr_Element*)std::get<0>(m_Branch->back()))->Elements.size();
    }

    return 0;
}

bool Json::Has_Name(const std::wstring& Name) {
    if (m_Halted) return 0;
    if (m_Error_Reading_Tree) {
        m_Error_Reading_Tree = false;
        return 0;
    }

    if (std::get<1>(m_Branch->back()) == Data_Value_Type::Object) {
        auto& Elements = ((Obj_Element*)std::get<0>(m_Branch->back()))->Elements;

        size_t Elements_Size = Elements.size();
        for (size_t Pos = 0; Pos < Elements_Size; Pos++) {
            if (std::get<2>(Elements.at(Pos)) == Name) {
                return true;
            }
        }
    }

    return false;
}

std::vector<std::wstring> Json::Get_Names() {
    if (m_Halted) return std::vector<std::wstring>();
    if (m_Error_Reading_Tree) {
        m_Error_Reading_Tree = false;
        return std::vector<std::wstring>();
    }

    if (std::get<1>(m_Branch->back()) == Data_Value_Type::Object) {
        std::vector<std::wstring> Keys;
        auto& Elements = ((Obj_Element*)std::get<0>(m_Branch->back()))->Elements;

        size_t Elements_Size = Elements.size();
        for (size_t Pos = 0; Pos < Elements_Size; Pos++) {
            Keys.push_back(std::get<2>(Elements.at(Pos)));
        }

        return Keys;
    }

    return std::vector<std::wstring>();
}


std::wstringstream Json::Get_Tree() {
    if (m_Halted) return std::wstringstream();
    std::wstringstream Ret;

    void* C_Ptr = std::get<0>(m_Branch->back());
    auto c_type = std::get<1>(m_Branch->back());

    if (!m_get_str_tree_is_root && !m_get_str_tree_disable_is_root) {
        if (m_get_str_tree_last_type == Data_Value_Type::Object) {
            auto Elm = ((Obj_Element*)m_get_str_tree_last_ptr);
            Ret << "Key: \"" << (std::get<2>(Elm->Elements.at(m_get_str_tree_last_Index))) << '"' << std::endl;

        } else {
            Ret << "Index: " << m_get_str_tree_last_Index << std::endl;
        }
        m_get_str_tree_disable_is_root = true;
    }


    if (m_Branch->size() == 1) {
        Ret << "Root: ";

        switch (c_type) {
        case Data_Value_Type::Object: Ret << "Object"; break;
        case Data_Value_Type::Array: Ret << "Array"; break;
        case Data_Value_Type::Bool: Ret << "Bool"; break;
        case Data_Value_Type::Number: Ret << "Number"; break;
        case Data_Value_Type::String: Ret << "String"; break;
        }

        Ret << std::endl;
    }
    

    if (c_type == Data_Value_Type::Object) {
        auto Elm = ((Obj_Element*)C_Ptr);

        for (size_t i = 0; i < Elm->Elements.size(); i++) {
            m_Branch->push_back({ (std::get<0>(Elm->Elements.at(i))), (std::get<1>(Elm->Elements.at(i))) });

            for (size_t i = 0; i < m_print_tree_Index * 2; i++) {
                if (i % 2 == 0) {
                    Ret << m_print_tree_Index_S->at(i / 2);
                }

                else {
                    Ret << "   ";
                }
            }

            m_print_tree_Index++;
            if (i + 1 == Elm->Elements.size()) {
                Ret << L'└';
                m_print_tree_Index_S->push_back(' ');
            } else {
                Ret << L'├';
                m_print_tree_Index_S->push_back(L'│');
            }

            for (wchar_t i = 0; i < 3; i++) Ret << L'─';
            Ret << "Key: \"" << (std::get<2>(Elm->Elements.at(i))) << '"' << std::endl;

            Ret << Get_Tree().str();
        }
        m_print_tree_Index--;
        if (m_print_tree_Index_S->size() > 0) m_print_tree_Index_S->pop_back();
        m_Branch->pop_back();

        return Ret;
    }

    else if (c_type == Data_Value_Type::Array) {
        auto Elm = ((Arr_Element*)C_Ptr);

        for (size_t i = 0; i < Elm->Elements.size(); i++) {
            m_Branch->push_back({ (std::get<0>(Elm->Elements.at(i))), (std::get<1>(Elm->Elements.at(i))) });

            for (size_t i = 0; i < m_print_tree_Index * 2; i++) {
                if (i % 2 == 0) {
                    Ret << m_print_tree_Index_S->at(i / 2);
                }

                else {
                    Ret << "   ";
                }
            }

            m_print_tree_Index++;
            if (i + 1 == Elm->Elements.size()) {
                Ret << L'└';
                m_print_tree_Index_S->push_back(L' ');
            } else {
                Ret << L'├';
                m_print_tree_Index_S->push_back(L'│');
            }

            for (wchar_t i = 0; i < 3; i++) Ret << L'─';
            Ret << L"Index: " << i << std::endl;

            Ret << Get_Tree().str();
        }
        m_print_tree_Index--;
        if (m_print_tree_Index_S->size() > 0) m_print_tree_Index_S->pop_back();
        m_Branch->pop_back();

        return Ret;
    }

    else if (c_type == Data_Value_Type::Number) {
        m_Branch->pop_back();
        auto Elm = ((Num_Element*)C_Ptr);

        for (size_t i = 0; i < m_print_tree_Index * 2; i++) {
            if (i % 2 == 0) {
                Ret << m_print_tree_Index_S->at(i / 2);
            }

            else {
                Ret << L"   ";
            }
        }

        Ret << L'└';
        for (wchar_t i = 0; i < 3; i++) Ret << L'─';
        Ret << L"Number: " << *Elm << std::endl;

        m_print_tree_Index--;
        if (m_print_tree_Index_S->size() > 0) m_print_tree_Index_S->pop_back();
        return Ret;

    }

    else if (c_type == Data_Value_Type::String) {
        m_Branch->pop_back();
        auto Elm = ((Str_Element*)C_Ptr);

        for (size_t i = 0; i < m_print_tree_Index * 2; i++) {
            if (i % 2 == 0) {
                Ret << m_print_tree_Index_S->at(i / 2);
            }

            else {
                Ret << "   ";
            }
        }

        Ret << L'└';
        for (wchar_t i = 0; i < 3; i++) Ret << L'─';
        Ret << "String: \"" << *Elm << '\"' << std::endl;

        m_print_tree_Index--;
        if (m_print_tree_Index_S->size() > 0) m_print_tree_Index_S->pop_back();
        return Ret;

    } else {
        m_Branch->pop_back();
        auto Elm = ((Bol_Element*)C_Ptr);

        for (size_t i = 0; i < m_print_tree_Index * 2; i++) {
            if (i % 2 == 0) {
                Ret << m_print_tree_Index_S->at(i / 2);
            }

            else {
                Ret << "   ";
            }
        }

        Ret << L'└';
        for (wchar_t i = 0; i < 3; i++) Ret << L'─';
        Ret << "Bool: " << (*Elm ? "true" : "false") << std::endl;

        m_print_tree_Index--;
        if (m_print_tree_Index_S->size() > 0) m_print_tree_Index_S->pop_back();
        return Ret;

    }
}


void Json::Delete_Tree() {
    if (m_Branch->size() == 0) return;
    void* c_ptr = std::get<0>(m_Branch->back());
    auto c_type = std::get<1>(m_Branch->back());
    if (c_ptr == nullptr) return;

    if (c_type == Data_Value_Type::Number) {
        m_Branch->pop_back();
        auto Elm = ((Num_Element*)c_ptr);

        if (m_Branch->size() != 0) {
            void* last_ptr = std::get<0>(m_Branch->back());
            auto last_type = std::get<1>(m_Branch->back());

            if (last_type == Data_Value_Type::Object) {
                auto last_Elm = ((Obj_Element*)last_ptr);
                //std::wcout << L"Deleting number element: " << Elm << ", value: " << Elm << ", key: \"" << std::get<2>(last_Elm->Elements.at(Index)) << '"' << std::endl;
            } else {
                //std::wcout << L"Deleting number element: " << Elm << ", value: " << Elm << ", Index:" << Index << std::endl;
            }

        } else {
            //std::wcout << L"Deleting number element: " << Elm << ", value: " << Elm << std::endl;
        }

        delete Elm;
        return;

    } else if (c_type == Data_Value_Type::String) {
        m_Branch->pop_back();
        auto Elm = ((Str_Element*)c_ptr);

        if (m_Branch->size() != 0) {
            void* last_ptr = std::get<0>(m_Branch->back());
            auto last_type = std::get<1>(m_Branch->back());

            if (last_type == Data_Value_Type::Object) {
                auto last_Elm = ((Obj_Element*)last_ptr);
                //std::wcout << L"Deleting string element: " << Elm << ", value: " << Elm << ", key: \"" << std::get<2>(last_Elm->Elements.at(Index)) << '"' << std::endl;
            } else {
                //std::wcout << L"Deleting string element: " << Elm << ", value: " << Elm << ", Index:" << Index << std::endl;
            }

        } else {
            //std::wcout << L"Deleting string element: " << Elm << ", value: " << Elm << std::endl;
        }

        delete Elm;
        return;

    } else if (c_type == Data_Value_Type::Bool) {
        m_Branch->pop_back();
        auto Elm = ((Bol_Element*)c_ptr);

        if (m_Branch->size() != 0) {
            void* last_ptr = std::get<0>(m_Branch->back());
            auto last_type = std::get<1>(m_Branch->back());

            if (last_type == Data_Value_Type::Object) {
                auto last_Elm = ((Obj_Element*)last_ptr);
                //std::wcout << L"Deleting bool element: " << Elm << ", value: " << ((Elm) ? "true" : "false") << ", key: \"" << std::get<2>(last_Elm->Elements.at(Index)) << '"' << std::endl;
            } else {
                //std::wcout << L"Deleting bool element: " << Elm << ", value: " << ((Elm) ? "true" : "false") << ", Index:" << Index << std::endl;
            }

        } else {
            //std::wcout << L"Deleting bool element: " << Elm << ", value: " << ((Elm) ? "true" : "false") << std::endl;
        }

        delete Elm;
        return;

    } else if (c_type == Data_Value_Type::Pointer) {
        m_Branch->pop_back();
        auto Elm = ((Ptr_Element*)c_ptr);

        if (m_Branch->size() != 0) {
            void* last_ptr = std::get<0>(m_Branch->back());
            auto last_type = std::get<1>(m_Branch->back());

            if (last_type == Data_Value_Type::Object) {
                auto last_Elm = ((Obj_Element*)last_ptr);
                //std::wcout << L"Deleting double element: " << Elm << ", value: " << Elm << ", key: \"" << std::get<2>(last_Elm->Elements.at(Index)) << '"' << std::endl;
            } else {
                //std::wcout << L"Deleting double element: " << Elm << ", value: " << Elm << ", Index:" << Index << std::endl;
            }

        } else {
            //std::wcout << L"Deleting double element: " << Elm << ", value: " << Elm << std::endl;
        }

        delete Elm;
        return;

    }

    else if (c_type == Data_Value_Type::Object) {
        auto Elm = ((Obj_Element*)c_ptr);

        for (size_t i = 0; i < Elm->Elements.size(); i++) {
            m_Branch->push_back({ (std::get<0>(Elm->Elements.at(i))), (std::get<1>(Elm->Elements.at(i))) });
            Delete_Tree();
        }
        m_Branch->pop_back();
        //std::wcout << L"Deleting object element: " << c_ptr << std::endl;
        delete Elm;
        return;

    } else  if (c_type == Data_Value_Type::Array) {
        auto Elm = ((Arr_Element*)c_ptr);

        for (size_t i = 0; i < Elm->Elements.size(); i++) {
            m_Branch->push_back({ (std::get<0>(Elm->Elements.at(i))), (std::get<1>(Elm->Elements.at(i))) });
            Delete_Tree();
        }
        m_Branch->pop_back();
        //std::wcout << L"Deleting array element: " << c_ptr << std::endl;
        delete Elm;
        return;

    }
}
