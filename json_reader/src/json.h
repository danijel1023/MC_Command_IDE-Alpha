#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <tuple>

#include "json_type_defines.h"

class Json {
public:
    Json();
    Json(const std::wstring& Path);
    ~Json();

    //Init function with just path to file
    void Init(const std::wstring& Path);

    Json& Root();
    Json& Back();
    Json& Obj(std::wstring Name);
    Json& Arr(size_t Index);

    Str_Element Str();
    Num_Element Num();
    Bol_Element Bol();
    Ptr_Element Ptr();
    size_t Size();
    std::vector<std::wstring> Get_Names();
    bool Has_Name(const std::wstring& Name);

    std::wstringstream Get_Tree();


    Json& operator=(Json& Right);

private:
    size_t m_InitCalls = 0;
    bool m_Initialized = false;
    bool m_Halted = false;
    bool m_Error_Reading_Tree = false;
    bool m_Error_Building_Tree = false;



    //Init function with for raw file data
    void RD_Init(const std::wstring& Raw_Data);

    //Sanitised raw data: removed unnecessary spaces, newlines and tabs
    std::wstring* m_Raw_Data = new std::wstring();

    //Sanitised file data - simplified structure
    std::wstring* Simp_Data = new std::wstring();

    // file data - 'names' and 'values' corresponding to 'simplified structure'
    std::vector<std::wstring>* Simp_Data_Val = new std::vector<std::wstring>();

    //Contains pointer to first 'value' and its type
    Element_Struct* m_Root = new Element_Struct();

    //Keeps track of current selected item and all other items in the tree
    std::vector<Element_Struct>* m_Branch = new std::vector<Element_Struct>();



    //Remove all unnecessary chars (space, new line, tabs) - sanitize the raw input
    void Remove_Unnec_Char(std::wstring& Data);
    
    //Simplifies data in easy-to-work with format
    void Format_Data(std::wstring& InputData, std::wstring* Simp_Data_Ptr, std::vector<std::wstring>* Simp_Data_Val_Ptr);

    //Builds the data tree based on simplified data
    void Build_Tree(std::wstring& Simp_Data, std::vector<std::wstring>& Simp_Data_Val);



    //Tests if given char is number from 0-9; can exclude one number 0-9
    bool Is_Number(wchar_t N, int Exclude = 10);

    //Tests if given char is "sysntax char", aka []{},:
    bool Is_Syntax_ch(wchar_t ch);

    //Tests if given char is "value char" aka SNBP[{
    bool Is_Value(wchar_t ch);
    


    //On request, delete whole tree. 
    void Delete_Tree();

    //Copy tree from 'From.m_Branch'
    void Copy_Tree(const Json& From);



    bool m_get_str_tree_is_root = true;
    bool m_get_str_tree_disable_is_root = false;
    void* m_get_str_tree_last_ptr = nullptr;
    size_t m_get_str_tree_last_Index = 0;
    Data_Value_Type m_get_str_tree_last_type = Data_Value_Type::Array;
    size_t m_print_tree_Index = 0;

    std::vector<wchar_t>* m_print_tree_Index_S = new std::vector<wchar_t>();
};
