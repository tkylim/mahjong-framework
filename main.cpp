#include "gamecontroller.h"
#include "gamewindow.h"
#include "networkmanager.h"
#include "resourcemanager.h"

#include <QApplication>
#include <QThread>
#include <QMetaType>

using game::Meld;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QThread gameThread;
    QThread networkThread;

    qRegisterMetaType<PlayerId>();
    qRegisterMetaType<RenderCallbackType>();
    qRegisterMetaType<game::Hand>();
    qRegisterMetaType<game::Tile>();
    qRegisterMetaType<Melds>();
    qRegisterMetaType<game::Meld>();

    GameWindow gameWindow;
    gameWindow.Init(PlayerId::WIND_EAST);
    gameWindow.show();

    ResourceManager::GetInstance().Init("harbin");

    // TODO:  Do we need a game controller on clients?
    // TODO:  Clean up main
    game::GameController gameController;
    NetworkManager networkManager;
    gameController.moveToThread(&gameThread);
    // TODO:  awaiting mutex bullshit is blocking event loop from being executed, maybe use a qt specific condition_variable
    // This causes an error about registering the metatype datatype when not on game thread?
    networkManager.moveToThread(&networkThread);

    // GameController to NetworkManager
    QObject::connect(&gameController, &game::GameController::DispatchRenderRequest, &networkManager, &NetworkManager::ReceiveRenderRequest);
    QObject::connect(&gameController, &game::GameController::DispatchMeldOffer, &networkManager, &NetworkManager::ReceiveMeldOffer);

    // NetworkManager to GameController
    QObject::connect(&networkManager, &NetworkManager::PostMeldAccepted, &gameController, &game::GameController::ReceiveMeldAccepted);
    QObject::connect(&networkManager, &NetworkManager::PostMeldDeclined, &gameController, &game::GameController::ReceiveMeldDeclined);

    // NetworkManager to Window
    QObject::connect(&networkManager, &NetworkManager::PostRenderRequest, &gameWindow, &GameWindow::ProcessRender);
    QObject::connect(&networkManager, &NetworkManager::PostMeldOffer, &gameWindow, &GameWindow::ProcessMeldOffer);

    // Window to NetworkManager
    QObject::connect(&gameWindow, &GameWindow::DispatchMeldAccepted, &networkManager, &NetworkManager::ReceiveMeldAccepted);
    QObject::connect(&gameWindow, &GameWindow::DispatchMeldDeclined, &networkManager, &NetworkManager::ReceiveMeldDeclined);

    // Buttons to window
    QObject::connect(gameWindow.GetMeldAcceptedButton(), &QPushButton::released, &gameWindow, &GameWindow::ProcessMeldAccepted);
    QObject::connect(gameWindow.GetMeldDeclinedButton(), &QPushButton::released, &gameWindow, &GameWindow::ProcessMeldDeclined);
    QObject::connect(gameWindow.GetMeldNextOptionButton(), &QPushButton::released, &gameWindow, &GameWindow::ProcessMeldNext);

    // TODO:  Remove this
    QObject::connect(gameWindow.GetStartButton(), &QPushButton::released, &gameController, &game::GameController::Loop);

    //emit gameWindow.DispatchMeldAccepted(game::Meld::Empty());

    gameThread.start();
    networkThread.start();

    int ret = a.exec();
    gameThread.quit();
    gameThread.wait();
    networkThread.quit();
    networkThread.wait();

    return ret;
}
