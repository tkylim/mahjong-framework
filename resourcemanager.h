#ifndef PACKAGEMANAGER_H
#define PACKAGEMANAGER_H

#include <QPixmap>
#include <string>

#include "include/rapidjson/document.h"

#include "globals.h"
#include "hand.h"

class ResourceManager
{
public:
    static ResourceManager& GetInstance() {
        static ResourceManager instance;
        return instance;
    }

    void Init(std::string pkg);
    unsigned char UpdateHandState(const game::Hand& hand, game::Tile t, bool IsDrawPeriod = false);

    static unsigned int CalculateScore();

    bool IsRotatingCw() const;
    bool CanChiOffAnyoneIfOneTileOffOneTileOffState() const;
    unsigned char GetNumOfReservedTiles() const;
    const std::vector<game::Tile> *GetAllTiles() const;
    const QPixmap& GetTilePixmap(game::Tile::TileType t) const;
    const QPixmap& GetIndicatorPixmap(IndicatorType t) const;
    void LoadAllTiles();
    void LoadTilePixmaps();
    void LoadIndicatorPixmaps();

private:
    ResourceManager() {}
    bool GetConfigBool(const char *key, bool& value) const;
    bool GetConfigInt(const char *key, int& value) const;

    std::string pkg;
    bool isRotatingCw = true;
    bool canChiOffAnyoneIfOneTileOffOneTileOffState = false;
    unsigned char numOfReservedTiles = 0;
    rapidjson::Document configJson;
    std::vector<game::Tile::TileType> allTileIds;
    std::vector<game::Tile> allTiles;
    std::vector<QPixmap> tilePixmaps;
    std::vector<QPixmap> indicatorPixmaps;

    // TODO:  replace filenames for hu le, kouting with generic
    const QString indicatorTypeToFnameMap[5] = {"chi", "cha", "gang", "kouting", "hule"};
    const QString tileTypeToFnameMap[38] = {"front", "num1", "num2", "num3", "num4", "num5", "num6", "num7", "num8", "num9", "num5-dora",
                                          "pancake1", "pancake2", "pancake3", "pancake4", "pancake5", "pancake6", "pancake7", "pancake8", "pancake9", "pancake5-dora",
                                          "stick1", "stick2", "stick3", "stick4", "stick5", "stick6", "stick7", "stick8", "stick9", "stick5-dora",
                                          "dragon-red", "dragon-green", "dragon-white",
                                          "wind-east", "wind-south", "wind-west", "wind-north"};
};

#endif
