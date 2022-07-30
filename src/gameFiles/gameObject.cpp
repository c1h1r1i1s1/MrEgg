#include "gameObject.h"

gameObject::gameObject(std::string& textureFile, bool active, float xPos, float yPos, sf::FloatRect hitBox, float rotation, int animationCycle, bool sharp, bool textured, bool moving, int moveType, float offset, bool finisher) {
    // Movement
    this->xSpeed = 0;
    this->ySpeed = 0;

    this->textureFile = textureFile;
    this->active = active;
    this->animationCycle = animationCycle;
    this->hitBox = hitBox;
    this->sharp = sharp;
    this->textured = textured;
    this->moving = moving;
    this->moveType = moveType;
    this->startX = xPos;
    this->startY = yPos;
    this->offset = offset;
    this->finisher = finisher;

    if (textured) {
        this->loadTexture(textureFile);
    }
    this->loadSprite(xPos, yPos);
}

void gameObject::setLocation(float startX, float startY) {
    this->objSprite.setPosition(sf::Vector2f(startX, startY));
}

sf::Vector2f gameObject::getLocation() {
    return this->objSprite.getPosition();
}

sf::Vector2f gameObject::getSpeed() {
    sf::Vector2f speed = sf::Vector2f(this->xSpeed, this->ySpeed);
    return speed;
}

sf::FloatRect gameObject::getBounds() {
    return this->objSprite.getGlobalBounds();
}

void gameObject::setYSpeed(float y) {
    this->ySpeed = y;
}

void gameObject::loadTexture(std::string& textureFile) {
    this->objTexture = new sf::Texture;
    if (!this->objTexture->loadFromFile(textureFile)) {
        std::cout << "Didn't load image" << std::endl;
    }
    //this->objTexture->setSmooth(true);
}

void gameObject::loadSprite(float xPos, float yPos) {
    if (this->active) {
        this->objRectSprite = new sf::IntRect(0, 64, 64, 64);
        this->objSprite.setTextureRect(*this->objRectSprite);
    }
    if (this->textured) {
        this->objSprite.setTexture(*this->objTexture);
    }
    this->objSprite.setHitbox(this->hitBox);
    this->objSprite.setPosition(sf::Vector2f(xPos, yPos));
}

void gameObject::calcMovement(float elapsed) {
    if (this->moving) {
        if (this->moveType == 1) {
            this->objSprite.setPosition(this->getSprite().getPosition().x, this->startY + 500*sin(3.5*elapsed + this->offset));
        }
    }
}

void gameObject::updateTexture() {
    this->objRectSprite->top = 0;
        if (this->objRectSprite->left >= 64*(this->animationCycle-1)) {
        this->objRectSprite->left = 0;
    } else {
        this->objRectSprite->left += 64;
    }
    this->objSprite.setTextureRect(*this->objRectSprite);
}

HitBoxSprite gameObject::getSprite() {
    return this->objSprite;
}