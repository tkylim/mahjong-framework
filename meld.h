#ifndef MELD_H
#define MELD_H

#include <QVector>

#include "tile.h"

namespace game {

class Meld
{
public:
    enum MeldType {
        CHI,
        CHA,
        GANG,
        INVALID
    };

    Meld();
    Meld(MeldType type, Tile first, Tile second, Tile third);
    Meld(Tile first, Tile second, Tile third, Tile fourth); // Gang

    static Meld Empty();

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

typedef QVector<game::Meld> Melds;

Q_DECLARE_METATYPE(Melds)
Q_DECLARE_METATYPE(game::Meld);
#endif // MELD_H
