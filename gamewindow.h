#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>

#include <functional>

#include "globals.h"
#include "hand.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    ~GameWindow();

    void Init(PlayerId player);

    QPushButton *GetStartButton();
    QPushButton *GetMeldAcceptedButton();
    QPushButton *GetMeldDeclinedButton();
    QPushButton *GetMeldNextOptionButton();

    GameWindow(QWidget *parent = nullptr);
    void RenderDiscardCallback(game::Tile& discard);

public slots:
    void ProcessRender(PlayerId p, game::Hand hand, RenderCallbackType type, game::Tile tile);
    void ProcessMeldOffer(PlayerId p, Melds meldOptions);
    void ProcessMeldAccepted();
    void ProcessMeldDeclined();
    void ProcessMeldNext();

private slots:
    void DismissIndicator();

signals:
    void DispatchMeldAccepted(game::Meld meld);
    void DispatchMeldDeclined();

private:
    void Render(game::Hand hand, RenderCallbackType type, game::Tile tile, QBoxLayout* meldLayout, std::vector<QLabel*>& tiles, std::vector<QLabel*>& meldTiles, int rotationAngle);
    void UpdateMeldOptions();
    void RenderIndicator(IndicatorType type, bool removeLastTile);

    QBoxLayout *playerIdMeldLayoutMap[PLAYER_NUM];
    QBoxLayout *playerIdHandLayoutMap[PLAYER_NUM];

    int discardedTileMap[MAX_NUM_DISCARDED_TILES];
    int discardedTileCount;
    QPixmap discardPixmap;
    QPixmap lastDiscardPixmap; // Stores the last pixmap so rendering chi, cha, gang, wins can be done nicely
    QTimer indicatorTimer;

    PlayerId player;

    // Meld options
    unsigned int selectedMeldOption = 0;
    Melds meldOptions;

    // TODO:  Check to see if these backing vectors are necessary or if it's sufficient to store the labels in the layouts themselves. (this is probably the case)
    std::vector<QLabel*> guiTiles[PLAYER_NUM];
    std::vector<QLabel*> guiMeldTiles[PLAYER_NUM];
    std::vector<QLabel*> guiReservedTiles;
    std::vector<QLabel*> guiOptionTiles;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
