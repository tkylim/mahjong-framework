#ifndef TILE_H
#define TILE_H

#include <QMetaType>

namespace game {

class Tile
{
public:
    enum TileType {
        UNKNOWN = 0, // Used to display a blank tile for opponent hands
        NUM_1 = 1,
        NUM_2 = 2,
        NUM_3 = 3,
        NUM_4 = 4,
        NUM_5 = 5,
        NUM_6 = 6,
        NUM_7 = 7,
        NUM_8 = 8,
        NUM_9 = 9,
        NUM_5_DORA = 10,

        PANCAKE_1 = 11,
        PANCAKE_2 = 12,
        PANCAKE_3 = 13,
        PANCAKE_4 = 14,
        PANCAKE_5 = 15,
        PANCAKE_6 = 16,
        PANCAKE_7 = 17,
        PANCAKE_8 = 18,
        PANCAKE_9 = 19,
        PANCAKE_5_DORA = 20,

        STICK_1 = 21,
        STICK_2 = 22,
        STICK_3 = 23,
        STICK_4 = 24,
        STICK_5 = 25,
        STICK_6 = 26,
        STICK_7 = 27,
        STICK_8 = 28,
        STICK_9 = 29,
        STICK_5_DORA = 30,

        DRAGON_RED = 31,
        DRAGON_GREEN = 32,
        DRAGON_WHITE = 33,

        WIND_EAST = 34,
        WIND_SOUTH = 35,
        WIND_WEST = 36,
        WIND_NORTH = 37,
    };

public:
    Tile();
    Tile(unsigned int id, Tile::TileType type);

    static Tile GetBlankTile();

    TileType GetType() const;
    unsigned int GetId() const ;
    unsigned char GetNumericalValue() const;
    bool CompareType(const Tile& other) const;

    bool operator==(const Tile& other) const;
    bool operator<(const Tile& other) const;

private:
    unsigned int id; // 0 is a reserved ID denoting usage for comparism or searching only
    TileType type;
};

}
Q_DECLARE_METATYPE(game::Tile);

#endif // TILE_H
