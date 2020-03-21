#include <algorithm>

#include "hand.h"

namespace game {

Hand::Hand()
{
}


const std::vector<Tile>& Hand::GetTiles() const
{
    return tiles;
}

Hand::HandState Hand::GetState() const
{
    return state;
}

void Hand::SetState(const Hand::HandState &state)
{
    this->state = state;
}

void Hand::Draw(Tile t)
{
    tiles.push_back(t);
    std::sort(tiles.begin(), tiles.end());
}

// Returns the discarded tile
Tile Hand::Discard(const Tile& t)
{
    auto it = std::find(tiles.begin(), tiles.end(), t);

    if (it == tiles.end())
        return Tile(0, Tile::TileType::UNKNOWN);

    tiles.erase(it);
    return t;
}

bool Hand::IsOpen() const
{
    return isOpen;
}

void Hand::AddMeld(const Meld& meld)
{
    if (!IsOpen())
        isOpen = true;
    melds.push_back(meld);
}

}
