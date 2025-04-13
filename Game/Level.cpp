#include "Level.h"
#include "raylib.h"

Level::Level()
{
    map.resize(ROWS, std::vector<Tile>(COLS));

    for (int y = 0; y < ROWS; ++y)
        for (int x = 0; x < COLS; ++x)
        {
            float tx = x * TILE, ty = y * TILE;

            // ---------- ground with pits ----------
     // ---------- Ground with pits ----------
            if (y == ROWS - 1)
            {
                bool pit = ((x / 10) & 1) && (x % 10 < 3); // Wider spacing
                map[y][x] = pit ? Tile(tx, ty, EMPTY, false)
                    : Tile(tx, ty, GROUND, true);
            }

            // ---------- Sparse platforms ----------
            if (y == ROWS - 4) // Higher platform row
            {
                // Platform every 20 columns with alternating offsets
                if ((x % 20 >= 5 && x % 20 <= 8) ||
                    (x % 20 >= 15 && x % 20 <= 18))
                {
                    map[y][x] = Tile(tx, ty, PLATFORM, true);
                    if (x % 20 == 6 || x % 20 == 16) // Coins above center
                        map[y - 1][x] = Tile(tx, ty - TILE, COIN, false);
                }
            }

            // ---------- Final Gate ----------
            if (x == COLS - 6 && y >= ROWS - 5)
            {
                // Tall 2x3 gate structure
                map[y][x] = Tile(tx, ty, GATE, true);
                map[y][x + 1] = Tile(tx + TILE, ty, GATE, true);
                map[y][x + 2] = Tile(tx + TILE * 2, ty, GATE, true);
            }

            // ---------- Lava pool ----------
            if (y == ROWS - 1 && x > COLS - 15)
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
            case GROUND:   DrawRectangle(t.x, t.y, TILE, TILE, DARKGRAY); break;
            case PLATFORM: DrawRectangle(t.x, t.y, TILE, TILE, LIGHTGRAY); break;
            case LAVA:     DrawRectangle(t.x, t.y, TILE, TILE, RED);      break;
            case COIN:
                if (!t.taken)
                    DrawCircle(t.x + TILE / 2, t.y + TILE / 2, 8, YELLOW);
                break;
            case GATE:
                DrawRectangle(t.x, t.y, TILE, TILE * 2, GREEN); break;
            default: break;
            }
        }
}
