#pragma once
#include <string>

typedef struct RANGE {
    long double Min = 0.0;
    long double Max = 0.0;
    bool Use = false;

    RANGE& operator=(RANGE New) {
        this->Min = New.Min;
        this->Max = New.Max;
        Use = true;
        return *this;
    }

} RANGE;

typedef struct MC_NType {
    long double Val = 0.0;
    bool Use = false;

    operator long double() {
        return Val;
    }

    const long double operator=(const long double New_Val) {
        this->Val = New_Val;
        Use = true;
        return this->Val;
    }

} MC_NType;

typedef struct MCE_Prop {
    enum class AMOUNT : char {
        Single,
        Multiple
    };

    enum class TYPE : char {
        Players,
        Entities
    };

    AMOUNT Amount = AMOUNT::Single;
    TYPE Type = TYPE::Players;
} MCE_Prop;

typedef struct STR {
    std::wstring Val;
    bool Use = false;

    operator std::wstring() {
        return Val;
    }

    const std::wstring operator=(const std::wstring New_Val) {
        this->Val = New_Val;
        Use = true;
        return this->Val;
    }
} STR;

typedef struct ROTATION {
    RANGE VRotation;
    RANGE HRotation;
} ROTATION;

typedef struct SELECT_AREA {
    RANGE Radius;
    bool Use_Radius = false;
    MC_NType DX, DY, DZ;
    bool Use_Volume = false;

} SELECT_AREA;

typedef struct COORDINATES {
    MC_NType X, Y, Z;
} COORDINATES;


enum class SORT : char {
    Arbitrary,
    Furthest,
    Nearest,
    Random
};

enum class Selection_Type : char {
    Radius,
    Volume
};

enum class GAMEMODE : char {
    Adventure,
    Creativ,
    Spectator,
    Survival
};



typedef struct MC_Entity {
    MCE_Prop Ent_Prop;
    MC_NType Limit;


    RANGE XP_Level;
    COORDINATES Coordinates;
    SELECT_AREA Select_Area;
    ROTATION Rotation;
    SORT Sort = SORT::Nearest;
    GAMEMODE Gamemode = GAMEMODE::Creativ;
    STR Team;
    STR Name;
    STR NBT;
    STR Advancements;
    STR Tags;
    STR Scores;
} MC_Entity;
