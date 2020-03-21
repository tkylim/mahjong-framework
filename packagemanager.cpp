#include <QFile>
#include <QDir>
#include <QTextStream>
#include <fstream>
#include <sstream>
#include <iostream>

#include "packagemanager.h"

namespace game {

void PackageManager::Init(std::string pkg) {
    this->pkg = pkg;
    // TODO load
    std::ifstream f;
    std::stringstream ss;
    //QFile fconfig("pkg/"+pkg+"/config.json");
    // TODO Fix
    QFile fconfig("C:/Users/thoma/OneDrive/Personal/Projects/mahjong-framework/mahjong-framework/pkg/harbin/config.json");
                  //"../../mahjong-framework/pkg/harbin/config.json");
    if (!fconfig.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QDir x;

    QTextStream in(&fconfig);
    QString configStr = in.readAll();
    std::cout << configStr.toStdString() << std::endl;
    std::cout << QDir::currentPath().toStdString() << std::endl;
    configJson.Parse(configStr.toStdString().c_str());
}

bool PackageManager::IsRotatingCW() const
{
    return isRotatingCW;
}

bool PackageManager::CanChiOffAnyoneIfOneTileOffOneTileOffState() const
{
    return canChiOffAnyoneIfOneTileOffOneTileOffState;
}

unsigned char PackageManager::GetNumOfReservedTiles() const
{
    return numOfReservedTiles;
}

std::vector<Tile> *PackageManager::GetAllTiles() const
{
    std::vector<Tile> *allTiles = new std::vector<Tile>();
    // TODO Error check here
    const rapidjson::Value& a = configJson["tiles"];
    for (rapidjson::SizeType i = 0; i < a.Size(); i++)
        allTiles->push_back(Tile(static_cast<unsigned int>(i + 1), static_cast<Tile::TileType>(a[i].GetInt())));

    return allTiles;
}

}
