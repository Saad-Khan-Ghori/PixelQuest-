#include "Player.h"

Player::Player(Vector2 start) { reset(start); }

// ---------------------------------------------------------
void Player::reset(Vector2 start)
{
    position = start;
    vel = { 0,0 };
    canJump = true;
    isDead = false;
    reachedGate = false;
    points = 0;
}

// ---------------------------------------------------------
void Player::update(float dt)
{
    if (!level) return;

    // input
    vel.x = (IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT)) * SPEED;

    if (IsKeyPressed(KEY_SPACE) && canJump)
    {
        vel.y = -JUMP;
        canJump = false;
    }

    // gravity
    vel.y += GRAV * dt;
    if (vel.y > 600) vel.y = 600;

    moveY(dt);
    moveX(dt);

    // fell into pit?
    if (position.y > Level::ROWS * Level::TILE) isDead = true;
}

// ---------------------------------------------------------
void Player::moveY(float dt)
{
    position.y += vel.y * dt;
    Rectangle r = bounds();

    for (auto& row : level->map)
        for (auto& t : row)
        {
            if (!t.solid && t.type != COIN && t.type != LAVA && t.type != GATE) continue;

            Rectangle tr = { t.x, t.y,
                             (float)Level::TILE,
                             (t.type == GATE) ? Level::TILE * 2 : Level::TILE };

            if (!CheckCollisionRecs(r, tr)) continue;

            if (t.type == LAVA) { isDead = true; return; }
            if (t.type == GATE) { reachedGate = true; return; }

            // collide with solid
            if (vel.y > 0) { position.y = t.y - PH; canJump = true; }
            else { position.y = t.y + Level::TILE; }
            vel.y = 0;
            r.y = position.y;
        }

    // coins
    for (auto& row : level->map)
        for (auto& t : row)
            // In moveY() method, replace the coin collision check with:
            if (t.type == COIN && !t.taken)
            {
                Vector2 coinCenter = { t.x + Level::TILE / 2, t.y + Level::TILE / 2 };
                if (CheckCollisionCircleRec(coinCenter, 8, bounds()))
                {
                    t.taken = true;
                    points += 100;
                }
            }
}

// ---------------------------------------------------------
void Player::moveX(float dt)
{
    position.x += vel.x * dt;
    Rectangle r = bounds();

    for (auto& row : level->map)
        for (auto& t : row)
        {
            if (!t.solid && t.type != GATE) continue;

            Rectangle tr = { t.x, t.y,
                             (float)Level::TILE,
                             (t.type == GATE) ? Level::TILE * 2 : Level::TILE };

            if (!CheckCollisionRecs(r, tr)) continue;

            if (t.type == GATE) { reachedGate = true; return; }

            if (vel.x > 0) position.x = t.x - PW;
            else           position.x = t.x + Level::TILE;
            vel.x = 0;
            r.x = position.x;
        }
}

// ---------------------------------------------------------
void Player::draw() const
{
    DrawRectangle(position.x, position.y, PW, PH, BLUE);
    DrawText(TextFormat("Score: %d", points), 10, 10, 20, BLACK);
}
