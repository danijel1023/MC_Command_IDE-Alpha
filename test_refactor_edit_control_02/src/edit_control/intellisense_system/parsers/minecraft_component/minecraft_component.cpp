#include "pch.h"
#include "intelisense.h"

static bool Right_Format(Json& Comp);

bool IntelliSense::Minecraft_Component(std::wstring& Word) {
    Json Comp;
    Comp.RD_Init(Word);

    if (Comp.Is_Halted()) {
        std::wcout << L"Invalid JSON format";
        return false;
    }

    if (!Right_Format(Comp)) {
        std::wcout << L"Invalid JSON-chat format";
        return false;
    }

    return true;
}

static bool Right_Format(Json& Comp) {
    if (Comp.Is_Obj()) {
        if (Comp.Has_Name(L"text")) {
            Comp.Obj(L"text");
            if (!Comp.Is_Str()) return false;
            Comp.Back();
        } else return false;

        if (Comp.Has_Name(L"extra")) {
            Comp.Obj(L"extra");
            if (!Comp.Is_Arr()) return false;
            if (Comp.Size() == 0) return false;

            if (!Right_Format(Comp)) return false;
            Comp.Back();
        }
    }

    else if (Comp.Is_Arr()) {
        if (Comp.Size() == 0) return false;

        size_t Comp_Size = Comp.Size();
        for (size_t i = 0; i < Comp_Size; i++) {
            Comp.Arr(i);
            if (!Right_Format(Comp)) return false;
            Comp.Back();
        }
    }

    return true;
}
