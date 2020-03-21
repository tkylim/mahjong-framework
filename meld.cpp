#include "meld.h"

namespace game {
    Meld::Meld(Tile first, Tile second, Tile third, Tile fourth) : first(first), second(second), third(third), fourth(fourth)
    {
        type = GANG;
    }

    Meld::Meld(MeldType type, Tile first, Tile second, Tile third) : type(type), first(first), second(second), third(third), fourth(Tile(0, Tile::TileType::UNKNOWN))
    {}
}
