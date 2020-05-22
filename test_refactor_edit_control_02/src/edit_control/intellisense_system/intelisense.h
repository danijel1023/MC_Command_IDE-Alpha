#pragma once
#include "pch.h"
#include "../../json_reader/src/json.h"

#define TIMER_ID_TWO_SEC_TIMER 1

#include "is_parser_structs.h"
#include "error_handler.h"
#include "parasers/minecraft_entity/entity_struct.h"

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
    /*Json m_Minecraft_Color_Obj = Json();
    Json m_Minecraft_Color_Obj = Json();
    Json m_Minecraft_Color_Obj = Json();*/

    std::wstring Paraser_Matcher(std::wstring Word);
    
    std::wstring m_Paraser_Func_Name;
    bool m_Paraser_Set_Lock = false;
    unsigned int m_Paraser_Func_Counter = 0;

    bool Brigadier_Bool(std::wstring& Word);
    bool Brigadier_Double(std::wstring& Word);
    bool Brigadier_Float(std::wstring& Word);
    bool Brigadier_Integer(std::wstring& Word);
    bool Minecraft_Block_Pos(std::wstring& Word);

    bool Minecraft_Block_Predicate(std::wstring& Word);
    bool Minecraft_Color(std::wstring& Word);

    bool Minecraft_Column_Pos(std::wstring& Word);
    bool Minecraft_Component(std::wstring& Word);
    bool Minecraft_Dimension(std::wstring& Word);
    bool Minecraft_Entity(std::wstring& Word);
    bool Minecraft_Entity_Anchor(std::wstring& Word);
    bool Minecraft_Entity_Summon(std::wstring& Word);
    bool Minecraft_Function(std::wstring& Word);
    bool Minecraft_Game_Profile(std::wstring& Word);
    bool Minecraft_Int_Range(std::wstring& Word);
    bool Minecraft_Item_Enchantment(std::wstring& Word);
    bool Minecraft_Item_Predicate(std::wstring& Word);
    bool Minecraft_Item_Stack(std::wstring& Word);
    bool Minecraft_Message(std::wstring& Word);
    bool Minecraft_Mob_Effect(std::wstring& Word);
    bool Minecraft_Nbt_Compound_Tag(std::wstring& Word);
    bool Minecraft_Nbt_Path(std::wstring& Word);
    bool Minecraft_Nbt_Tag(std::wstring& Word);
    bool Minecraft_Objective(std::wstring& Word);
    bool Minecraft_Objective_Criteria(std::wstring& Word);
    bool Minecraft_Operation(std::wstring& Word);
    bool Minecraft_Particle(std::wstring& Word);
    bool Minecraft_Resource_Location(std::wstring& Word);
    bool Minecraft_Rotation(std::wstring& Word);
    bool Minecraft_Score_Holder(std::wstring& Word);
    bool Minecraft_Scoreboard_Slot(std::wstring& Word);
    bool Minecraft_String(std::wstring& Word);
    bool Minecraft_Swizzle(std::wstring& Word);
    bool Minecraft_Team(std::wstring& Word);
    bool Minecraft_Time(std::wstring& Word);
    bool Minecraft_Vec2(std::wstring& Word);
    bool Minecraft_Vec3(std::wstring& Word);

    size_t m_InitCalls = 0;
    bool m_Is_Initialized = false;

    friend class Edit_Control;
};
