#pragma once

struct Caret_Struct {
    size_t X = 0;
    size_t Y = 0;

    bool operator==(const Caret_Struct& Comp) {
        if (this->X == Comp.X && this->Y == Comp.Y) return true;
        else return false;
    }

    //this < Comp
    bool operator<(const Caret_Struct& Comp) {
        if (this->Y > Comp.Y) return false;
        else {
            if (this->Y == Comp.Y) {
                if (this->X < Comp.X) return true;
                else return false;
            } else return true;
        }
    }

    //this > Comp
    bool operator>(const Caret_Struct& Comp) {
        if (this->Y < Comp.Y) return false;
        else {
            if (this->Y == Comp.Y) {
                if (this->X > Comp.X) return true;
                else return false;
            } else return true;
        }
    }

    //this != Comp
    bool operator!=(const Caret_Struct& Comp) {
        if (this->Y == Comp.Y && this->X == Comp.X) return false;
        else return true;
    }
};
