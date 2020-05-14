#pragma once
#include <string>

typedef struct MCE_Prop {
    enum class AMOUNT : char {
        Single,
        Multiple
    };

    enum class TYPE : char {
        Players,
        Entities
    };

    AMOUNT Amount;
    TYPE Type;
} MCE_Prop;

typedef struct STR {
    std::wstring Name;
    bool This = false;
} STR;


enum class SORT : char {
    Arbitrary,
    Furthest,
    Nearest,
    Random,
    Unset
};

enum class Selection_Type : char {
    Radius,
    Volume
};

enum class GAMEMODE : char {
    Adventure,
    Creativ,
    Spectator,
    Survival,
    Unset
};



typedef struct MC_Entity {
    MCE_Prop Ent_Prop = MCE_Prop();

    unsigned long long Limit = 0;
    long double X = 0.0, Y = 0.0, Z = 0.0;
    long double Min_Distance = 0.0, Max_Distance = 0.0;
    long double DX = 0.0, DY = 0, DZ = 0.0;
    long double Min_VRotation = 0.0, Max_VRotation = 0.0;
    long double Min_HRotation = 0.0, Max_HRotation = 0.0;
    SORT Sort = SORT::Unset;
    GAMEMODE Gamemode = GAMEMODE::Unset;
    STR Team;
    STR Name;
    STR NBT;
    STR Advancements;
    STR Tags;
    STR Scores;
} MC_Entity;
