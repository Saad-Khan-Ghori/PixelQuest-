#include "Player.h"

Player::Player(Vector2 start) { reset(start); }
Sound jumpSound = LoadSound("jump.mp3");
Sound coinSound = LoadSound("coin.mp3");
void Player::reset(Vector2 start)
{
    position = start;
    vel = { 0,0 };
    canJump = true;
    isDead = false;
    reachedGate = false;
    points = 0;
}

void Player::update(float dt)
{
    if (!level) return;

    // Input
    vel.x = (IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT)) * SPEED;

    if (IsKeyPressed(KEY_SPACE) && canJump){
        PlaySound(jumpSound);
        vel.y = -JUMP;
        canJump = false;
    }

    // Gravity
    vel.y += GRAV * dt;
    if (vel.y > 600) vel.y = 600;

    moveY(dt);
    moveX(dt);
    collectCoins();  // Now properly declared

    // Fell into pit?
    if (position.y > Level::ROWS * Level::TILE) isDead = true;
}

// Implement collectCoins properly
void Player::collectCoins()
{
    for (auto& row : level->map)
        for (auto& t : row)
        {
            if (t.type == COIN && !t.taken)
            {
                Rectangle coinRect = { t.x, t.y, (float)Level::TILE, (float)Level::TILE };
                if (CheckCollisionRecs(coinRect, this->bounds()))
                {
                    t.taken = true;
                    points += 100;
                    // Add coin collection sound here
                }
            }
        }
}

// Modify the tile checking in moveY function
void Player::moveY(float dt)
{
    position.y += vel.y * dt;
    Rectangle r = bounds();

    for (auto& row : level->map)
        for (auto& t : row)
        {
            // Skip non-solid tiles except for LAVA and GATE
            if (!t.solid && t.type != LAVA && t.type != GATE) continue;

            Rectangle tr = { t.x, t.y, (float)Level::TILE,
                (t.type == GATE) ? Level::TILE * 2 : Level::TILE };

            if (!CheckCollisionRecs(r, tr)) continue;

            if (t.type == LAVA) { isDead = true; return; }
            if (t.type == GATE) { reachedGate = true; return; }

            // Collide with solid
            if (vel.y > 0) {
                position.y = t.y - PH - 1;
                canJump = true;
            }
            else {
                position.y = t.y + Level::TILE;
            }
            vel.y = 0;
            r.y = position.y;
        }
}

void Player::moveX(float dt)
{
    position.x += vel.x * dt;
    Rectangle r = bounds();

    for (auto& row : level->map)
        for (auto& t : row)
        {
            if (!t.solid && t.type != GATE) continue;

            Rectangle tr = { t.x, t.y, (float)Level::TILE,
                (t.type == GATE) ? Level::TILE * 2 : Level::TILE };

            if (!CheckCollisionRecs(r, tr)) continue;

            if (t.type == GATE) { reachedGate = true; return; }

            if (vel.x > 0) position.x = t.x - PW;
            else           position.x = t.x + Level::TILE;
            vel.x = 0;
            r.x = position.x;
        }
}

void Player::draw() const
{
    DrawRectangle(position.x, position.y, PW, PH, BLUE);
   
}
