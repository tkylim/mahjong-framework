#include "gamecontroller.h"
#include "mainwindow.h"
#include "packagemanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    game::PackageManager::GetInstance().Init("harbin");
    game::GameController gameController;
    gameController.Loop();

    return a.exec();
}
