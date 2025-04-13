#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Level.h"

class Player {
public:
    Player(Vector2 start);

    void  attach(Level* lvl) { level = lvl; }
    void  update(float dt);
    void  draw() const;

    void  reset(Vector2 start);

    Vector2 pos()   const { return position; }
    float   w()     const { return PW; }
    float   h()     const { return PH; }

    bool dead()     const { return isDead; }
    bool win()      const { return reachedGate; }
    int  score()    const { return points; }

private:
    static constexpr float PW = 32, PH = 32;
    static constexpr float SPEED = 150, JUMP = 350, GRAV = 600;

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
