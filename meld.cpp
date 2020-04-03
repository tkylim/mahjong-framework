#include "meld.h"

namespace game {
    Meld::Meld(Tile first, Tile second, Tile third, Tile fourth) : first(first), second(second), third(third), fourth(fourth)
    {
        type = GANG;
    }

    Meld::Meld(MeldType type, Tile first, Tile second, Tile third) : type(type), first(first), second(second), third(third), fourth(Tile::GetBlankTile())
    {}

    Meld::Meld() {
        type = INVALID;
    }

    Meld Meld::Empty()
    {
        return Meld();
    }

    bool Meld::Contains(const Tile& t) const
    {
        return t == first || t == second || t == third || (type == GANG && t == fourth);
    }

    Tile Meld::GetFirst() const
    {
        return first;
    }

    Tile Meld::GetSecond() const
    {
        return second;
    }

    Tile Meld::GetThird() const
    {
        return third;
    }

    Tile Meld::GetFourth() const
    {
        return fourth;
    }

    Meld::MeldType Meld::GetType() const
    {
        return type;
    }
}
