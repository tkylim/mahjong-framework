#include "tile.h"

namespace game {

Tile::Tile(unsigned int id, Tile::TileType type) : id(id), type(type)
{
}

Tile::TileType Tile::GetType() const {
    return type;
}

unsigned int Tile::GetId() const {
    return id;
}

unsigned char Tile::GetNumericalValue() const {
    if (type >= NUM_1 && type <= NUM_9)
        return type - NUM_1 + 1;

    if (type >= PANCAKE_1 && type <= PANCAKE_9)
        return type - PANCAKE_1 + 1;

    if (type >= STICK_1 && type <= STICK_9)
        return type - STICK_1 + 1;
    return 0;
}

bool Tile::operator==(const Tile& other) const {
    // Special case used for searching, etc
    if (this->id == 0|| other.id == 0)
        return this->type == other.type;
    return this->id == other.id;
}
bool Tile::operator<(const Tile& other) const {
    return this->type < other.type;
}

bool Tile::CompareType(const Tile& other) const {
    return this->type == other.type;
}

}
