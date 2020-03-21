#ifndef MAHJONGHELPER_H
#define MAHJONGHELPER_H

#include "globals.h"
#include "hand.h"

namespace game {

// TODO: refactor into namespace
class MahjongHelper
{
public:
    MahjongHelper();

    static std::vector<Meld> IsTileChiable(const Hand& hand, Tile discarded);
    static bool IsTileChaable(const Hand& hand, Tile discarded);
    static bool IsTileGangable(const Hand& hand, Tile discarded);
    static Hand::HandState UpdateHandState(const Hand& hand, Tile t, bool IsDrawPeriod = false);

    static unsigned int CalculateScore();
};

}

#endif // MAHJONGHELPER_H
