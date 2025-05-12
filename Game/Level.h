#ifndef LEVEL_H
#define LEVEL_H

#include "Tile.h"
#include <vector>
using namespace std;
class Level {
public:
    static constexpr int TILE = 32;
    static constexpr int ROWS = 12;
    static constexpr int COLS = 100;   // 3200?px wide
    Texture2D texPlatform;
    Texture2D texGround;
    Texture2D texLava;
    Texture2D texGate;
    vector<vector<Tile>> map;

    Level();
    void reset();
    void draw() const;
    ~Level() {
        UnloadTexture(texPlatform);
        UnloadTexture(texGround);
        UnloadTexture(texLava);
        UnloadTexture(texGate);
    }
};

#endif
