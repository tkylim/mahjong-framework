#include "gamewindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QImage>
#include <QImageReader>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QThread>

#include "gamecontroller.h"

#include "resourcemanager.h"

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    playerIdMeldLayoutMap[0] = ui->playerMeldLayout; ui->playerMeldLayout->setDirection(QBoxLayout::Direction::LeftToRight);
    playerIdMeldLayoutMap[1] = ui->leftMeldLayout; ui->leftMeldLayout->setDirection(QBoxLayout::Direction::TopToBottom);
    playerIdMeldLayoutMap[2] = ui->acrossMeldLayout; ui->acrossMeldLayout->setDirection(QBoxLayout::Direction::RightToLeft);
    playerIdMeldLayoutMap[3] = ui->rightMeldLayout; ui->rightMeldLayout->setDirection(QBoxLayout::Direction::BottomToTop);

    playerIdHandLayoutMap[0] = ui->playerHandLayout; ui->playerHandLayout->setDirection(QBoxLayout::Direction::LeftToRight);
    playerIdHandLayoutMap[1] = ui->leftHandLayout; ui->leftHandLayout->setDirection(QBoxLayout::Direction::TopToBottom);
    playerIdHandLayoutMap[2] = ui->acrossHandLayout; ui->acrossHandLayout->setDirection(QBoxLayout::Direction::RightToLeft);
    playerIdHandLayoutMap[3] = ui->rightHandLayout; ui->rightHandLayout->setDirection(QBoxLayout::Direction::BottomToTop);

    // Init hands
    for (int i = 0; i < PLAYER_NUM; i++)
        for (int j = 0; j < DEALT_TILES_PER_PLAYER; j++) {
            guiTiles[i].push_back(new QLabel());
        }
    // TODO:  Whenever an extra tile is added to the hand i.e. gang for total of 14 + 1 tiles
    for (unsigned int i = 0; i < PLAYER_NUM; i++) {
        for (unsigned int j = 0; j < DEALT_TILES_PER_PLAYER; j++) {
            playerIdHandLayoutMap[i]->addWidget(guiTiles[i][j], 0, Qt::AlignLeading);
        }
    }

    // TODO: do this for all of them
    QSizePolicy spTile = guiTiles[0][0]->sizePolicy();
    spTile.setRetainSizeWhenHidden(true);
    ui->drawnGuiTile->setSizePolicy(spTile);

    for (int i = 0; i < PLAYER_NUM; i++)
        for (unsigned int j = 0; j < guiTiles[i].size(); j++)
            guiTiles[i][j]->setSizePolicy(spTile);

    // Reserved tiles layout
    for (int i = 0; i < 4; i++) {
        guiOptionTiles.push_back(new QLabel());
        ui->meldOptionLayout->addWidget(guiOptionTiles.back(), 0, Qt::AlignLeading);
    }
    //ui->reservedTilesLayout->addStretch(1);
    ui->meldAcceptButton->setVisible(false);
    ui->meldDeclineButton->setVisible(false);
    ui->meldNextOptionButton->setVisible(false);

    for (unsigned int i = 0; i < 4; i++) {
        guiOptionTiles[i]->setVisible(false);
        guiOptionTiles[i]->setSizePolicy(spTile);
    }

    memset(discardedTileMap, 0, sizeof(discardedTileMap));
    discardedTileCount = 0;

    indicatorTimer.setSingleShot(true);
    connect(&indicatorTimer, &QTimer::timeout, this, &GameWindow::DismissIndicator);
}

void AddLabelToLayout(QBoxLayout* layout, QLabel* label, bool isHorizontal) {
    if (isHorizontal)
        (static_cast<QHBoxLayout*>(layout))->insertWidget(layout->count()-1, label);
    else (static_cast<QVBoxLayout*>(layout))->insertWidget(layout->count()-1, label);
}

void GameWindow::Render(game::Hand hand, RenderCallbackType type, game::Tile tile, QBoxLayout* meldLayout, std::vector<QLabel*>& tiles, std::vector<QLabel*>& meldTiles, int rotationAngle) {
        QMatrix rm;
        rm.rotate(rotationAngle);
        switch (type) {
        // TODO: Why do melds rearrange themselves besides the hand
        case MELD_ADDED: {
            // TODO:  Fix melds displaying in odd order
            meldTiles.push_back(new QLabel()); meldTiles.back()->setPixmap(ResourceManager::GetInstance().GetTilePixmap(hand.GetMelds().back().GetFirst().GetType()).scaledToWidth(GUI_MELD_TILE_WIDTH, Qt::SmoothTransformation).transformed(rm)); AddLabelToLayout(meldLayout, meldTiles.back(), rotationAngle == 0);
            meldTiles.push_back(new QLabel()); meldTiles.back()->setPixmap(ResourceManager::GetInstance().GetTilePixmap(hand.GetMelds().back().GetSecond().GetType()).scaledToWidth(GUI_MELD_TILE_WIDTH, Qt::SmoothTransformation).transformed(rm)); AddLabelToLayout(meldLayout, meldTiles.back(), rotationAngle == 0);
            meldTiles.push_back(new QLabel()); meldTiles.back()->setPixmap(ResourceManager::GetInstance().GetTilePixmap(hand.GetMelds().back().GetThird().GetType()).scaledToWidth(GUI_MELD_TILE_WIDTH, Qt::SmoothTransformation).transformed(rm)); AddLabelToLayout(meldLayout, meldTiles.back(), rotationAngle == 0);

            bool gang = hand.GetMelds().back().GetType() == game::Meld::MeldType::GANG;
            if (gang) {
                meldTiles.push_back(new QLabel());
                meldTiles.back()->setPixmap(ResourceManager::GetInstance().GetTilePixmap(hand.GetMelds().back().GetFourth().GetType()).scaledToWidth(GUI_MELD_TILE_WIDTH, Qt::SmoothTransformation).transformed(rm));
                AddLabelToLayout(meldLayout, meldTiles.back(), rotationAngle == 0);
            }
            if (hand.GetMelds().size() == 1)
                meldLayout->addStretch(1);

            RenderIndicator(static_cast<IndicatorType>(hand.GetMelds().back().GetType()), true);

            // TODO:  Uncomment
            //ui->drawnGuiTile->setVisible(false);
            break;
        }

        case TILE_ADDED: {
            ui->drawnGuiTile->setPixmap(ResourceManager::GetInstance().GetTilePixmap(tile.GetType()).scaledToWidth(GUI_TILE_WIDTH, Qt::SmoothTransformation));
            ui->drawnGuiTile->setVisible(true);
            break;
        }

        case TILE_DISCARDED: {
            // TODO:  Uncomment
            //ui->drawnGuiTile->setVisible(false);

            // Need room for 96 tiles (37 * 4 - 13 * 4)
            // Tiles are 600 x 800 so 3 x 4 ratio
            // lets do 12 x 9 grid
            // Update pixmap

            if (discardedTileCount == 0) {
                discardPixmap = QPixmap(GUI_DISCARD_AREA_DIMENSION, GUI_DISCARD_AREA_DIMENSION);
                discardPixmap.fill(Qt::transparent); // force alpha channel
            }
            lastDiscardPixmap = discardPixmap;

            QPainter painter(&discardPixmap);
            // TODO:  Randomly arrange tiles
            painter.drawPixmap((discardedTileCount % 12) * GUI_DISCARD_TILE_WIDTH, (discardedTileCount / 12) * GUI_DISCARD_TILE_HEIGHT, ResourceManager::GetInstance().GetTilePixmap(tile.GetType()).scaledToWidth(GUI_DISCARD_TILE_WIDTH, Qt::SmoothTransformation));
            discardedTileMap[discardedTileCount] = tile.GetId();
            discardedTileCount++;
            ui->discardArea->setPixmap(discardPixmap);

            break;
        }

        case DRAW_RESERVED_TILES: {
            // TODO:  Actually call this
            if (guiReservedTiles.empty()) {
                for (unsigned int i = 0; i < hand.GetTiles().size(); i++) {
                    guiReservedTiles.push_back(new QLabel());
                    guiReservedTiles.back()->setPixmap(ResourceManager::GetInstance().GetTilePixmap(hand.GetTiles()[i].GetType()).scaledToWidth(GUI_RESERVED_TILE_WIDTH, Qt::SmoothTransformation));
                    ui->reservedTilesLayout->insertWidget(layout()->count()-1, guiReservedTiles.back());
                }
                ui->reservedTilesLayout->addStretch(1);
            }
            else {
                for (unsigned int i = 0; i < hand.GetTiles().size(); i++) {
                    guiReservedTiles[i]->setPixmap(ResourceManager::GetInstance().GetTilePixmap(hand.GetTiles()[i].GetType()).scaledToWidth(GUI_RESERVED_TILE_WIDTH, Qt::SmoothTransformation));
                    // TODO:  possibly update UI?
                }
            }
            break;
        }

        }

        if (type != DRAW_RESERVED_TILES) {
            // Regardless render the hand
            for (unsigned int i = 0; i < tiles.size(); i++) {
                if (i >= hand.GetTiles().size())
                    tiles[i]->setVisible(false);
                else {
                    tiles[i]->setVisible(true);
                    tiles[i]->setPixmap(ResourceManager::GetInstance().GetTilePixmap(hand.GetTiles()[i].GetType()).scaledToWidth(GUI_TILE_WIDTH, Qt::SmoothTransformation).transformed(rm));
                }
            }
        }
}

void GameWindow::ProcessRender(PlayerId p, game::Hand hand, RenderCallbackType type, game::Tile tile) {
    // TODO:  Move to NetworkManager class and have it call this replacing the non-player hands with unknown before sending over socket
    int index = p - player;
    if (index < 0) index += PLAYER_NUM;
    if (index == 0 || index == 2) Render(hand, type, tile, playerIdMeldLayoutMap[index], guiTiles[p], guiMeldTiles[p], 0);
    else if (index == 1) Render(hand, type, tile, playerIdMeldLayoutMap[index], guiTiles[p], guiMeldTiles[p], 270);
    else if (index == 3) Render(hand, type, tile, playerIdMeldLayoutMap[index], guiTiles[p], guiMeldTiles[p], 90);
}

void GameWindow::RenderIndicator(IndicatorType type, bool removeLastTile) {
    // TODO:  Remove the last randomly placed tile
    if (removeLastTile) {
        discardedTileCount--;
        discardedTileMap[discardedTileCount] = 0;
        discardPixmap = lastDiscardPixmap;
    }
    else {
        lastDiscardPixmap = discardPixmap;
    }
    QPainter painter(&discardPixmap);
    painter.drawPixmap(0, 0, ResourceManager::GetInstance().GetIndicatorPixmap(type).scaledToWidth(GUI_DISCARD_AREA_DIMENSION, Qt::TransformationMode::SmoothTransformation));
    ui->discardArea->setPixmap(discardPixmap);
    indicatorTimer.start(INDICATOR_DISPLAY_TIME_MS);
}

void GameWindow::DismissIndicator() {
    discardPixmap = lastDiscardPixmap;
    ui->discardArea->setPixmap(discardPixmap);
}

void GameWindow::ProcessMeldOffer(PlayerId p, Melds meldOptions) {
    ui->meldAcceptButton->setVisible(true);
    ui->meldDeclineButton->setVisible(true);
    if (meldOptions.size() > 1)
        ui->meldNextOptionButton->setVisible(true);

    bool gang = meldOptions[0].GetType() == game::Meld::MeldType::GANG;
    for (unsigned int i = 0; i < (gang ? 4 : 3); i++)
        guiOptionTiles[i]->setVisible(true);

    selectedMeldOption = 0;
    this->meldOptions = meldOptions;

    UpdateMeldOptions();
}

void GameWindow::ProcessMeldAccepted() {
    emit DispatchMeldAccepted(meldOptions[selectedMeldOption]);

    for (unsigned int i = 0; i < 4; i++)
        guiOptionTiles[i]->setVisible(false);

    ui->meldAcceptButton->setVisible(false);
    ui->meldDeclineButton->setVisible(false);
    ui->meldNextOptionButton->setVisible(false);
}

void GameWindow::ProcessMeldDeclined() {
    emit DispatchMeldDeclined();

    for (unsigned int i = 0; i < 4; i++)
        guiOptionTiles[i]->setVisible(false);

    ui->meldAcceptButton->setVisible(false);
    ui->meldDeclineButton->setVisible(false);
    ui->meldNextOptionButton->setVisible(false);
}

void GameWindow::ProcessMeldNext() {
    selectedMeldOption++;
    if (selectedMeldOption == meldOptions.size()) {
        selectedMeldOption = 0;
    }
    UpdateMeldOptions();
}

void GameWindow::UpdateMeldOptions() {
    bool gang = meldOptions[selectedMeldOption].GetType() == game::Meld::MeldType::GANG;
    guiOptionTiles[0]->setPixmap(ResourceManager::GetInstance().GetTilePixmap(meldOptions[selectedMeldOption].GetFirst().GetType()).scaledToWidth(GUI_MELD_TILE_WIDTH, Qt::SmoothTransformation));
    guiOptionTiles[1]->setPixmap(ResourceManager::GetInstance().GetTilePixmap(meldOptions[selectedMeldOption].GetSecond().GetType()).scaledToWidth(GUI_MELD_TILE_WIDTH, Qt::SmoothTransformation));
    guiOptionTiles[2]->setPixmap(ResourceManager::GetInstance().GetTilePixmap(meldOptions[selectedMeldOption].GetThird().GetType()).scaledToWidth(GUI_MELD_TILE_WIDTH, Qt::SmoothTransformation));
    if (gang) guiOptionTiles[3]->setPixmap(ResourceManager::GetInstance().GetTilePixmap(meldOptions[selectedMeldOption].GetFourth().GetType()).scaledToWidth(GUI_MELD_TILE_WIDTH, Qt::SmoothTransformation));
}

GameWindow::~GameWindow()
{
    for(int i = 0; i < PLAYER_NUM; i++) {
        for (QLabel* tile : guiTiles[i]) {
            delete tile;
        }
        for (QLabel* meldTile : guiMeldTiles[i]) {
            delete meldTile;
        }
        for (QLabel* reservedTile : guiReservedTiles) {
            delete reservedTile;
        }
        for (QLabel* optionTile : guiOptionTiles) {
            delete optionTile;
        }
    }

    delete ui;
}

void GameWindow::Init(PlayerId player)
{
    this->player = player;
}

QPushButton *GameWindow::GetStartButton()
{
    return ui->pushButton;
}

QPushButton *GameWindow::GetMeldAcceptedButton()
{
    return ui->meldAcceptButton;
}

QPushButton *GameWindow::GetMeldDeclinedButton()
{
    return ui->meldDeclineButton;
}

QPushButton *GameWindow::GetMeldNextOptionButton()
{
    return ui->meldNextOptionButton;
}
