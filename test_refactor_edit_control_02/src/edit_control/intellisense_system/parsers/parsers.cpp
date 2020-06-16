#include "pch.h"
#include "intelisense.h"

std::wstring IntelliSense::Parser_Matcher(std::wstring& Word) {
    return Parser_Matcher(Word, m_Syntax_Obj);
}

std::wstring IntelliSense::Parser_Matcher(std::wstring& Word, Json& m_Syntax_Obj) {
    if (Word.size() == 0) {
        Error_Handler << L"Empty paramater";
        return L"ERROR";
    }

    std::wstring Parser = m_Syntax_Obj.Str();   //debugging

    if (m_Syntax_Obj.Str() == L"brigadier:bool") {
        m_Syntax_Obj.Back();
        if (Brigadier_Bool(Word)) return L"brigadier:bool";
    }

    else if (m_Syntax_Obj.Str() == L"brigadier:double") {
        m_Syntax_Obj.Back();
        if (Brigadier_Double(Word)) return L"brigadier:double";
    }

    else if (m_Syntax_Obj.Str() == L"brigadier:float") {
        m_Syntax_Obj.Back();
        if (Brigadier_Float(Word)) return L"brigadier:float";
    }

    else if (m_Syntax_Obj.Str() == L"brigadier:integer") {
        m_Syntax_Obj.Back();
        if (Brigadier_Integer(Word)) return L"brigadier:integer";
    }

    else if (m_Syntax_Obj.Str() == L"brigadier:string") {
        m_Syntax_Obj.Back();
        if (Brigadier_String(Word)) return L"brigadier:string";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:block_pos") {
        m_Syntax_Obj.Back();
        if (Minecraft_Block_Pos(Word)) return L"minecraft:block_pos";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:block_predicate") {
        m_Syntax_Obj.Back();
        if (Minecraft_Block_Predicate(Word)) return L"minecraft:block_predicate";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:block_state") {
        m_Syntax_Obj.Back();
        if (Minecraft_Block_State(Word)) return L"minecraft:block_state";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:color") {
        m_Syntax_Obj.Back();
        if (Minecraft_Color(Word)) return L"minecraft:color";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:column_pos") {
        m_Syntax_Obj.Back();
        if (Minecraft_Column_Pos(Word)) return L"minecraft:column_pos";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:component") {
        m_Syntax_Obj.Back();
        if (Minecraft_Component(Word)) return L"minecraft:component";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:dimension") {
        m_Syntax_Obj.Back();
        if (Minecraft_Dimension(Word)) return L"minecraft:dimension";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:double_range") { //Custom parsers
        m_Syntax_Obj.Back();
        if (Minecraft_Double_Range(Word)) return L"minecraft:double_range";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:entity") {
        m_Syntax_Obj.Back();
        if (Minecraft_Entity(Word)) return L"minecraft:entity";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:entity_advancements") {  //Custom parsers
        m_Syntax_Obj.Back();
        if (Minecraft_Entity_Advancements(Word)) return L"minecraft:entity_advancements";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:entity_anchor") {
        m_Syntax_Obj.Back();
        if (Minecraft_Entity_Anchor(Word)) return L"minecraft:entity_anchor";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:entity_scores") {    //Custom parsers
        m_Syntax_Obj.Back();
        if (Minecraft_Entity_Scores(Word)) return L"minecraft:entity_scores";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:entity_summon") {
        m_Syntax_Obj.Back();
        if (Minecraft_Entity_Summon(Word)) return L"minecraft:entity_summon";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:entity_type") {    //Custom parsers
        m_Syntax_Obj.Back();
        if (Minecraft_Entity_Type(Word)) return L"minecraft:entity_type";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:function") {
        m_Syntax_Obj.Back();
        if (Minecraft_Function(Word)) return L"minecraft:function";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:game_profile") {
        m_Syntax_Obj.Back();
        if (Minecraft_Game_Profile(Word)) return L"minecraft:game_profile";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:int_range") {
        m_Syntax_Obj.Back();
        if (Minecraft_Int_Range(Word)) return L"minecraft:int_range";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:item_enchantment") {
        m_Syntax_Obj.Back();
        if (Minecraft_Item_Enchantment(Word)) return L"minecraft:item_enchantment";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:item_predicate") {
        m_Syntax_Obj.Back();
        if (Minecraft_Item_Predicate(Word)) return L"minecraft:item_predicate";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:item_stack") {
        m_Syntax_Obj.Back();
        if (Minecraft_Item_Stack(Word)) return L"minecraft:item_stack";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:message") {
        m_Syntax_Obj.Back();
        if (Minecraft_Message(Word)) return L"minecraft:message";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:mob_effect") {
        m_Syntax_Obj.Back();
        if (Minecraft_Mob_Effect(Word)) return L"minecraft:mob_effect";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:nbt_compound_tag") {
        m_Syntax_Obj.Back();
        if (Minecraft_Nbt_Compound_Tag(Word)) return L"minecraft:nbt_compound_tag";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:nbt_path") {
        m_Syntax_Obj.Back();
        if (Minecraft_Nbt_Path(Word)) return L"minecraft:nbt_path";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:nbt_tag") {
        m_Syntax_Obj.Back();
        if (Minecraft_Nbt_Tag(Word)) return L"minecraft:nbt_tag";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:objective") {
        m_Syntax_Obj.Back();
        if (Minecraft_Objective(Word)) return L"minecraft:objective";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:objective_criteria") {
        m_Syntax_Obj.Back();
        if (Minecraft_Objective_Criteria(Word)) return L"minecraft:objective_criteria";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:operation") {
        m_Syntax_Obj.Back();
        if (Minecraft_Operation(Word)) return L"minecraft:operation";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:particle") {
        m_Syntax_Obj.Back();
        if (Minecraft_Particle(Word)) return L"minecraft:particle";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:resource_location") {
        m_Syntax_Obj.Back();
        if (Minecraft_Resource_Location(Word)) return L"minecraft:resource_location";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:rotation") {
        m_Syntax_Obj.Back();
        if (Minecraft_Rotation(Word)) return L"minecraft:rotation";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:score_holder") {
        m_Syntax_Obj.Back();
        if (Minecraft_Score_Holder(Word)) return L"minecraft:score_holder";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:scoreboard_slot") {
        m_Syntax_Obj.Back();
        if (Minecraft_Scoreboard_Slot(Word)) return L"minecraft:scoreboard_slot";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:swizzle") {
        m_Syntax_Obj.Back();
        if (Minecraft_Swizzle(Word)) return L"minecraft:swizzle";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:team") {
        m_Syntax_Obj.Back();
        if (Minecraft_Team(Word)) return L"minecraft:team";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:time") {
        m_Syntax_Obj.Back();
        if (Minecraft_Time(Word)) return L"minecraft:time";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:vec2") {
        m_Syntax_Obj.Back();
        if (Minecraft_Vec2(Word)) return L"minecraft:vec2";
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:vec3") {
        m_Syntax_Obj.Back();
        if (Minecraft_Vec3(Word)) return L"minecraft:vec3";
    }

    if (m_Paraser_Set_Lock) return L"INCOMPLETE";
    return L"ERROR";
}
