#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "HitBoxSprite.h"
#include <cmath>

class Player {
    private:
        sf::Texture eggTexture;
        sf::IntRect* eggRectSprite;
        HitBoxSprite eggSprite;

        int moveAccel;
        int moveDeccel;
        int turnAccel;
        float xSpeed;
        float ySpeed;
        float maxRunSpeed;
        float jumpSpeed;
        float boostSpeed;
        float frictionForce;
        float boostTick;
        int gravityAccel;
        int textureTick;
        int health;
        float invincibleTimer;
        bool textureSwitch;
        enum moveStatus { nothing, jumped, dashed };
        moveStatus currentMove;
    
    public:
        bool movingRight;
        bool movingLeft;
        bool movingUp;
        bool movingDown;
        bool paused;
        bool jumping;
        bool inAir;
        bool boostUsed;
        bool doubleJumpUsed;
        bool justJumped;
        bool invincible;

        Player();
        void setLocation(float startX, float startY);
        sf::Vector2f getLocation();
        sf::Vector2f getSpeed();
        void loadTexture();
        void loadSprite();
        void calcMovement(float timeChangeMilli);
        void updateTexture();
        sf::FloatRect getBounds();
        void movePlayer(float x, float y);
        void setYSpeed(float y);
        void setXSpeed(float x);
        HitBoxSprite getSprite();
        void affectGravity(float timeChangeMilli);
        bool damage(int damageAmount);
        void reset();
        int status();
};