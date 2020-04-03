#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>

#include <condition_variable>
#include <functional>
#include <mutex>
#include <vector>

#include "globals.h"
#include "hand.h"
#include "player.h"
#include "tile.h"

namespace game {

class GameController : public QObject
{
    Q_OBJECT

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


public slots:
    void Loop();
    void ReceiveMeldAccepted(Meld meld);
    void ReceiveMeldDeclined();

signals:
    void DispatchRenderRequest(PlayerId p, game::Hand hand, RenderCallbackType type, game::Tile tile);
    void DispatchMeldOffer(PlayerId p, Melds meldOptions);
    void UnblockGameController();

private:
    PlayerId GetNextPlayer();
    PlayerId GetNextPlayer(PlayerId p);
    unsigned char InterpretCurPlayer();
    unsigned char InterpretPlayer(PlayerId p);
    void Wait(unsigned int durationMs, bool shouldTimeout);
    void AwaitUnblockSignal();

    Meld selectedMeld;
    bool meldSelectedFlag = false;

    std::vector<Tile> wallTiles;
    std::vector<Tile> discardTiles;
    std::vector<Tile> reservedTiles;  // Treasure dora etc
    std::vector<Player> players;
    std::vector<Hand> hands;
    PlayerId curPlayer;
    GameState gameState;
};

#endif // GAMECONTROLLER_H

}
