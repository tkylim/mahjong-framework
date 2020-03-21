#include "player.h"

// Remove when proper implementation comes
#include <stdlib.h>

namespace game {

Player::Player(PlayerId id, Hand *p_hand) : id(id), p_hand(p_hand)
{

}


Tile Player::GetDiscardBlocking()
{
    // TODO fix randomness
    return p_hand->GetTiles()[rand() % p_hand->GetTiles().size()];
}

}
