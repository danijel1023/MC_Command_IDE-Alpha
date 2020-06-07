#pragma once
#include "pch.h"
#include "../../json_reader/src/json.h"

#define TIMER_ID_TWO_SEC_TIMER 1

#include "is_parser_structs.h"
#include "error_handler.h"
#include "parsers/minecraft_entity/entity_struct.h"
#include "snbt_formater/snbt_formater.h"
#include "state_formater/state_formater.h"

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

    Json m_Minecraft_Block_Predicate_Obj = Json();
    Json m_Minecraft_Block_State_Obj = Json();
    Json m_Minecraft_Color_Obj = Json();
    Json m_Minecraft_Dimension_Obj = Json();
    Json m_Minecraft_Entity_Obj = Json();
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
    std::wstring Parser_Matcher(std::wstring Word, Json& m_Syntax_Obj);

    std::wstring m_Paraser_Func_Name;
    bool m_Paraser_Set_Lock = false;
    unsigned int m_Paraser_Func_Counter = 0;

    bool Brigadier_Bool(std::wstring& Word);
    bool Brigadier_Double(std::wstring& Word, bool Use_Ret = false, double* Ret_Val = nullptr, bool Use_Prop = true, bool Use_Min = false, double Min = 0, bool Use_Max = false, double Max = 0);
    bool Brigadier_Double(std::wstring& Word, Json& m_Syntax_Obj, bool Use_Ret = false, double* Ret_Val = nullptr, bool Use_Prop = true, bool Use_Min = false, double Min = 0, bool Use_Max = false, double Max = 0);
    bool Brigadier_Float(std::wstring& Word, bool Use_Ret = false, double* Ret_Val = nullptr, bool Use_Prop = true, bool Use_Min = false, double Min = 0, bool Use_Max = false, double Max = 0);
    bool Brigadier_Float(std::wstring& Word, Json& m_Syntax_Obj, bool Use_Ret = false, double* Ret_Val = nullptr, bool Use_Prop = true, bool Use_Min = false, double Min = 0, bool Use_Max = false, double Max = 0);
    bool Brigadier_Integer(std::wstring& Word, bool Use_Ret = false, int* Ret_Val = nullptr, bool Use_Prop = true, bool Use_Min = false, int Min = 0, bool Use_Max = false, int Max = 0);
    bool Brigadier_Integer(std::wstring& Word, Json& m_Syntax_Obj, bool Use_Ret = false, int* Ret_Val = nullptr, bool Use_Prop = true, bool Use_Min = false, int Min = 0, bool Use_Max = false, int Max = 0);
    bool Brigadier_String(std::wstring& Word, std::wstring Properties = std::wstring());

    bool Minecraft_Block_Pos(std::wstring& Word);
    bool Minecraft_Block_Predicate(std::wstring& Word);
    bool Minecraft_Block_State(std::wstring& Word);
    bool Minecraft_Color(std::wstring& Word);
    bool Minecraft_Column_Pos(std::wstring& Word);
    bool Minecraft_Component(std::wstring& Word);
    bool Minecraft_Dimension(std::wstring& Word);
    bool Minecraft_Double_Range(std::wstring& Word, bool Use_Prop = true, bool Use_Min = false, double Min = 0, bool Use_Max = false, double Max = 0);
    bool Minecraft_Double_Range(std::wstring& Word, Json& m_Syntax_Obj, bool Use_Prop = true, bool Use_Min = false, double Min = 0, bool Use_Max = false, double Max = 0);
    bool Minecraft_Entity(std::wstring& Word);
    bool Minecraft_Entity_Advancements(std::wstring& Word);
    bool Minecraft_Entity_Anchor(std::wstring& Word);
    bool Minecraft_Entity_Scores(std::wstring& Word);
    bool Minecraft_Entity_Summon(std::wstring& Word);
    bool Minecraft_Function(std::wstring& Word);
    bool Minecraft_Game_Profile(std::wstring& Word);
    bool Minecraft_Int_Range(std::wstring& Word, bool Use_Prop = true, bool Use_Min = false, int Min = 0, bool Use_Max = false, int Max = 0);
    bool Minecraft_Int_Range(std::wstring& Word, Json& m_Syntax_Obj, bool Use_Prop = true, bool Use_Min = false, int Min = 0, bool Use_Max = false, int Max = 0);
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
    bool Minecraft_Swizzle(std::wstring& Word);
    bool Minecraft_Team(std::wstring& Word);
    bool Minecraft_Time(std::wstring& Word);
    bool Minecraft_Vec2(std::wstring& Word);
    bool Minecraft_Vec3(std::wstring& Word);

    bool Generic_ArrElm_Search(Json& Obj, std::wstring Word);
    bool Generic_Split(std::wstring Words, std::vector<std::wstring>* Output_Ptr);


    size_t m_InitCalls = 0;
    bool m_Is_Initialized = false;

    friend class Edit_Control;
};
