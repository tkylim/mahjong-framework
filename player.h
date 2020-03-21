#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "hand.h"

namespace game {

class Player
{
public:
    Player(PlayerId id, Hand *p_hand);
    Tile GetDiscardBlocking();
    Hand *p_hand;

private:
    PlayerId id;
};

}

#endif // PLAYER_H
