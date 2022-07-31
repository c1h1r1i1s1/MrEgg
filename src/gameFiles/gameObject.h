#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "HitBoxSprite.h"
#include <cmath>

class gameObject {
    private:
        sf::Texture *objTexture;
        sf::IntRect* objRectSprite;
        HitBoxSprite objSprite;

        // Movement stuff
        float xSpeed;
        float ySpeed;
        std::string textureFile;
        int moveType;
        int animationCycle;
        sf::FloatRect hitBox;
        float startX;
        float startY;
        float offset;
        float scale;
        float boxScale;
    
    public:
        bool active;
        bool sharp;
        bool textured;
        bool moving;
        bool finisher;
        bool landing;
        bool finished;
        sf::Vector2f getLocation();
        sf::Vector2f getSpeed();
        sf::FloatRect getBounds();
        HitBoxSprite getSprite();
        
        gameObject(std::string& textureFile, bool active, float xPos, float yPos, sf::FloatRect hitBox, float rotation, int animationCycle, bool sharp, bool textured, bool moving, int moveType, float offset, bool finisher, float boxScale, float scale);
        void setLocation(float startX, float startY);
        void loadTexture(std::string& textureFile);
        void loadSprite(float xPos, float yPos);
        void updateTexture();
        void setYSpeed(float y);
        void calcMovement(float elapsed);
};