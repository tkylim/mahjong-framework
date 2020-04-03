#include <QFile>
#include <QDir>
#include <QTextStream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <QPainter>
#include <QDebug>

#include "resourcemanager.h"

void ResourceManager::Init(std::string pkg) {
    this->pkg = pkg;

    // Read config
    std::ifstream f;
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

    LoadAllTiles();

    LoadTilePixmaps();
    LoadIndicatorPixmaps();
}

void ResourceManager::LoadAllTiles() {
    // TODO Error check here
    const rapidjson::Value& a = configJson["tiles"];
        for (rapidjson::SizeType i = 0; i < a.Size(); i++) {
            allTileIds.push_back(static_cast<game::Tile::TileType>(a[i].GetInt()));
            for (unsigned int j = 0; j < NUM_EACH_TILE; j++)
                allTiles.push_back(game::Tile(static_cast<unsigned int>(i * NUM_EACH_TILE + j + 1), static_cast<game::Tile::TileType>(a[i].GetInt())));
        }
}

void ResourceManager::LoadTilePixmaps() {
    QImage imgBase, img;
    if (imgBase.load("../mahjong-framework/res/images/tiles/light/"+tileTypeToFnameMap[0]+".png")) {
        // Load the base tile
        //imgBase = imgBase.scaledToWidth(GUI_TILE_WIDTH, Qt::SmoothTransformation);
        QPixmap base = QPixmap::fromImage(imgBase);
        tilePixmaps.push_back(base);

        for (game::Tile::TileType &type : allTileIds) {
            if (img.load("../mahjong-framework/res/images/tiles/light/"+tileTypeToFnameMap[type]+".png")) {
                //img = img.scaledToWidth(GUI_TILE_WIDTH, Qt::SmoothTransformation);
                QPixmap result(base.width(), base.height());
                result.fill(Qt::transparent); // force alpha channel
                QPainter painter(&result);
                painter.drawPixmap(0, 0, base);
                painter.drawPixmap(0, 0, QPixmap::fromImage(img));
                tilePixmaps.push_back(result);
                qDebug() << "Loaded image" << tileTypeToFnameMap[type];
            }
            else qDebug() << "Error loading image" << tileTypeToFnameMap[type];
        }
    }
    else qDebug() << "Error loading image" << tileTypeToFnameMap[0];
}

void ResourceManager::LoadIndicatorPixmaps() {
    QImage img;
    for (unsigned int i = 0; i < IndicatorType::COUNT; i++) {
        if (img.load("../mahjong-framework/res/images/indicators/"+indicatorTypeToFnameMap[i]+".png")) {
            indicatorPixmaps.push_back(QPixmap::fromImage(img));
            qDebug() << "Loaded image" << indicatorTypeToFnameMap[i];
        }
        else qDebug() << "Error loading image" << indicatorTypeToFnameMap[i];
    }
}

const QPixmap& ResourceManager::GetTilePixmap(game::Tile::TileType t) const
{
    auto it = std::find(allTileIds.begin(), allTileIds.end(), t);
    if (it == allTileIds.end())
        return tilePixmaps[0];
    unsigned int x = std::distance(allTileIds.begin(), it);
    return tilePixmaps[x + 1];
}

const QPixmap& ResourceManager::GetIndicatorPixmap(IndicatorType t) const
{
    return indicatorPixmaps[t];
}

bool ResourceManager::IsRotatingCw() const
{
    return isRotatingCw;
}

bool ResourceManager::CanChiOffAnyoneIfOneTileOffOneTileOffState() const
{
    return canChiOffAnyoneIfOneTileOffOneTileOffState;
}

unsigned char ResourceManager::GetNumOfReservedTiles() const
{
    return numOfReservedTiles;
}

// Returns true on an error
bool ResourceManager::GetConfigBool(const char *key, bool& value) const {
    if (configJson.HasMember(key)) {
        value = configJson[key].GetBool();
        return false;
    }
    return true;
}

bool ResourceManager::GetConfigInt(const char *key, int& value) const {
    if (configJson.HasMember(key)) {
        value = configJson[key].GetInt();
        return false;
    }
    return true;
}

const std::vector<game::Tile> *ResourceManager::GetAllTiles() const {
    return &allTiles;
}
