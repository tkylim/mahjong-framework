#include "mahjonghelper.h"

#include <algorithm>

namespace game {

// Returns sets of 3 tiles length of the return is always a multiple of 3
std::vector<Meld> MahjongHelper::IsTileChiable(const Hand &hand, Tile discarded) {
    const std::vector<Tile>& tiles = hand.GetTiles();
    // Check in negative direction
    unsigned char value = discarded.GetNumericalValue();
    std::vector<Meld> chis;

    // Incrementing chi
    if (value <= 8) {
        auto tilePlusOneIt = std::find(tiles.begin(), tiles.end(), Tile(0, static_cast<Tile::TileType>(discarded.GetType() + 1)));
        if (tilePlusOneIt != tiles.end()) {  // D, D+1
            if (value <= 7) {
                auto tilePlusTwoIt = std::find(tiles.begin(), tiles.end(), Tile(0, static_cast<Tile::TileType>(discarded.GetType() + 2)));
                if (tilePlusTwoIt != tiles.end()) {  // D, D+1, D+2
                    chis.push_back(Meld(Meld::MeldType::CHI, discarded, *tilePlusOneIt, *tilePlusTwoIt));
                }
            }
            auto tileMinusOneIt = std::find(tiles.begin(), tiles.end(), Tile(0, static_cast<Tile::TileType>(discarded.GetType() - 1)));
            if (tileMinusOneIt != tiles.end()) {  // D-1, D, D+1
                chis.push_back(Meld(Meld::MeldType::CHI, *tileMinusOneIt, discarded, *tilePlusOneIt));
            }
        }
    }
    // Decrementing chi
    if (value >= 3) {
        auto tileMinusOneIt = std::find(tiles.begin(), tiles.end(), Tile(0, static_cast<Tile::TileType>(discarded.GetType() - 1)));
        if (tileMinusOneIt != tiles.end()) {  // D-1, D
            auto tileMinusTwoIt = std::find(tiles.begin(), tiles.end(), Tile(0, static_cast<Tile::TileType>(discarded.GetType() - 2)));
            if (tileMinusTwoIt != tiles.end()) {  // D-2, D-1, D
                chis.push_back(Meld(Meld::MeldType::CHI, *tileMinusTwoIt, *tileMinusOneIt, discarded));
            }
        }
    }

    return chis;
}

bool MahjongHelper::IsTileChaable(const Hand& hand, Tile discarded) {
    if (std::count(hand.GetTiles().begin(), hand.GetTiles().end(), Tile(0, discarded.GetType())) >= 2)
        return true;

    return false;
}

bool MahjongHelper::IsTileGangable(const Hand& hand, Tile discarded) {
    if (std::count(hand.GetTiles().begin(), hand.GetTiles().end(), Tile(0, discarded.GetType())) >= 3)
        return true;

    return false;
}

Hand::HandState MahjongHelper::UpdateHandState(const Hand& hand, Tile t, bool IsDrawPeriod) {
    // TODO run python script to check
    return Hand::HandState::NORMAL;
}

unsigned int MahjongHelper::CalculateScore() {
    // TODO run python script to calculate score
    return 0;
}

}
