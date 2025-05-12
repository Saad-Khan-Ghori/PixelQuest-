#include "Player.h"

Player::Player(Vector2 start) { 
    texIdle = LoadTexture("Idle (32x32).png");
    texRun = LoadTexture("Run (32x32).png");
    texJump = LoadTexture("Jump (32x32).png");
    framesIdle = texIdle.width / PW; 
    framesRun = texRun.width / PW;
    reset(start);
    
}

void Player::reset(Vector2 start)
{
    std::cout << "Spawning player at (" << position.x << ", " << position.y << ")\n";
    position = start;
    vel = { 0,0 };
    canJump = true;
    isDead = false;
    reachedGate = false;
    points = 0;
    animState = AS_IDLE;
    currentFrame = 0;
    frameTimer = 0.f;
}

void Player::update(float dt)
{
    Sound fallSound = LoadSound("fall.wav");
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
    if (vel.y < 0)                         animState = AS_JUMP;
    else if (vel.y > 0 && !canJump)             animState = AS_FALL;
    else if (std::abs(vel.x) > 0.1f)            animState = AS_RUN;
    else                                        animState = AS_IDLE;

    // advance frame only for Idle & Run
    if (animState == AS_IDLE || animState == AS_RUN) {
        frameTimer += dt;
        if (frameTimer >= frameDuration) {
            frameTimer -= frameDuration;
            int maxF = (animState == AS_IDLE ? framesIdle : framesRun);
            currentFrame = (currentFrame + 1) % maxF;
        }
    }
    else {
        // single?frame for jump & fall
        currentFrame = 0;
    }
    collectCoins();  // Now properly declared
    PlaySound(coinSound);

    // Fell into pit?
    if (position.y > Level::ROWS * Level::TILE) {
        PlaySound(fallSound);
        isDead = true;
    }
}

// Implement collectCoins properly
void Player::collectCoins()
{
    Sound coinSound = LoadSound("coin.wav");
    for (auto& row : level->map)
        for (auto& t : row)
        {
            if (t.type == COIN && !t.taken)
            {
                Rectangle coinRect = { t.x, t.y, (float)Level::TILE, (float)Level::TILE };
                if (CheckCollisionRecs(coinRect, this->bounds()))
                {
                    PlaySound(coinSound);
                    t.taken = true;
                    points += 100;
                    PlaySound(coinSound);
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
    const Texture2D& tex =
        (animState == AS_RUN ? texRun :
            animState == AS_IDLE ? texIdle :
            /*JUMP or FALL*/       texJump);

    // source rectangle from frame index
    Rectangle src = {
        currentFrame * PW, 0.f,
        (float)PW, (float)PH
    };
    Rectangle dst = {
     position.x, position.y,
     float(PW) * SCALE,
     float(PH) * SCALE
    };
    DrawTexturePro(tex, src, dst, { 0,0 }, 0.f, WHITE);
   
}
