#ifndef GLOBALS_H
#define GLOBALS_H

#include <QMetaType>

enum PlayerId {
    WIND_EAST = 0,
    WIND_SOUTH = 1,
    WIND_WEST = 2,
    WIND_NORTH = 3,
    INVALID
};

PlayerId AdvancePlayerId(PlayerId p);

enum RenderCallbackType {
    MELD_ADDED,
    TILE_ADDED,
    TILE_DISCARDED,
    DRAW_RESERVED_TILES
};

enum IndicatorType {
    CHI,
    CHA,
    GANG,
    ONE_TILE_OFF,
    WIN,
    COUNT // Not an actual enum type
};

#define PLAYER_NUM 4
#define DEALT_TILES_PER_PLAYER 13
#define NUM_EACH_TILE 4
#define MAX_NUM_DISCARDED_TILES 96

// GUI defines, most of these are temporary
#define GUI_TILE_WIDTH 50
#define GUI_MELD_TILE_WIDTH 30
#define GUI_DISCARD_TILE_WIDTH 40
#define GUI_DISCARD_TILE_HEIGHT (GUI_DISCARD_TILE_WIDTH * 4 / 3)
#define GUI_RESERVED_TILE_WIDTH 40
#define GUI_DISCARD_AREA_DIMENSION 480

#define INDICATOR_DISPLAY_TIME_MS 1000
#define TIMEOUT_MS 100000

Q_DECLARE_METATYPE(PlayerId);
Q_DECLARE_METATYPE(RenderCallbackType);

#endif // GLOBALS_H
