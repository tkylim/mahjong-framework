#ifndef PACKAGEMANAGER_H
#define PACKAGEMANAGER_H

#include <string>

#include "include/rapidjson/document.h"

#include "hand.h"

namespace game {

class PackageManager
{
public:
    static PackageManager& GetInstance() {
        static PackageManager instance;
        return instance;
    }

    void Init(std::string pkg);
    unsigned char UpdateHandState(const Hand& hand, Tile t, bool IsDrawPeriod = false);

    static unsigned int CalculateScore();

    bool IsRotatingCW() const;
    bool CanChiOffAnyoneIfOneTileOffOneTileOffState() const;
    unsigned char GetNumOfReservedTiles() const;
    std::vector<Tile> *GetAllTiles() const;

private:
    PackageManager() {}

    std::string pkg;
    bool isRotatingCW = true;
    bool canChiOffAnyoneIfOneTileOffOneTileOffState = false;
    unsigned char numOfReservedTiles = 0;
    rapidjson::Document configJson;
};

}

#endif // PACKAGEMANAGER_H
