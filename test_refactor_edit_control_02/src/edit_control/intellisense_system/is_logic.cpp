#include "pch.h"
#include "intelisense.h"


void IntelliSense::Analise_Line(std::wstring& Line, std::vector<COLORREF>& Colors) {
    m_Syntax_Obj.Root();
    m_Syntax_Obj.Obj(L"children");

    std::wstring Brackets;
    size_t Last_Pos = 0;
    bool New_Last_Pos = true;
    size_t Line_Size = Line.size();
    for (size_t Pos = 0; Pos < Line_Size; Pos++) {
        wchar_t ch = Line.at(Pos);

        switch (ch) {
        case L'(': Brackets.push_back(L'('); break;
        case L'[': Brackets.push_back(L'['); break;
        case L'{': Brackets.push_back(L'{'); break;

        case L'}':
            if (Brackets.back() == L'{') Brackets.pop_back();
            else{
                for (size_t CPos = Last_Pos; CPos < Colors.size(); CPos++)
                    Colors.at(CPos) = RGB(255, 0, 0);
                //brackets don't add up
                return;
            }
            break;
        case L']':
            if (Brackets.back() == L'[') Brackets.pop_back();
            else {
                for (size_t CPos = Last_Pos; CPos < Colors.size(); CPos++)
                    Colors.at(CPos) = RGB(255, 0, 0);
                //brackets don't add up
                return;
            }
            break;
        case L')':
            if (Brackets.back() == L'(') Brackets.pop_back();
            else {
                for (size_t CPos = Last_Pos; CPos < Colors.size(); CPos++)
                    Colors.at(CPos) = RGB(255, 0, 0);
                //brackets don't add up
                return;
            }
            break;
        }

        
        if (ch == L' ' || Pos + 1 == Line_Size) {
            std::wstring Word = Line.substr(Last_Pos, Pos - Last_Pos + (ch != L' '));   //Get correct word
            auto Names = m_Syntax_Obj.Get_Names();  //"effect", "give", "tp", ...

            bool Found_Match = false;
            for (size_t NPos = 0; NPos < Names.size(); NPos++) {
                m_Syntax_Obj.Obj(Names.at(NPos)).Obj(L"type");
                //"type"

                if (m_Syntax_Obj.Str() == L"literal") {
                    if (Names.at(NPos) == Word) {
                        for (size_t CPos = Last_Pos; CPos < Pos + 1; CPos++)
                            Colors.at(CPos) = RGB(0, 255, 40);
                        Found_Match = true;
                        break;
                    }
                } else if (m_Syntax_Obj.Str() == L"argument") {
                    m_Syntax_Obj.Back().Obj(L"parser");

                    if (m_Syntax_Obj.Str() == L"minecraft:mob_effect") {

                    } else if (m_Syntax_Obj.Str() == L"minecraft:entity") {

                    }
                }
                m_Syntax_Obj.Back().Back();
            }


            if (!Found_Match) {
                for (size_t CPos = Last_Pos; CPos < Colors.size(); CPos++)
                    Colors.at(CPos) = RGB(255, 0, 0);
                //If no match is found, ignore rest of the line
                return;
            }
            else {
                //m_Syntax_Obj.Back().Obj(L"children");
            }

            if (New_Last_Pos) Last_Pos = Pos + 1;
            New_Last_Pos = true;
        }
    }

}
