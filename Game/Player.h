#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "raylib.h"
#include "Level.h"

class Player {
public:
    
    int coins;
    Player(Vector2 start);

    Sound jumpSound;
    Sound coinSound;

    void  attach(Level* lvl) { level = lvl; }
    void  update(float dt);
    void  draw() const;
    void  reset(Vector2 start);
    void  collectCoins();  // Added declaration
    static constexpr float SCALE = 1.7f;
    Vector2 pos()   const { return position; }
    int w()    const { return int(PW * SCALE); }
    int h()    const { return int(PH * SCALE); }

    bool dead()     const { return isDead; }
    bool win()      const { return reachedGate; }
    int  score()    const { return points; }

    ~Player() {
        UnloadTexture(texIdle);
        UnloadTexture(texRun);
        UnloadTexture(texJump);
    }


private:
    static constexpr float PW = 32, PH = 32;
    static constexpr float SPEED = 150, JUMP = 350, GRAV = 600;

    Texture2D texIdle{};
    Texture2D texRun{};
    Texture2D texJump{};
    enum AnimState { AS_IDLE, AS_RUN, AS_JUMP, AS_FALL } animState = AS_IDLE;
    int     currentFrame = 0;
    float   frameTimer = 0.f;
    float   frameDuration = 0.1f;      
    int     framesIdle = 0;
    int     framesRun = 0;

    Vector2 position{}, vel{};
    bool    canJump = true;
    bool    isDead = false;
    bool    reachedGate = false;
    int     points = 0;
    Level* level = nullptr;

    Rectangle bounds() const { return { position.x, position.y, PW, PH }; }
    void  moveY(float dt);
    void  moveX(float dt);
};


#endif
