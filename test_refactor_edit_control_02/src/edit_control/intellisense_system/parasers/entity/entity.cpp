#include "pch.h"
#include "intelisense.h"
#include "entity_struct.h"


static std::wstring AllowedChars =  L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
bool IntelliSense::Check_Minecraft_Entity(std::wstring& Word) {
    if (Word.size() == 0) {
        Log_IO::wcout() << "Unknown Error - 'Check_Minecraft_Entity', 'Word.size() == 0'" << std::endl;
        return false;
    }

    if (Word.size() > 16) {
        Log_IO::wcout() << "Too long Name: " << Word.size() << std::endl;
        return false;
    }

    MC_Entity MCE;
    if (m_Syntax_Obj.Has_Name(L"properties")) {
        m_Syntax_Obj.Obj(L"properties");

        if (m_Syntax_Obj.Has_Name(L"type")) {
            m_Syntax_Obj.Obj(L"type");
            std::wstring Type = m_Syntax_Obj.Str();

            if (Type == L"players") {
                MCE.Ent_Prop.Type = MCE_Prop::TYPE::Players;
            } else if (Type == L"entities") {
                MCE.Ent_Prop.Type = MCE_Prop::TYPE::Entities;
            } else {
                Log_IO::wcout() << L"Expected value 'players' or 'entities', got: " << Type << std::endl;
            }

            m_Syntax_Obj.Back();
        }

        if (m_Syntax_Obj.Has_Name(L"amount")) {
            m_Syntax_Obj.Obj(L"amount");
            std::wstring Amount = m_Syntax_Obj.Str();

            if (Amount == L"single") {
                MCE.Ent_Prop.Amount = MCE_Prop::AMOUNT::Single;
            } else if (Amount == L"multiple") {
                MCE.Ent_Prop.Amount = MCE_Prop::AMOUNT::Multiple;
            } else {
                Log_IO::wcout() << L"Expected value 'single' or 'multiple', got: " << Amount << std::endl;
            }

            m_Syntax_Obj.Back();
        }
        m_Syntax_Obj.Back();

        if (Word.at(0) != L'@') {
            if (Word.find_first_not_of(AllowedChars) != std::wstring::npos) {
                Log_IO::wcout() << L"Name containing ilegal char(s)" << std::endl;
                return false;
            } else return true;
        } else {
            if (Word.size() == 1) {
                Log_IO::wcout() << L"Name containing ilegal char(s)" << std::endl;
                return false;
            } else {
                switch (Word.at(1)) {
                case L'a':
                    MCE.Ent_Prop.Type = MCE_Prop::TYPE::Players;
                    MCE.Ent_Prop.Amount = MCE_Prop::AMOUNT::Multiple;
                    MCE.Sort = SORT::Unset;
                    break;

                case L'e':
                    MCE.Ent_Prop.Type = MCE_Prop::TYPE::Entities;
                    MCE.Ent_Prop.Amount = MCE_Prop::AMOUNT::Multiple;
                    MCE.Sort = SORT::Unset;
                    break;

                case L'p':
                    MCE.Ent_Prop.Type = MCE_Prop::TYPE::Players;
                    MCE.Ent_Prop.Amount = MCE_Prop::AMOUNT::Single;
                    MCE.Sort = SORT::Nearest;
                    break;

                case L'r':
                    MCE.Ent_Prop.Type = MCE_Prop::TYPE::Players;
                    MCE.Ent_Prop.Amount = MCE_Prop::AMOUNT::Single;
                    MCE.Sort = SORT::Random;
                    break;

                case L's':
                    MCE.Ent_Prop.Type = MCE_Prop::TYPE::Entities;
                    MCE.Ent_Prop.Amount = MCE_Prop::AMOUNT::Single;
                    MCE.Sort = SORT::Unset;
                    break;

                default:
                    Log_IO::wcout() << L"Name containing ilegal char(s)" << std::endl;
                    return false;
                }
            }
        }
    }

    return true;
}
