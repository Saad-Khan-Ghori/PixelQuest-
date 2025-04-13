#ifndef TILE_H
#define TILE_H

#include "raylib.h"

enum TileType {
    EMPTY,
    GROUND,
    PLATFORM,
    COIN,
    LAVA,
    GATE          // finish line
};

struct Tile {
    float     x{}, y{};
    TileType  type = EMPTY;
    bool      solid = false;   // can collide?
    bool      taken = false;   // for coins

    Tile(float X = 0, float Y = 0,
        TileType t = EMPTY, bool s = false)
        : x(X), y(Y), type(t), solid(s) {
    }
};

#endif
