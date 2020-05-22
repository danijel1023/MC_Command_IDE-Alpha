#include "pch.h"
#include "intelisense.h"


std::wstring IntelliSense::Paraser_Matcher(std::wstring Word) {
    if (m_Syntax_Obj.Str() == L"brigadier:bool") {
        m_Syntax_Obj.Back();
        if (Brigadier_Bool(Word)) return L"brigadier:bool";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"brigadier:bool";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"brigadier:double") {
        m_Syntax_Obj.Back();
        if (Brigadier_Double(Word)) return L"brigadier:double";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"brigadier:double";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"brigadier:float") {
        m_Syntax_Obj.Back();
        if (Brigadier_Float(Word)) return L"brigadier:float";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"brigadier:float";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"brigadier:integer") {
        m_Syntax_Obj.Back();
        if (Brigadier_Integer(Word)) return L"brigadier:integer";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"brigadier:integer";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:block_pos") {
        m_Syntax_Obj.Back();
        if (Minecraft_Block_Pos(Word)) return L"minecraft:block_pos";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:block_pos";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:block_predicate") {
        m_Syntax_Obj.Back();
        if (Minecraft_Block_Predicate(Word)) return L"minecraft:block_predicate";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:block_predicate";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:color") {
        m_Syntax_Obj.Back();
        if (Minecraft_Color(Word)) return L"minecraft:color";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:color";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:column_pos") {
        m_Syntax_Obj.Back();
        if (Minecraft_Column_Pos(Word)) return L"minecraft:column_pos";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:column_pos";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:component") {
        m_Syntax_Obj.Back();
        if (Minecraft_Component(Word)) return L"minecraft:component";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:component";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:dimension") {
        m_Syntax_Obj.Back();
        if (Minecraft_Dimension(Word)) return L"minecraft:dimension";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:dimension";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:entity") {
        m_Syntax_Obj.Back();
        if (Minecraft_Entity(Word)) return L"minecraft:entity";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:entity";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:entity_anchor") {
        m_Syntax_Obj.Back();
        if (Minecraft_Entity_Anchor(Word)) return L"minecraft:entity_anchor";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:entity_anchor";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:entity_summon") {
        m_Syntax_Obj.Back();
        if (Minecraft_Entity_Summon(Word)) return L"minecraft:entity_summon";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:entity_summon";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:function") {
        m_Syntax_Obj.Back();
        if (Minecraft_Function(Word)) return L"minecraft:function";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:function";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:game_profile") {
        m_Syntax_Obj.Back();
        if (Minecraft_Game_Profile(Word)) return L"minecraft:game_profile";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:game_profile";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:int_range") {
        m_Syntax_Obj.Back();
        if (Minecraft_Int_Range(Word)) return L"minecraft:int_range";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:int_range";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:item_enchantment") {
        m_Syntax_Obj.Back();
        if (Minecraft_Item_Enchantment(Word)) return L"minecraft:item_enchantment";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:item_enchantment";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:item_predicate") {
        m_Syntax_Obj.Back();
        if (Minecraft_Item_Predicate(Word)) return L"minecraft:item_predicate";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:item_predicate";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:item_stack") {
        m_Syntax_Obj.Back();
        if (Minecraft_Item_Stack(Word)) return L"minecraft:item_stack";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:item_stack";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:message") {
        m_Syntax_Obj.Back();
        if (Minecraft_Message(Word)) return L"minecraft:message";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:message";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:mob_effect") {
        m_Syntax_Obj.Back();
        if (Minecraft_Mob_Effect(Word)) return L"minecraft:mob_effect";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:mob_effect";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:nbt_compound_tag") {
        m_Syntax_Obj.Back();
        if (Minecraft_Nbt_Compound_Tag(Word)) return L"minecraft:nbt_compound_tag";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:nbt_compound_tag";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:nbt_path") {
        m_Syntax_Obj.Back();
        if (Minecraft_Nbt_Path(Word)) return L"minecraft:nbt_path";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:nbt_path";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:nbt_tag") {
        m_Syntax_Obj.Back();
        if (Minecraft_Nbt_Tag(Word)) return L"minecraft:nbt_tag";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:nbt_tag";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:objective") {
        m_Syntax_Obj.Back();
        if (Minecraft_Objective(Word)) return L"minecraft:objective";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:objective";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:objective_criteria") {
        m_Syntax_Obj.Back();
        if (Minecraft_Objective_Criteria(Word)) return L"minecraft:objective_criteria";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:objective_criteria";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:operation") {
        m_Syntax_Obj.Back();
        if (Minecraft_Operation(Word)) return L"minecraft:operation";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:operation";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:particle") {
        m_Syntax_Obj.Back();
        if (Minecraft_Particle(Word)) return L"minecraft:particle";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:particle";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:resource_location") {
        m_Syntax_Obj.Back();
        if (Minecraft_Resource_Location(Word)) return L"minecraft:resource_location";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:resource_location";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:rotation") {
        m_Syntax_Obj.Back();
        if (Minecraft_Rotation(Word)) return L"minecraft:rotation";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:rotation";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:score_holder") {
        m_Syntax_Obj.Back();
        if (Minecraft_Score_Holder(Word)) return L"minecraft:score_holder";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:score_holder";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:scoreboard_slot") {
        m_Syntax_Obj.Back();
        if (Minecraft_Scoreboard_Slot(Word)) return L"minecraft:scoreboard_slot";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:scoreboard_slot";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:string") {
        m_Syntax_Obj.Back();
        if (Minecraft_String(Word)) return L"minecraft:string";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:string";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:swizzle") {
        m_Syntax_Obj.Back();
        if (Minecraft_Swizzle(Word)) return L"minecraft:swizzle";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:swizzle";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:team") {
        m_Syntax_Obj.Back();
        if (Minecraft_Team(Word)) return L"minecraft:team";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:team";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:time") {
        m_Syntax_Obj.Back();
        if (Minecraft_Time(Word)) return L"minecraft:time";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:time";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:vec2") {
        m_Syntax_Obj.Back();
        if (Minecraft_Vec2(Word)) return L"minecraft:vec2";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:vec2";
            return L"INCOMPLETE";
        }
    }

    else if (m_Syntax_Obj.Str() == L"minecraft:vec3") {
        m_Syntax_Obj.Back();
        if (Minecraft_Vec3(Word)) return L"minecraft:vec3";
        if (m_Paraser_Set_Lock) {
            m_Paraser_Func_Name = L"minecraft:vec3";
            return L"INCOMPLETE";
        }
    }

    return L"ERROR";
}
