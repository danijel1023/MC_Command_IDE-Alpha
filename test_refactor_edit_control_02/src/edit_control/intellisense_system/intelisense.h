#pragma once
#include "pch.h"
#include "../../json_reader/src/json.h"

#define TIMER_ID_TWO_SEC_TIMER 1

#include "is_parser_structs.h"
#include "error_handler.h"
#include "parsers/minecraft_entity/entity_struct.h"

class Edit_Control;
class IntelliSense {
public:
    void Init(HWND Parent);
    void Proc_Msg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void Proc_Msg_After(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    IntelliSense() = delete;
    IntelliSense(Edit_Control& EC);
    ~IntelliSense();

    void Two_Sec_Timer();

    void WM_KeyDown(WPARAM wParam, LPARAM lParam);
    void WM_Char(WPARAM wParam, LPARAM lParam);

private:
    Edit_Control& m_EC;
    HWND m_Parent = NULL;
    Json m_Syntax_Obj = Json();
    Error_Handler Error_Handler = ::Error_Handler();

    void Analise_Line(std::wstring& Line, std::vector<COLORREF>& Colors);
    COLORREF Get_Color(std::wstring Type);

    void Start_New_Line();

    bool Validate_Name(std::wstring Name);
    bool Validate_NBT(std::wstring Word);
    bool Validate_Advancements(std::wstring Word);
    bool Validate_Tags(std::wstring Word);
    bool Validate_Scores(std::wstring Word);
    bool Get_Num(std::wstring Word, MC_NType* Num);
    bool Get_Range(std::wstring Word, RANGE* Num);


    Json m_Minecraft_Block_Predicate_Obj = Json();
    Json m_Minecraft_Color_Obj = Json();
    Json m_Minecraft_Dimension_Obj = Json();
    Json m_Minecraft_Entity_Anchor_Obj = Json();
    Json m_Minecraft_Entity_Summon_Obj = Json();
    Json m_Minecraft_Item_Enchantment_Obj = Json();
    Json m_Minecraft_Item_Predicate_Obj = Json();
    Json m_Minecraft_Item_Stack_Obj = Json();
    Json m_Minecraft_Mob_Effect_Obj = Json();
    Json m_Minecraft_Objective_Criteria_Obj = Json();
    Json m_Minecraft_Operation_Obj = Json();
    Json m_Minecraft_Particle_Obj = Json();
    Json m_Minecraft_Scoreboard_Slot_Obj = Json();

    std::wstring Parser_Matcher(std::wstring Word);
    
    std::wstring m_Paraser_Func_Name;
    bool m_Paraser_Set_Lock = false;
    unsigned int m_Paraser_Func_Counter = 0;

    bool Brigadier_Bool(std::wstring& Word);
    bool Brigadier_Double(std::wstring& Word);
    bool Brigadier_Float(std::wstring& Word);
    bool Brigadier_Integer(std::wstring& Word);
    bool Brigadier_String(std::wstring& Word);          //"type": "word" - same as Minecraft_Objective, "type": "greedy" - same as Minecraft_Message

    bool Minecraft_Block_Pos(std::wstring& Word);
    bool Minecraft_Block_Predicate(std::wstring& Word);
    bool Minecraft_Color(std::wstring& Word);
    bool Minecraft_Column_Pos(std::wstring& Word);


    bool Minecraft_Component(std::wstring& Word);       //Later - Json format


    bool Minecraft_Dimension(std::wstring& Word);
    bool Minecraft_Entity(std::wstring& Word);
    bool Minecraft_Entity_Anchor(std::wstring& Word);
    bool Minecraft_Entity_Summon(std::wstring& Word);   //Entity list

    bool Minecraft_Function(std::wstring& Word);        //Function name syntax
    bool Minecraft_Game_Profile(std::wstring& Word);    //[not supported]
    bool Minecraft_Int_Range(std::wstring& Word);       //int (.. int)
    bool Minecraft_Item_Enchantment(std::wstring& Word);//Enchantment list
    bool Minecraft_Item_Predicate(std::wstring& Word);  //Full Item list
    bool Minecraft_Item_Stack(std::wstring& Word);      //Item list
    bool Minecraft_Message(std::wstring& Word);         //Any string - by nature cannot be not-on-the-end of the command
    bool Minecraft_Mob_Effect(std::wstring& Word);      //Effect list
    bool Minecraft_Nbt_Compound_Tag(std::wstring& Word);//Later - Json format
    bool Minecraft_Nbt_Path(std::wstring& Word);        //Later - semi Json format
    bool Minecraft_Nbt_Tag(std::wstring& Word);         //Later - idk wtf is this
    bool Minecraft_Objective(std::wstring& Word);       //String - "... includes all lowercase and uppercase letters, numbers, underscore, period, minus and plus."
    bool Minecraft_Objective_Criteria(std::wstring& Word);  //scoreboard objectives add <name> <Objective_Criteria>
    bool Minecraft_Operation(std::wstring& Word);       //scoreboard players operation <a> <b> <Operation>
    bool Minecraft_Particle(std::wstring& Word);        //Particle list (need parser lock on minecraft:block/item)
    bool Minecraft_Resource_Location(std::wstring& Word);   //Later - ???... just string... idk... more info
    bool Minecraft_Rotation(std::wstring& Word);        //double double
    bool Minecraft_Score_Holder(std::wstring& Word);    //(almost?) like entity
    bool Minecraft_Scoreboard_Slot(std::wstring& Word); //scoreboard objectives setdisplay <Scoreboard_Slot>
    bool Minecraft_Swizzle(std::wstring& Word);         //combination of 'x', 'y' and 'z'
    bool Minecraft_Team(std::wstring& Word);            //same as Minecraft_Objective
    bool Minecraft_Time(std::wstring& Word);            //number (+ d/s/t)
    bool Minecraft_Vec2(std::wstring& Word);            //Lock for 2x double
    bool Minecraft_Vec3(std::wstring& Word);            //Lock for 3x double

    bool Generic_ArrElm_Search(Json& Obj, std::wstring Word);
    bool Generic_Split(std::wstring Words, std::vector<std::wstring>* Output_Ptr);

    bool Is_Lowercase(wchar_t& ch);
    bool Is_Uppercase(wchar_t& ch);
    bool Is_Number(wchar_t& ch);

    size_t m_InitCalls = 0;
    bool m_Is_Initialized = false;

    friend class Edit_Control;
};
