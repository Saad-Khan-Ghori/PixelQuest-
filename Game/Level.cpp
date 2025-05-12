#include "Level.h"
#include "raylib.h"
void Level::reset() {
    
    UnloadTexture(texPlatform);
    UnloadTexture(texLava);
    

    
    texPlatform = LoadTexture("wood.png");
    texLava = LoadTexture("lava.png");
   
}
Level::Level(){
    texPlatform = LoadTexture("wood.png");
    texLava = LoadTexture("lava.png");
    map.resize(ROWS, vector<Tile>(COLS));
         const int SAFE_ZONE = 15;
    for (int y = 0; y < ROWS; ++y)
        for (int x = 0; x < COLS; ++x)
        {
            float tx = x * TILE, ty = y * TILE;

            if (y == ROWS - 1) {
                // First 10 tiles are always safe ground
                if (x < 10) {
                    map[y][x] = Tile(tx, ty, GROUND, true);
                }
                else {
                    bool pit = ((x / 10) & 1) && (x % 10 < 3);
                    if (pit) {
                        int pitWidth = 2;
                        // Ensure we don't create pits near start
                        if (x + pitWidth < COLS) {
                            for (int i = 0; i < pitWidth; i++) {
                                map[y][x + i] = Tile(tx + i * TILE, ty, EMPTY, false); // Fixed position calculation
                            }
                            x += pitWidth - 1;
                        }
                    }
                    else {
                        map[y][x] = Tile(tx, ty, GROUND, true);
                    }
                }
            }


            if (y == ROWS - 4 && x < COLS - 20&& GetRandomValue(0, 100) < 60)
            {
                if ((x % 20 >= 5 && x % 20 <= 8) ||
                    (x % 20 >= 15 && x % 20 <= 18))
                {
                    map[y][x] = Tile(tx, ty, PLATFORM, true);

                    if (x < COLS - 25 && (x % 20 == 6 || x % 20 == 16))
                        map[y - 1][x] = Tile(tx, ty - TILE, COIN, false);
                }
            }

            if (x == COLS - 6 && y >= ROWS - 5)
            {
                map[y][x] = Tile(tx, ty, GATE, true);
                map[y][x + 1] = Tile(tx + TILE, ty, GATE, true);
                map[y][x + 2] = Tile(tx + TILE * 2, ty, GATE, true);
            }


            if (y == ROWS - 1 && x > COLS - 11)
                map[y][x] = Tile(tx, ty, LAVA, true);
        }
}

void Level::draw() const
{
    for (const auto& row : map)
        for (const auto& t : row)
        {
            switch (t.type)
            {
            case GROUND:    DrawRectangle(t.x, t.y, TILE, TILE, Color{ 210, 175, 120, 255 }); break;
            case PLATFORM: DrawTexture(texPlatform, t.x, t.y, WHITE); break;
            case LAVA:     DrawTexture(texLava, t.x, t.y, WHITE);      break;
            case COIN:
                if (!t.taken)
                    DrawCircle(t.x + TILE / 2, t.y + TILE / 2, 8, YELLOW);
                break;
            case GATE:
                DrawRectangle(t.x, t.y, TILE, TILE, Color{ 180, 255, 180, 255 });
                 break;
            default: break;
            }
        }
}
