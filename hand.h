#ifndef HAND_H
#define HAND_H

#include <vector>

#include "meld.h"
#include "tile.h"


namespace game {

class Hand
{
public:
    Hand();

    enum HandState {
        NORMAL,
        ONE_TILE_OFF,
        WINNING
    };

    void Draw(Tile t);
    Tile Discard(const Tile& t);
    bool IsOpen() const;
    void AddMeld(const Meld& meld);

    const std::vector<Tile>& GetTiles() const;

    HandState GetState() const;
    void SetState(const HandState &value);

    std::vector<Meld> GetMelds() const;

private:
    std::vector<Meld> melds;
    std::vector<Tile> tiles;
    bool isOpen = false;
    HandState state = NORMAL;
};

}

#endif // HAND_H
