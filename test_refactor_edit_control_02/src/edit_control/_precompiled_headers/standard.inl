bool Std::Is_Lowercase(wchar_t& ch) {
    switch (ch) {
    case L'a': break;
    case L'b': break;
    case L'c': break;
    case L'd': break;
    case L'e': break;
    case L'f': break;
    case L'g': break;
    case L'h': break;
    case L'i': break;
    case L'j': break;
    case L'k': break;
    case L'l': break;
    case L'm': break;
    case L'n': break;
    case L'o': break;
    case L'p': break;
    case L'q': break;
    case L'r': break;
    case L's': break;
    case L't': break;
    case L'u': break;
    case L'v': break;
    case L'w': break;
    case L'x': break;
    case L'y': break;
    case L'z': break;
    default: return false;
    }
    return true;
}


bool Std::Is_Uppercase(wchar_t& ch) {
    switch (ch) {
    case L'A': break;
    case L'B': break;
    case L'C': break;
    case L'D': break;
    case L'E': break;
    case L'F': break;
    case L'G': break;
    case L'H': break;
    case L'I': break;
    case L'J': break;
    case L'K': break;
    case L'L': break;
    case L'M': break;
    case L'N': break;
    case L'O': break;
    case L'P': break;
    case L'Q': break;
    case L'R': break;
    case L'S': break;
    case L'T': break;
    case L'U': break;
    case L'V': break;
    case L'W': break;
    case L'X': break;
    case L'Y': break;
    case L'Z': break;
    default: return false;
    }
    return true;
}


bool Std::Is_Number(wchar_t& ch) {
    switch (ch) {
    case L'0': break;
    case L'1': break;
    case L'2': break;
    case L'3': break;
    case L'4': break;
    case L'5': break;
    case L'6': break;
    case L'7': break;
    case L'8': break;
    case L'9': break;
    default: return false;
    }
    return true;
}


bool Std::Has_Namespace(std::wstring& Word) {
    size_t Word_Size = Word.size();
    for (size_t i = 0; i < Word_Size; i++) {
        wchar_t ch = Word.at(i);

        if (ch == L':') {
            if (i == 0)
                return false;
            return true;
        }

        else if (!Std::Is_Std_ch(ch)) return false;
    }

    return false;
}


bool Std::Insert_Namespace(std::wstring& Word, std::wstring* Error) {
    if (Std::Has_Namespace(Word)) return true;

    if (Word.at(0) == L':') Word.insert(0, L"minecraft");
    else Word.insert(0, L"minecraft:");

    bool Pass_NS = false;
    size_t Word_Size = Word.size();
    for (size_t i = 0; i < Word_Size; i++) {
        wchar_t ch = Word.at(i);
        if (Pass_NS) {
            if (ch == L':') {
                *Error = L"Cannot have two or more ':' in a namespace value";
                return false;
            }
        }

        else {
            if (ch == L':') Pass_NS = true;
            else if (!Std::Is_Std_ch(ch)) {
                *Error = L"Ilegal char in namespace value";
                return false;
            }
        }
    }

    return true;
}


bool Std::Is_Std_ch(wchar_t ch) {
    return (Is_Lowercase(ch) || Is_Uppercase(ch) || Is_Number(ch)
            || ch == L'+' || ch == L'-' || ch == L'_');
}


bool Std::Is_Std_Str(std::wstring Str) {
    size_t Str_Size = Str.size();
    for (size_t i = 0; i < Str_Size; i++) {
        if (!Is_Std_ch(Str.at(i))) return false;
    }

    return true;
}


void Std::Clear_Spaces(std::wstring* Word_Ptr) {
    auto& Word = *Word_Ptr;
    size_t Word_Size = Word.size();
    for (size_t i = 0; i < Word_Size; i++) {
        if (Word.at(i) != L' ') {
            Word.erase(0, i);
            break;
        }
    }

    Word_Size = Word.size();
    for (size_t i = Word_Size; i > 0; i--) {
        if (Word.at(i - 1) != L' ') {
            Word.erase(i, Word_Size - i);
            break;
        }
    }
}


bool Std::Check_Spaces(std::wstring& Word) {
    bool Start_Space = false, End_Space = false;
    size_t Word_Size = Word.size();
    for (size_t i = 0; i < Word_Size; i++) {
        wchar_t ch = Word.at(i);

        if (ch == L' ') {
            if (Start_Space) {
                End_Space = true;
            }
        }

        else {
            if (!Start_Space) {
                Start_Space = true;
            }

            if (End_Space) {
                return false;
            }
        }
    }

    //if Start_Space is false, it means it didn't find any letters
    return Start_Space;
}


bool Std::Is_Valid_Double(std::wstring& Word) {
    bool Has_Decimal = false;
    size_t Word_Size = Word.size();
    for (size_t i = (Word.at(0) == L'-'); i < Word_Size; i++) {
        if (Word.at(i) == L'.') {
            if (Has_Decimal) return false;
            Has_Decimal = true;
        }

        else if (!Is_Number(Word.at(i))) return false;
    }

    return true;
}


bool Std::Is_Valid_Integer(std::wstring& Word) {

    size_t Word_Size = Word.size();
    for (size_t i = (Word.at(0) == L'-'); i < Word_Size; i++) {
        if (!Is_Number(Word.at(i))) return false;
    }

    return true;
}
