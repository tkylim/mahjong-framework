#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <vector>

#include "globals.h"
#include "hand.h"
#include "player.h"
#include "tile.h"


namespace game {

#define PLAYER_NUM 4
#define NUM_EACH_TILE 4
#define DEALT_TILES_PER_PLAYER 13

class GameController
{
public:
    GameController();
    ~GameController();

    enum GameState {
        SETUP,
        MAIN,
        EXHAUSTED,
        WIN_EAST,
        WIN_SOUTH,
        WIN_WEST,
        WIN_NORTH
    };

    Tile Draw();
    void Discard(Tile t);
    bool Turn();
    void Loop();

private:
    PlayerId GetNextPlayer();
    PlayerId GetNextPlayer(PlayerId p);
    unsigned char InterpretCurPlayer();
    unsigned char InterpretPlayer(PlayerId p);

    std::vector<Tile> wallTiles;
    std::vector<Tile> discardTiles;
    std::vector<Tile> reservedTiles;  // Treasure dora etc
    std::vector<Player> players;
    std::vector<Hand> hands;
    PlayerId curPlayer;
    GameState gameState;
};
}

#endif // GAMECONTROLLER_H
