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
    std::ifstream f;
    std::stringstream ss;
    QFile fconfig("../mahjong-framework/pkg/"+QString::fromStdString(pkg)+"/config.json");
    if (!fconfig.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QDir x;

    QTextStream in(&fconfig);
    QString configStr = in.readAll();
    configJson.Parse(configStr.toStdString().c_str());
    int configInt;
    GetConfigBool("isRotatingCw", isRotatingCw);
    GetConfigBool("canChiOffAnyoneIfOneTileOffOneTileOffState", canChiOffAnyoneIfOneTileOffOneTileOffState);
    GetConfigInt("numOfReservedTiles", configInt); numOfReservedTiles = configInt;
}

bool PackageManager::IsRotatingCw() const
{
    return isRotatingCw;
}

bool PackageManager::CanChiOffAnyoneIfOneTileOffOneTileOffState() const
{
    return canChiOffAnyoneIfOneTileOffOneTileOffState;
}

unsigned char PackageManager::GetNumOfReservedTiles() const
{
    return numOfReservedTiles;
}

// Returns true on an error
bool PackageManager::GetConfigBool(const char *key, bool& value) const {
    if (configJson.HasMember(key)) {
        value = configJson[key].GetBool();
        return false;
    }
    return true;
}

bool PackageManager::GetConfigInt(const char *key, int& value) const {
    if (configJson.HasMember(key)) {
        value = configJson[key].GetInt();
        return false;
    }
    return true;
}

std::vector<Tile> *PackageManager::GetAllTiles() const
{
    std::vector<Tile> *allTiles = new std::vector<Tile>();
    // TODO Error check here
    const rapidjson::Value& a = configJson["tiles"];
        for (rapidjson::SizeType i = 0; i < a.Size(); i++)
            for (int j = 0; j < NUM_EACH_TILE; j++)
                allTiles->push_back(Tile(static_cast<unsigned int>(i * NUM_EACH_TILE + j + 1), static_cast<Tile::TileType>(a[i].GetInt())));

    return allTiles;
}

}
