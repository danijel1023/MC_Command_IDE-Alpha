#include "pch.h"
#include "state_formater.h"


bool State_Formater::Seperate_State(std::wstring& Word, std::vector<std::wstring>* Params_Ptr, std::wstring* Error, wchar_t&& Open, wchar_t&& Close) {
    auto& Params = *Params_Ptr;

    if (Word.size() < 2) return false;
    if (Word.at(0) != Open || Word.back() != Close) return false;


    size_t Ancor = 1;
    std::wstring Brackets;
    bool Looking_For_Eq = true;
    bool Skip = false, Is_Str = false;

    size_t Word_Size = Word.size() - 1;
    for (size_t i = 1; i < Word_Size; i++) {
        wchar_t ch = Word.at(i);
        if (Skip) { Skip = false; continue; }
        if (Is_Str) {
            if (ch == L'\\') {
                Skip = true;
            }

            else if (ch == L'"') {
                Is_Str = false;
            }
        }

        else if (ch == L'"') {
            Is_Str = true;
        }

        else if (ch == L'(' || ch == L'[' || ch == L'{') {
            Brackets.push_back(ch);
        }
        
        else if (ch == L')' || ch == L']' || ch == L'}') {
            if (ch == L')' && Brackets.back() == L'(' ||
                ch == L']' && Brackets.back() == L'[' ||
                ch == L'}' && Brackets.back() == L'{') {
                Brackets.pop_back();
            }
            else {
                *Error = L"Brackets don't match up";
                return false;
            }
            Brackets.pop_back();
        }

        else if (Brackets.size() == 0) {
            if (ch == L',') {
                if (Looking_For_Eq) {
                    *Error = L"Expected '=''";
                    return false;
                }
                Looking_For_Eq = true;

                Params.push_back(Word.substr(Ancor, Ancor - i));
                Ancor = i + 1;
            }
            
            else if (ch == L'=') {
                if (!Looking_For_Eq) {
                    *Error = L"Expected ',' or ']'";
                    return false;
                }
                Looking_For_Eq = false;

                Params.push_back(Word.substr(Ancor, Ancor - i));
                Ancor = i + 1;
            }
        }
    }


    return !Looking_For_Eq;
}
