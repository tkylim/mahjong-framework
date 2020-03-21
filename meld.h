#ifndef MELD_H
#define MELD_H

#include "tile.h"

namespace game {

class Meld
{
public:
    enum MeldType {
        CHI,
        CHA,
        GANG
    };

    Meld(MeldType type, Tile first, Tile second, Tile third);
    Meld(Tile first, Tile second, Tile third, Tile fourth); // Gang

    bool Contains(const Tile& t) const;

    Tile GetFirst() const;

    Tile GetSecond() const;

    Tile GetThird() const;

    Tile GetFourth() const;

    MeldType GetType() const;

private:
    MeldType type;
    Tile first;
    Tile second;
    Tile third;
    Tile fourth;
};

}

#endif // MELD_H
