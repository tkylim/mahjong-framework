#include "player.h"

// Remove when proper implementation comes
#include <stdlib.h>

namespace game {

Player::Player(PlayerId id, Hand *p_hand) : id(id), p_hand(p_hand)
{

}


Tile Player::GetDiscardBlocking()
{
    // TODO: Can't discard melds
    // TODO fix randomness
    bool r = true;
    Tile t(0, Tile::TileType::UNKNOWN);
    while (r) {
        t = p_hand->GetTiles()[rand() % p_hand->GetTiles().size()];
        r = false;
        for (const Meld& m : p_hand->GetMelds()) {
            if (m.Contains(t)) {
                r = true;
                break;
            }
        }
    }
    return t;
}

}
