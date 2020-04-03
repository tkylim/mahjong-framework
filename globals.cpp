#include "globals.h"

PlayerId AdvancePlayerId(PlayerId p) {
    PlayerId p2 = static_cast<PlayerId>(p + 1);
    if (p2 == INVALID)
        return WIND_EAST;
    return p2;
}
