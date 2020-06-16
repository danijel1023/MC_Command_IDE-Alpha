#include "pch.h"
#include "edit_control/_edit_control/edit_control.h"

IntelliSense::IntelliSense(Edit_Control& EC)
    :m_EC(EC)
{}

IntelliSense::~IntelliSense() {
    KillTimer(m_Parent, TIMER_ID_TWO_SEC_TIMER);
}


void IntelliSense::Init(HWND Parent) {
    Log_IO::Start_Log_System obj(SYSTEM_ID_INTELLISENSE);
    Log_IO::wcout() << L"Init" << std::endl;

    m_InitCalls++;
    if (m_Is_Initialized) {
        MB_ERR("Initialization Function was Called More than 1 Time!");
        return;
    }
    m_Parent = Parent;

    Json IS_Linker(L"intellisense_structures/linker.json");
    auto& Commands = IS_Linker.Root().Obj(L"Commands").Str();
    auto& Minecraft_Block_Predicate = IS_Linker.Root().Obj(L"Minecraft_Block_Predicate").Str();
    auto& Minecraft_Block_State = IS_Linker.Root().Obj(L"Minecraft_Block_State").Str();
    auto& Minecraft_Color = IS_Linker.Root().Obj(L"Minecraft_Color").Str();
    auto& Minecraft_Dimension = IS_Linker.Root().Obj(L"Minecraft_Dimension").Str();
    auto& Minecraft_Entity = IS_Linker.Root().Obj(L"Minecraft_Entity").Str();
    auto& Minecraft_Entity_Anchor = IS_Linker.Root().Obj(L"Minecraft_Entity_Anchor").Str();
    auto& Minecraft_Entity_Summon = IS_Linker.Root().Obj(L"Minecraft_Entity_Summon").Str();
    auto& Minecraft_Entity_Type = IS_Linker.Root().Obj(L"Minecraft_Entity_Type").Str();
    auto& Minecraft_Item_Enchantment = IS_Linker.Root().Obj(L"Minecraft_Item_Enchantment").Str();
    auto& Minecraft_Item_Predicate = IS_Linker.Root().Obj(L"Minecraft_Item_Predicate").Str();
    auto& Minecraft_Item_Stack = IS_Linker.Root().Obj(L"Minecraft_Item_Stack").Str();
    auto& Minecraft_Mob_Effect = IS_Linker.Root().Obj(L"Minecraft_Mob_Effect").Str();
    auto& Minecraft_Objective_Criteria = IS_Linker.Root().Obj(L"Minecraft_Objective_Criteria").Str();
    auto& Minecraft_Operation = IS_Linker.Root().Obj(L"Minecraft_Operation").Str();
    auto& Minecraft_Particle = IS_Linker.Root().Obj(L"Minecraft_Particle").Str();
    auto& Minecraft_Scoreboard_Slot = IS_Linker.Root().Obj(L"Minecraft_Scoreboard_Slot").Str();

    Log_IO::wcout() << L"Starting Compiling IntelliSence structures" << std::endl;
    m_Syntax_Obj.Init(Commands);
    m_Minecraft_Block_Predicate_Obj.Init(Minecraft_Block_Predicate);
    m_Minecraft_Block_State_Obj.Init(Minecraft_Block_State);
    m_Minecraft_Color_Obj.Init(Minecraft_Color);
    m_Minecraft_Dimension_Obj.Init(Minecraft_Dimension);
    m_Minecraft_Entity_Obj.Init(Minecraft_Entity);
    m_Minecraft_Entity_Anchor_Obj.Init(Minecraft_Entity_Anchor);
    m_Minecraft_Entity_Summon_Obj.Init(Minecraft_Entity_Summon);
    m_Minecraft_Entity_Type_Obj.Init(Minecraft_Entity_Type);
    m_Minecraft_Item_Enchantment_Obj.Init(Minecraft_Item_Enchantment);
    m_Minecraft_Item_Predicate_Obj.Init(Minecraft_Item_Predicate);
    m_Minecraft_Item_Stack_Obj.Init(Minecraft_Item_Stack);
    m_Minecraft_Mob_Effect_Obj.Init(Minecraft_Mob_Effect);
    m_Minecraft_Objective_Criteria_Obj.Init(Minecraft_Objective_Criteria);
    m_Minecraft_Operation_Obj.Init(Minecraft_Operation);
    m_Minecraft_Particle_Obj.Init(Minecraft_Particle);
    m_Minecraft_Scoreboard_Slot_Obj.Init(Minecraft_Scoreboard_Slot);

    Log_IO::wcout() << L"Completed Compiling IntelliSence structures" << std::endl;

}


void IntelliSense::Proc_Msg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System Obj(SYSTEM_ID_INTELLISENSE);
    m_EC.m_Dispatcher.Return = false;

    switch (uMsg) {
    case WM_CHAR:
        SetTimer(m_Parent, TIMER_ID_TWO_SEC_TIMER, 2 * (1000), NULL);
        WM_Char(wParam, lParam);
        if (m_EC.m_Dispatcher.Return) return;
        break;

    case WM_TIMER:
        KillTimer(m_Parent, TIMER_ID_TWO_SEC_TIMER);

        switch (wParam) {
        case TIMER_ID_TWO_SEC_TIMER:
            Two_Sec_Timer();
            if (m_EC.m_Dispatcher.Return) return;
            break;
        }
    }
}

void IntelliSense::Proc_Msg_After(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System Obj(SYSTEM_ID_INTELLISENSE);
    m_EC.m_Dispatcher.Return = false;

    switch (uMsg) {
    case WM_KEYDOWN:
        if (HIBYTE(GetKeyState(VK_CONTROL)) == 0) {
            switch (wParam) {
            case VK_LEFT:
            case VK_DELETE:
            case VK_BACK:
                Analise_Line(m_EC.m_Text.at(m_EC.m_Caret.Y), m_EC.m_TextColor.at(m_EC.m_Caret.Y));
                if (Error_Handler.Has_Error()) {
                    Log_IO::Set_Color::Error();
                    Log_IO::wcout() << Error_Handler.Get_Error() << std::endl;
                    Log_IO::Set_Color::Default();
                }

                CHECK_ERR(InvalidateRect(m_Parent, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
                break;
            }
        }
        if (m_EC.m_Dispatcher.Return) return;
        break;

    case WM_CHAR:
        Analise_Line(m_EC.m_Text.at(m_EC.m_Caret.Y), m_EC.m_TextColor.at(m_EC.m_Caret.Y));
        if (Error_Handler.Has_Error()) {
            Log_IO::Set_Color::Error();
            Log_IO::wcout() << Error_Handler.Get_Error() << std::endl;
            Log_IO::Set_Color::Default();
        }
        CHECK_ERR(InvalidateRect(m_Parent, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
        break;
    }
}


void IntelliSense::WM_KeyDown(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System Obj(SYSTEM_ID_INTELLISENSE);

    m_EC.m_Dispatcher.Return = false;
}


void IntelliSense::WM_Char(WPARAM wParam, LPARAM lParam) {
    Log_IO::Start_Log_System Obj(SYSTEM_ID_INTELLISENSE);
    Log_IO::wcout() << "WM_Char: " << (wchar_t)wParam << " - " << lParam << std::endl;

    m_EC.m_Dispatcher.Return = false;
}


void IntelliSense::Two_Sec_Timer() {
    Log_IO::Start_Log_System Obj(SYSTEM_ID_INTELLISENSE);
    Log_IO::wcout() << "Two_Sec_Timer" << std::endl;

    m_EC.m_Dispatcher.Return = false;
}


COLORREF IntelliSense::Get_Color(std::wstring Type) {
    if (Type == L"INCOMPLETE") return RGB(255, 0, 255);
    return RGB(0, 255, 40);
}


void IntelliSense::Start_New_Line() {
    Error_Handler.Get_Error();
    m_Syntax_Obj.Root();

    m_Paraser_Func_Name = L"";
    m_Paraser_Set_Lock = false;
    m_Paraser_Func_Counter = 0;
}


bool IntelliSense::Generic_ArrElm_Search(Json& Obj, std::wstring Word) {
    Obj.Root();

    size_t Size = Obj.Size();
    for (size_t i = 0; i < Size; i++) {
        if (Obj.Arr(i).Str() == Word) return true;
        Obj.Back();
    }

    return false;
}


bool IntelliSense::Generic_Split(std::wstring Words, std::vector<std::wstring>* Output_Ptr) {
    auto& Output = *Output_Ptr;

    size_t Last_Pos = 0;
    std::wstring Brackets;
    size_t Words_Size = Words.size();
    for (size_t Pos = 0; Pos < Words_Size; Pos++) {
        wchar_t ch = Words.at(Pos);

        switch (ch) {
        case L'[': Brackets.push_back(L'['); break;
        case L'{': Brackets.push_back(L'{'); break;

        default:
            if (Brackets.size() == 0 && (ch == L']' || ch == L')')) {
                Error_Handler << L"Brackets don't add up";
                return false;
            }

            else {
                switch (ch) {
                case L'}':
                    if (Brackets.back() == L'{') Brackets.pop_back();
                    else {
                        Error_Handler << L"Brackets don't add up";
                        return false;
                    }
                    break;
                case L']':
                    if (Brackets.back() == L'[') Brackets.pop_back();
                    else {
                        Error_Handler << L"Brackets don't add up";
                        return false;
                    }
                    break;
                }
            }
        }

        if ((ch == L' ' || Pos + 1 == Words_Size) && Brackets.size() == 0) {
            Output.push_back(Words.substr(Last_Pos, Pos - Last_Pos + (ch != L' ')));
            Last_Pos = Pos + 1;
        }
    }

    return true;
}
