#include <iostream>
#include <algorithm>
#include <cstdio> // for snprintf
#include "raylib.h"
#include "Level.h"
#include "Player.h"

enum State { TITLE, PLAY, OVER, WIN };

int main() {
    const int SW = 800, SH = 400;
    Vector2 startPos = { 2 * Level::TILE, (Level::ROWS - 3) * Level::TILE };

    // Raylib init
    InitWindow(SW, SH, "Pixel Quest");
    SetTargetFPS(60);
    Texture2D background = LoadTexture("Background.png");

    Level level;
    Player player(startPos);
    player.attach(&level);

    Camera2D cam{};
    cam.offset = { SW / 2.0f, SH / 2.0f };
    cam.zoom = 1.0f;

    InitAudioDevice();
    Sound jumpSound = LoadSound("jump.wav");
    Sound coinSound = LoadSound("coin.wav");
    Sound winSound = LoadSound("win.wav");

    State state = TITLE;
    float startTime = 0.0f;
    float finishTime = 0.0f;

    while (!WindowShouldClose()) {
        // Title Screen
        if (state == TITLE) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(background, 0, 0, WHITE);
            const char* titleText = "PIXEL QUEST";
            int titleFontSize = 40;
            int titleWidth = MeasureText(titleText, titleFontSize);
            int titleX = SW / 2 - titleWidth / 2;
            int titleY = SH / 2 - 60;
            DrawRectangle(titleX - 10, titleY - 10, titleWidth + 20, titleFontSize + 20, Fade(WHITE, 0.7f));
            DrawText(titleText, titleX, titleY, titleFontSize, BLACK);
            const char* promptText = "Press ENTER to start";
            int promptFontSize = 20;
            int promptWidth = MeasureText(promptText, promptFontSize);
            int promptX = SW / 2 - promptWidth / 2;
            int promptY = SH / 2 + 20;
            DrawRectangle(promptX - 10, promptY - 5, promptWidth + 20, promptFontSize + 10, Fade(WHITE, 0.7f));
            DrawText(promptText, promptX, promptY, promptFontSize, DARKGRAY);
            EndDrawing();

            if (IsKeyPressed(KEY_ENTER)) {
                state = PLAY;
                startTime = GetTime();
            }
            continue;
        }

        float dt = GetFrameTime();

        // State transitions
        if (state == PLAY && player.win()) {
            finishTime = GetTime() - startTime;
            state = WIN;
            PlaySound(winSound);
        }
        if (state == PLAY && player.dead()) {
            state = OVER;
            PlaySound(coinSound);
        }

        // Restart input
        if ((state == OVER || state == WIN) && IsKeyPressed(KEY_R)) {
            level.reset();
            player.reset(startPos);
            player.attach(&level);
            state = PLAY;
            startTime = GetTime();
        }

        // Game update
        if (state == PLAY) {
            player.update(dt);
            cam.target = { player.pos().x + player.w() / 2, player.pos().y + player.h() / 2 };
            float LW = Level::COLS * Level::TILE;
            float LH = Level::ROWS * Level::TILE;
            float hw = SW / 2.0f;
            float hh = SH / 2.0f;
            if (LW > SW) cam.target.x = std::clamp(cam.target.x, hw, LW - hw);
            if (LH > SH) cam.target.y = std::clamp(cam.target.y, hh, LH - hh);
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(background, 0, 0, WHITE);

        if (state == PLAY) {
            BeginMode2D(cam);
            level.draw();
            player.draw();
            EndMode2D();
            char scoreText[32];
            std::snprintf(scoreText, sizeof(scoreText), "Score: %d", player.score());
            DrawRectangle(0, 0, MeasureText(scoreText, 20) + 20, 30, Fade(WHITE, 0.7f));
            DrawText(scoreText, 10, 5, 20, BLACK);
        }
        else if (state == OVER) {
            const char* overText = "GAME OVER";
            int overFont = 40;
            int overW = MeasureText(overText, overFont);
            int overX = SW / 2 - overW / 2;
            int overY = SH / 2 - 20;
            DrawRectangle(overX - 10, overY - 10, overW + 20, overFont + 20, Fade(WHITE, 0.7f));
            DrawText(overText, overX, overY, overFont, RED);

            const char* restartText = "Press R to restart";
            int restartFont = 20;
            int restartW = MeasureText(restartText, restartFont);
            int restartX = SW / 2 - restartW / 2;
            int restartY = SH / 2 + 30;
            DrawRectangle(restartX - 10, restartY - 5, restartW + 20, restartFont + 10, Fade(WHITE, 0.7f));
            DrawText(restartText, restartX, restartY, restartFont, DARKGRAY);
        }
        else if (state == WIN) {
            const char* winText = "YOU WIN!";
            int winFont = 40;
            int winW = MeasureText(winText, winFont);
            int winX = SW / 2 - winW / 2;
            int winY = SH / 2 - 60;
            DrawRectangle(winX - 10, winY - 10, winW + 20, winFont + 20, Fade(WHITE, 0.7f));
            DrawText(winText, winX, winY, winFont, GREEN);

            char finalScore[32];
            std::snprintf(finalScore, sizeof(finalScore), "Final Score: %d", player.score());
            int fsFont = 20;
            int fsW = MeasureText(finalScore, fsFont);
            int fsX = SW / 2 - fsW / 2;
            int fsY = SH / 2;
            DrawRectangle(fsX - 10, fsY - 5, fsW + 20, fsFont + 10, Fade(WHITE, 0.7f));
            DrawText(finalScore, fsX, fsY, fsFont, DARKGRAY);

            char timeText[32];
            std::snprintf(timeText, sizeof(timeText), "Time: %.2f s", finishTime);
            int tFont = 20;
            int tW = MeasureText(timeText, tFont);
            int tX = SW / 2 - tW / 2;
            int tY = SH / 2 + 40;
            DrawRectangle(tX - 10, tY - 5, tW + 20, tFont + 10, Fade(WHITE, 0.7f));
            DrawText(timeText, tX, tY, tFont, DARKGRAY);

            const char* replayText = "Press R to replay";
            int rFont = 20;
            int rW = MeasureText(replayText, rFont);
            int rX = SW / 2 - rW / 2;
            int rY = SH / 2 + 80;
            DrawRectangle(rX - 10, rY - 5, rW + 20, rFont + 10, Fade(WHITE, 0.7f));
            DrawText(replayText, rX, rY, rFont, DARKGRAY);
        }

        if (IsKeyPressed(KEY_SPACE)) PlaySound(jumpSound);
        EndDrawing();
    }

    // Cleanup
    UnloadSound(jumpSound);
    UnloadSound(coinSound);
    UnloadSound(winSound);
    UnloadTexture(background);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
