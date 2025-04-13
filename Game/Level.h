#ifndef LEVEL_H
#define LEVEL_H

#include "Tile.h"
#include <vector>

class Level {
public:
    static constexpr int TILE = 32;
    static constexpr int ROWS = 12;
    static constexpr int COLS = 100;   // 3200?px wide

    std::vector<std::vector<Tile>> map;

    Level();
    void draw() const;
};

#endif
