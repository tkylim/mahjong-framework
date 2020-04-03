#include "gamecontroller.h"
#include "mahjonghelper.h"
#include "resourcemanager.h"

#include <algorithm>
#include <QDebug>

#include <QEventLoop>
#include <QTimer>
#include <windows.h>

namespace game{

GameController::GameController()
{
    for (unsigned char i = 0; i < PLAYER_NUM; i++)
        hands.push_back(Hand());
    // This has to be done on a seperate line otherwise the references to the hands go bad due to vector resizing
    for (unsigned char i = 0; i < PLAYER_NUM; i++)
        players.push_back(Player(static_cast<PlayerId>(i), &hands[i]));
}

GameController::~GameController()
{

}

Tile GameController::Draw()
{
    if (wallTiles.empty())
        return Tile(0, Tile::TileType::UNKNOWN);

    Tile t = wallTiles.back();
    wallTiles.pop_back();
    qDebug() << "Player" << InterpretCurPlayer() << "drew tile" << t.GetType();
    return t;
}

void GameController::Loop()
{
    gameState = SETUP;

    // Create wall
    const std::vector<Tile> *allTiles = ResourceManager::GetInstance().GetAllTiles();
    std::copy(allTiles->begin(), allTiles->end(), std::back_inserter(wallTiles));
    delete allTiles;

    // TODO fix randomness
    std::random_shuffle(wallTiles.begin(), wallTiles.end());

    // Handle treasure slash dora
    for (unsigned char i = 0; i < ResourceManager::GetInstance().GetNumOfReservedTiles(); i++) {
        reservedTiles.push_back(wallTiles.back());
        wallTiles.pop_back();
    }

    // Deal tiles
    for (unsigned char i = 0; i < DEALT_TILES_PER_PLAYER; i++) {
        for (unsigned char j = 0; j < PLAYER_NUM; j++) {
            hands[j].Draw(wallTiles.back());
            wallTiles.pop_back();
        }
    }

    curPlayer = PlayerId::WIND_EAST;

    gameState = MAIN;
    while(true) {
        if(Turn()) break;
        Sleep(uint(100));
    }
    qDebug() << "Game finished state: " << gameState;
    // someone won or we ran out of tiles update the next starter
}


void GameController::Wait(unsigned int durationMs, bool shouldTimeout) {
    QTimer timer;
    QEventLoop loop;
    timer.setSingleShot(true);
    connect(this, &GameController::UnblockGameController, &loop, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(durationMs);
    loop.exec();

    // TODO:  deal with timeout
    if(!shouldTimeout && !timer.isActive())
        qDebug("Timeout occurred");
}

void GameController::AwaitUnblockSignal() {
    Wait(TIMEOUT_MS, false);
}

void GameController::ReceiveMeldAccepted(Meld meld) {
    selectedMeld = meld;
    emit UnblockGameController();
}

void GameController::ReceiveMeldDeclined() {
    selectedMeld = Meld::Empty();
    emit UnblockGameController();
}

// Returns true when the game should stop
bool GameController::Turn()
{
    // Used to skip draw phase on melds
    static bool meldOccurredLastTurn = false;
    if (!meldOccurredLastTurn) {
        Tile drawnTile = Draw();
        if (drawnTile.GetType() == Tile::TileType::UNKNOWN) {  // No more tiles
            gameState = EXHAUSTED;
            return true;
        }

        // Render before drawing the tile so the drawn tile can be rendered seperately
        // TODO:  NetworkManager only send the tile to the player who draws it
        emit DispatchRenderRequest(curPlayer, hands[InterpretCurPlayer()], RenderCallbackType::TILE_ADDED, drawnTile);

        hands[InterpretCurPlayer()].Draw(drawnTile);

        // Update current players hand
        Hand::HandState s = MahjongHelper::UpdateHandState(hands[InterpretCurPlayer()], drawnTile, true);
        if (s == Hand::HandState::ONE_TILE_OFF) {
            // Present player with one tile off option (kou ting, riichi etc)
            // If they accept
            hands[InterpretCurPlayer()].SetState(Hand::HandState::ONE_TILE_OFF);
        }
        else if (s == Hand::HandState::WINNING) {
            gameState = static_cast<GameState>(WIN_EAST + InterpretPlayer(curPlayer));
            return true;
        }
    }
    else {
        meldOccurredLastTurn = false;
    }

    // Blocking call for request player discard choice
    Tile discardedTile = players[InterpretCurPlayer()].GetDiscardBlocking();

    // TODO:  Actually handle discard
    Discard(discardedTile);

    // Call for render
    emit DispatchRenderRequest(curPlayer, hands[InterpretCurPlayer()], RenderCallbackType::TILE_DISCARDED, discardedTile);

    // Update hand states
    PlayerId p = curPlayer;
    for (unsigned int i = 0; i < PLAYER_NUM; i++) {
        Hand::HandState s = MahjongHelper::UpdateHandState(hands[i], discardedTile);
        if (s == Hand::HandState::ONE_TILE_OFF) {
            // Present player with one tile off option (kou ting, riichi etc)
            // If they accept
            hands[i].SetState(Hand::HandState::ONE_TILE_OFF);
        }
        else if (s == Hand::HandState::WINNING) {
            gameState = static_cast<GameState>(WIN_EAST + InterpretPlayer(p));
            return true;
        }
        p = GetNextPlayer(p);
    }

    // Check for gang
    for (unsigned int i = 0; i < PLAYER_NUM; i++) {
        if (MahjongHelper::IsTileGangable(hands[i], discardedTile)) {
            qDebug() << "Offered player" << i << "a gang";
            hands[i].Draw(discardedTile);
            auto itFirst = std::find(hands[i].GetTiles().begin(), hands[i].GetTiles().end(), Tile(0, static_cast<Tile::TileType>(discardedTile.GetType())));
            emit DispatchMeldOffer(static_cast<PlayerId>(i), QVector<Meld>(1, Meld(*itFirst, *(itFirst+1), *(itFirst+2), *(itFirst+3))));
            AwaitUnblockSignal();
            if (selectedMeld.GetType() != Meld::MeldType::INVALID) {
                hands[i].AddMeld(Meld(*itFirst, *(itFirst+1), *(itFirst+2), *(itFirst+3)));
                curPlayer = static_cast<PlayerId>(i);
                meldOccurredLastTurn = true;
                emit DispatchRenderRequest(curPlayer, hands[InterpretCurPlayer()], RenderCallbackType::MELD_ADDED, Tile::GetBlankTile());
                Wait(INDICATOR_DISPLAY_TIME_MS, true);
            }
            break; // There can only be one gang
        }
    }

    // Check for cha
    for (unsigned int i = 0; i < PLAYER_NUM; i++) {
        if (MahjongHelper::IsTileChaable(hands[i], discardedTile)) {
            qDebug() << "Offered player" << i << "a cha";
            hands[i].Draw(discardedTile);
            auto itFirst = std::find(hands[i].GetTiles().begin(), hands[i].GetTiles().end(), Tile(0, static_cast<Tile::TileType>(discardedTile.GetType())));
            emit DispatchMeldOffer(static_cast<PlayerId>(i), QVector<Meld>(1, Meld(Meld::MeldType::CHA, *itFirst, *(itFirst+1), *(itFirst+2))));
            AwaitUnblockSignal();
            if (selectedMeld.GetType() != Meld::MeldType::INVALID) {
                hands[i].AddMeld(Meld(Meld::MeldType::CHA, *itFirst, *(itFirst+1), *(itFirst+2)));
                curPlayer = static_cast<PlayerId>(i);
                meldOccurredLastTurn = true;
                emit DispatchRenderRequest(curPlayer, hands[InterpretCurPlayer()], RenderCallbackType::MELD_ADDED, Tile::GetBlankTile());
                Wait(INDICATOR_DISPLAY_TIME_MS, true);
                return false;
            }
            break; // There can only be one cha
        }
    }

    // Check for chi for ONLY the next player GetNextPlayer()
    // TODO:  Deal with Harbin's awful rule about chi from anywhere to be in kou ting if your hand is closed
    std::vector<Meld> chis = MahjongHelper::IsTileChiable(hands[InterpretPlayer(GetNextPlayer())], discardedTile);
    if (!chis.empty()) {
        qDebug() << "Offered player" << GetNextPlayer() << "a chi";
        emit DispatchMeldOffer(GetNextPlayer(), QVector<Meld>::fromStdVector(chis));
        AwaitUnblockSignal();
        if (selectedMeld.GetType() != Meld::MeldType::INVALID) {
            hands[GetNextPlayer()].Draw(discardedTile);
            auto itFirst = std::find(hands[GetNextPlayer()].GetTiles().begin(), hands[GetNextPlayer()].GetTiles().end(), Tile(0, static_cast<Tile::TileType>(selectedMeld.GetFirst().GetType())));
            auto itSecond = std::find(hands[GetNextPlayer()].GetTiles().begin(), hands[GetNextPlayer()].GetTiles().end(), Tile(0, static_cast<Tile::TileType>(selectedMeld.GetSecond().GetType())));
            auto itThird = std::find(hands[GetNextPlayer()].GetTiles().begin(), hands[GetNextPlayer()].GetTiles().end(), Tile(0, static_cast<Tile::TileType>(selectedMeld.GetThird().GetType())));
            hands[GetNextPlayer()].AddMeld(Meld(Meld::MeldType::CHI, *itFirst, *itSecond, *itThird));
            curPlayer = static_cast<PlayerId>(GetNextPlayer());
            meldOccurredLastTurn = true;
            emit DispatchRenderRequest(curPlayer, hands[InterpretCurPlayer()], RenderCallbackType::MELD_ADDED, Tile::GetBlankTile());
            Wait(INDICATOR_DISPLAY_TIME_MS, true);
            return false;
        }
    }

    // Send rendering/UI update to allow players to select options
    // TODO: Deal with cha and chi at the same time

    // TODO: Should flip a reserved tile and if so for whom? Let's do this through calling the updatehand function
    // TODO: Replace treasure if

    curPlayer = GetNextPlayer();
    return false;
}

void GameController::Discard(Tile t)
{
    qDebug() << "Player" << InterpretCurPlayer() << "discarded tile" << t.GetType();
    hands[InterpretCurPlayer()].Discard(t);
}

PlayerId GameController::GetNextPlayer() {
    return GetNextPlayer(curPlayer);
}

PlayerId GameController::GetNextPlayer(PlayerId p) {
    unsigned char next = p;
    if (ResourceManager::GetInstance().IsRotatingCw()) {
        if (next == PLAYER_NUM - 1)
            next = 0;
        else
            next = static_cast<unsigned char>(p) + 1;
    }
    else {
        if (next == 0)
            next = PLAYER_NUM - 1;
        else
            next = static_cast<unsigned char>(p) - 1;
    }
    return static_cast<PlayerId>(next);
}

unsigned char GameController::InterpretCurPlayer() {
    return InterpretPlayer(curPlayer);
}

unsigned char GameController::InterpretPlayer(PlayerId p) {
    return static_cast<unsigned char>(p);
}

}
