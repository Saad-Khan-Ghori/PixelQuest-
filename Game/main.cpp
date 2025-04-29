#include "raylib.h"
#include "Level.h"
#include "Player.h"

enum State { PLAY, OVER, WIN };

int main(){
    Vector2 startPos = {
    100.0f,
    static_cast<float>((Level::ROWS - 1) * Level::TILE - Player(startPos).h()) // Ground position
    };
    Vector2 position = { 0,0 };
    Player player(startPos);
    const int SW = 800, SH = 400;
    InitWindow(SW, SH, "Pixel Quest");
    SetTargetFPS(60);
    Texture2D background = LoadTexture("Background.png");


    Level  level;
    //Player player({ 100, 100 });
    player.attach(&level);

    Camera2D cam{};
    cam.offset = { SW / 2.0f, SH / 2.0f };
    cam.zoom = 1.0f;

    State state = PLAY;

    InitAudioDevice();
    Sound jumpSound = LoadSound("jump.mp3");
    Sound coinSound = LoadSound("coin.mp3");

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();


        if (state == PLAY)
        {
            player.update(dt);

            // Camera follow
            cam.target = { player.pos().x + player.w() / 2,
                           player.pos().y + player.h() / 2 };


            float LW = Level::COLS * Level::TILE,
                LH = Level::ROWS * Level::TILE,
                hw = SW / 2.0f, hh = SH / 2.0f;

            if (LW > SW)
            {
                if (cam.target.x < hw) cam.target.x = hw;
                if (cam.target.x > LW - hw) cam.target.x = LW - hw;
            }
            if (LH > SH)
            {
                if (cam.target.y < hh) cam.target.y = hh;
                if (cam.target.y > LH - hh) cam.target.y = LH - hh;
            }

            if (player.dead()) state = OVER;
            if (player.win())  state = WIN;
        }
        else if (state == OVER && IsKeyPressed(KEY_R)) {
            level = Level();
            player.reset(startPos);  // Use the same ground position
            player.attach(&level);
            state = PLAY;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(background, 0, 0, WHITE);
        float scaleX = (float)GetScreenWidth() / background.width;
        float scaleY = (float)GetScreenHeight() / background.height;
        DrawTextureEx(background, position, 0.0f, scaleX, WHITE); // scaleX for width
        DrawTextureEx(background, position, 0.0f, scaleY, WHITE); // scaleY for height

        if (state == PLAY)
        {
            BeginMode2D(cam);
            level.draw();
            player.draw();
            EndMode2D();

            // Draw fixed UI elements
            DrawText(TextFormat("Score: %d", player.score()), 10, 10, 20, BLACK);
        }
        else if (state == OVER)
        {
            DrawText("GAME OVER", SW / 2 - 140, SH / 2 - 20, 40, RED);
            DrawText("Press R to restart", SW / 2 - 140, SH / 2 + 30, 20, DARKGRAY);
        }
        else if (state == WIN)
        {

            DrawText("YOU WIN!", SW / 2 - 100, SH / 2 - 40, 40, GREEN);
            DrawText(TextFormat("Final Score: %d", player.score()),
                SW / 2 - 100, SH / 2, 20, DARKGRAY);
            DrawText("Press R to replay", SW / 2 - 100, SH / 2 + 30, 20, DARKGRAY);
        }

        EndDrawing();
    }
    UnloadTexture(background);
    CloseWindow();
    return 0;
}
