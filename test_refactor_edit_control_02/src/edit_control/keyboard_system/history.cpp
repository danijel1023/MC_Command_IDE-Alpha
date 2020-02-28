#include "pch.h"
#include "edit_control/_edit_control/edit_control.h"
#include "history_defines.h"


void Keyboard::Register_History(unsigned char Mode, Caret_Struct& Caret, Caret_Struct& CaretSel,
                                const std::vector<std::wstring>& Text,
                                const std::vector<std::wstring>& Aditional) {
    m_History.erase(m_History.begin() + m_History_Pointer, m_History.end());
    History_Struct History;

    History.Mode = Mode;
    History.Caret = Caret;
    History.CaretSel = CaretSel;

    switch (Mode) {
    case HISTORY_MSG_DEL_SEL:
        Extract_Selected(Text, Caret, CaretSel, &History.Text);
        break;
    case HISTORY_MSG_DEL_SEL_INSERT:
        History.Text = Text;
        History.Del_Sel_Insert_Text = Aditional;
        break;
    case HISTORY_MSG_INSERT:
        History.Text = Text;
        break;
    case HISTORY_MSG_DELETE:
        History.Text = Text;
        break;
    case HISTORY_MSG_BACKSPACE:
        History.Text = Text;
        break;
    }

    m_History.push_back(History);
    m_History_Pointer = m_History.size();
}


void Keyboard::UnDo() {
    if (m_History_Pointer == 0) return;
    auto& History = m_History.at(m_History_Pointer - 1);

    switch (History.Mode) {
    case HISTORY_MSG_DEL_SEL:
        //Insert deleted text on starting position (lowest in coord)
        Insert(History.Text, (History.Caret < History.CaretSel) ? History.Caret : History.CaretSel);
        m_EC.m_Caret = History.Caret;
        m_EC.m_CaretSel = History.CaretSel;
        break;

    case HISTORY_MSG_DEL_SEL_INSERT:
        {
            Caret_Struct Start, End;
            m_EC.Differentiate_Start_End(&Start, &End, History.CaretSel, History.Caret);
            auto Del_End = Start;
            if (History.Text.size() == 1) {
                Del_End.X += History.Text.at(0).size();
            } else {
                Del_End.Y += History.Text.size() - 1;
                Del_End.X = History.Text.back().size();
            }
            Delete_Selected(Start, Del_End, false);

            Insert(History.Del_Sel_Insert_Text, Start);
            m_EC.m_Caret = History.Caret;
            m_EC.m_CaretSel = History.CaretSel;
        }
        break;

    case HISTORY_MSG_INSERT:
        {
            auto CaretSel = History.CaretSel;

            if (History.Text.size() == 1 && History.Text.at(0) == (std::wstring() = VK_RETURN)) {
                CaretSel.X = 0;
                CaretSel.Y++;   //Move caret to next line (+1 pos in text coord)
            } else {
                if (History.Text.size() == 1) {
                    CaretSel.X += History.Text.at(0).size();
                } else {
                    CaretSel.Y += History.Text.size() - 1;  //Y coords are elements in vector so when using '.size()' on
                                                            //Y coord, the returned value needs to be reduced by 1
                    CaretSel.X = History.Text.back().size();//X coords are from 0 to '.size()' so no additional processing is needed
                }
            }

            //I'm lazy so I'll use a bit slow-ish functions instead of writing more efficient one(s)
            Delete_Selected(History.Caret, CaretSel, false);  //Delete without recording history
        }
        break;
        //code that is self explanatory doesn't need comments and is 'good code', but code without comments is bad code
    case HISTORY_MSG_DELETE:
        if (History.Text.at(0) == (std::wstring() = VK_RETURN)) {
            //By inserting 2 empty strings, 'Insert' func will just break the line and move all left text (from caret) to the next line
            Insert({ L"", L"" }, History.Caret);
        } else
            //Just insert deleted text back on caret pos
            Insert(History.Text, History.Caret);

        //Func 'Insert' messes with real caret pos, so just retrun to original pos (History.Caret)
        m_EC.m_Caret = History.Caret;
        m_EC.m_CaretSel = m_EC.m_Caret;
        break;

    case HISTORY_MSG_BACKSPACE:
        if (History.Text.at(0) == (std::wstring() = VK_RETURN)) {
            //By inserting 2 empty strings, 'Insert' func will just break the line and move all left text (from caret) to the next line
            Insert({ L"", L"" }, History.Caret);
        }
        else
            //Just insert deleted text back on caret pos
            Insert(History.Text, History.Caret);
        break;

    }

    m_EC.Corect_Scrolling();
    CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
    m_History_Pointer--;
}


void Keyboard::ReDo() {
    if (m_History_Pointer == m_History.size()) return;
    auto& History = m_History.at(m_History_Pointer);

    switch (History.Mode) {
    case HISTORY_MSG_DEL_SEL:
        Delete_Selected_Unordered(History.Caret, History.CaretSel, false);
        break;

    case HISTORY_MSG_DEL_SEL_INSERT:
        {
            Delete_Selected_Unordered(History.Caret, History.CaretSel, false);

            Caret_Struct Start, End;
            m_EC.Differentiate_Start_End(&Start, &End, History.CaretSel, History.Caret);
            Insert(History.Text, Start);
        }
        break;

    case HISTORY_MSG_INSERT:
        //Func 'Insert' will insert all the text and align caret accordingly
        Insert(History.Text, History.Caret);
        break;

    case HISTORY_MSG_DELETE:
        {
            auto CaretSel = History.CaretSel;
            if (History.Text.at(0) != (std::wstring() = VK_RETURN)) {
                CaretSel.X++;
            } else {
                CaretSel.Y++;
                CaretSel.X = 0;
            }
            //I'm lazy so I'll use a bit slow-ish functions instead of writing more efficient one(s)
            Delete_Selected(History.Caret, CaretSel, false);  //Delete without recording history
        }
        break;

    case HISTORY_MSG_BACKSPACE:
        {
            auto CaretSel = History.CaretSel;
            if (History.Text.at(0) != (std::wstring() = VK_RETURN)) {
                CaretSel.X++;
            } else {
                CaretSel.Y++;
                CaretSel.X = 0;
            }
            //I'm lazy so I'll use a bit slow-ish functions instead of writing more efficient one(s)
            Delete_Selected(History.Caret, CaretSel, false);  //Delete without recording history
        }
        break;
    }

    m_EC.Corect_Scrolling();
    CHECK_ERR(InvalidateRect(m_hWnd, NULL, TRUE), ERR_MSG_INVALIDATE_RECT);
    m_History_Pointer++;
}
