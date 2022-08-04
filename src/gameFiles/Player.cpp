#include "Player.h"

Player::Player() {
    this->movingRight = false;
    this->movingLeft = false;
    this->movingUp = false;
    this->movingDown = false;
    this->paused = false;
    this->jumping = false;
    this->inAir = false;
    this->boostUsed = false;
    this->doubleJumpUsed = false;
    this->boostTick = 0;
    this->justJumped = false;
    this->textureTick = 0;
    this->health = 100;
    this->invincibleTimer = -1;
    this->invincible = false;
    this->textureSwitch = false;

    // Movement
    this->moveAccel = 3000;
    this->moveDeccel = 4000;
    this->turnAccel = 6000;
    this->xSpeed = 0;
    this->ySpeed = 0;
    this->maxRunSpeed = 900;
    this->jumpSpeed = -1500;
    this->boostSpeed = 1800;
    this->gravityAccel = 4000;
}

void Player::setLocation(float startX, float startY) {
    this->eggSprite.setPosition(sf::Vector2f(startX, startY));
}

sf::Vector2f Player::getLocation() {
    return this->eggSprite.getPosition();
}

sf::Vector2f Player::getSpeed() {
    sf::Vector2f speed = sf::Vector2f(this->xSpeed, this->ySpeed);
    return speed;
}

sf::FloatRect Player::getBounds() {
    return this->eggSprite.getGlobalBounds();
}

void Player::movePlayer(float x, float y) {
    this->eggSprite.move(x, y);
}

void Player::setYSpeed(float y) {
    this->ySpeed = y;
}

void Player::setXSpeed(float x) {
    this->xSpeed = x;
}

void Player::loadTexture() {
    if (!this->eggTexture.loadFromFile("src/Images/Egg Animations 4.png")) {
        std::cout << "Didn't load image" << std::endl;
    }
}

void Player::loadSprite() {
    this->eggRectSprite = new sf::IntRect(0, 0, 64, 64);
    this->eggSprite.setTexture(this->eggTexture);
    this->eggSprite.setTextureRect(*this->eggRectSprite);
    this->eggSprite.setHitbox(sf::FloatRect(8.f, 0, 48.f, 64.f));
    this->eggSprite.setOrigin(sf::Vector2f(32.f, 32.f));
    this->eggSprite.setScale(sf::Vector2f(3.f, 3.f));
    this->eggSprite.setPosition(sf::Vector2f(7000.f, 200.f));
}

void Player::calcMovement(float timeChangeMilli) {
    // Air friction section, done with square negation
    float fraction = this->xSpeed/this->maxRunSpeed;
    this->frictionForce = pow(fraction, 6);
    if (this->xSpeed > 0) {
        this->xSpeed -= this->frictionForce;
    } else if (this->xSpeed < 0) {
        this->xSpeed += this->frictionForce;
    }

    // Check invincible timer
    if (this->invincibleTimer >= 0) {
        this->invincibleTimer += timeChangeMilli;
    }
    if (this->invincibleTimer > 1) {
        this->invincible = false;
        this->invincibleTimer = -1;
    }

    // Jumping section
    if (this->jumping) {
        this->justJumped = true;
        this->jumping = false;
        if (this->inAir) {
            if (this->movingLeft != this->movingRight) {
                if (!this->boostUsed) {
                    if (this->movingLeft && this->movingRight) {
                        if (!this->doubleJumpUsed) {
                            this->ySpeed = std::min(this->jumpSpeed, this->ySpeed);
                            this->doubleJumpUsed = true;
                        }
                    } else if (this->movingLeft && this->movingUp) {
                        if (!this->doubleJumpUsed) {
                            this->ySpeed = std::min(this->jumpSpeed, this->ySpeed);
                            this->doubleJumpUsed = true;
                        }
                    } else if (this->movingRight && this->movingUp) {
                        if (!this->doubleJumpUsed) {
                            this->ySpeed = std::min(this->jumpSpeed, this->ySpeed);
                            this->doubleJumpUsed = true;
                        }
                    } else if (this->movingRight) {
                        this->xSpeed = std::max(this->boostSpeed, this->xSpeed);
                        this->boostTick = 0;
                        this->ySpeed = 0;
                        this->boostUsed = true;
                    } else if (this->movingLeft) {
                        this->xSpeed = std::min(-1 * this->boostSpeed, this->xSpeed);
                        this->boostTick = 0;
                        this->boostUsed = true;
                        this->ySpeed = 0;
                    } else {
                        if (!this->doubleJumpUsed) {
                            this->ySpeed = std::min(this->jumpSpeed, this->ySpeed);
                            this->doubleJumpUsed = true;
                        }
                    }
                } else {
                    if (!this->doubleJumpUsed) {
                        this->ySpeed = std::min(this->jumpSpeed, this->ySpeed);
                        this->doubleJumpUsed = true;
                    }
                }
            } else {
                if (!this->doubleJumpUsed) {
                    this->ySpeed = std::min(this->jumpSpeed, this->ySpeed);
                    this->doubleJumpUsed = true;
                }
            }
        } else {
            this->inAir = true;
            this->ySpeed = this->jumpSpeed;
        }
    }

    // Main control accel section
    if (this->movingLeft && !this->movingRight) { // Moving Left
        float xChange;
        if (xSpeed > 0) {
            xChange = this->xSpeed * timeChangeMilli + 0.5 * -1 * this->turnAccel * timeChangeMilli * timeChangeMilli;
            this->xSpeed = this->xSpeed + -1 * this->turnAccel * timeChangeMilli;
        } else if (this->xSpeed > -1*this->maxRunSpeed) {
            xChange = this->xSpeed * timeChangeMilli + 0.5 * -1 * this->moveAccel * timeChangeMilli * timeChangeMilli;
            this->xSpeed = this->xSpeed + -1 * this->moveAccel * timeChangeMilli;
        } else {
            xChange = this->xSpeed * timeChangeMilli;
        }
        this->eggSprite.move(xChange, 0);
    } else if (this->movingRight && !this->movingLeft) { // Moving Right
        float xChange;
        if (this->xSpeed < 0) {
            xChange = this->xSpeed * timeChangeMilli + 0.5 * this->turnAccel * timeChangeMilli * timeChangeMilli;
            this->xSpeed = this->xSpeed + this->turnAccel * timeChangeMilli;
        } else if (this->xSpeed < this->maxRunSpeed) {
            xChange = this->xSpeed * timeChangeMilli + 0.5 * this->moveAccel * timeChangeMilli * timeChangeMilli;
            this->xSpeed = this->xSpeed + this->moveAccel * timeChangeMilli;
        } else {
            xChange = this->xSpeed * timeChangeMilli;
        }
        this->eggSprite.move(xChange, 0);
    } else { // Still
        if (this->xSpeed > 0) { // If moving right, force accel left
            float xChange = this->xSpeed * timeChangeMilli + 0.5 * -1 * this->moveDeccel * timeChangeMilli * timeChangeMilli;
            this->xSpeed = this->xSpeed + -1 * this->moveDeccel * timeChangeMilli;
            if (this->xSpeed > 0) {
                this->eggSprite.move(xChange, 0);
            } else {
                this->xSpeed = 0;
            }
        } else if (this->xSpeed < 0) { // If moving left, force accel right
            float xChange = this->xSpeed * timeChangeMilli + 0.5 * this->moveDeccel * timeChangeMilli * timeChangeMilli;
            this->xSpeed = this->xSpeed + this->moveDeccel * timeChangeMilli;
            if (this->xSpeed < 0) {
                this->eggSprite.move(xChange, 0);
            } else {
                this->xSpeed = 0;
            }
        }
    }
}

void Player::affectGravity(float timeChangeMilli) {
    if (timeChangeMilli > 0.2) {
        return;
    }
    // Gravity effect
    // First check for boost gravity
    if (this->boostUsed && this->inAir) {
        if (this->boostTick > 0.35 && this->boostTick < 0.5) {
            this->ySpeed = this->jumpSpeed/2;
        } else if (this->boostTick > 0.5) {
            float yChange = this->ySpeed * timeChangeMilli + 0.5 * this->gravityAccel * timeChangeMilli * timeChangeMilli;
            this->movePlayer(0, yChange);
            this->setYSpeed(this->ySpeed + this->gravityAccel * timeChangeMilli);
        }
        this->boostTick += timeChangeMilli;
    } else {
        float yChange = this->ySpeed * timeChangeMilli + 0.5 * this->gravityAccel * timeChangeMilli * timeChangeMilli;
        this->movePlayer(0, yChange);
        this->setYSpeed(this->ySpeed + this->gravityAccel * timeChangeMilli);
    }
}

void Player::updateTexture() {
    // For invincible flickering
    if (this->invincible) {
        if (this->textureSwitch) {
            this->textureSwitch = false;
        } else {
            this->textureSwitch = true;
        }
    } else {
        this->textureSwitch = false;
    }

    if (this->xSpeed > 0) {
        this->textureTick = 0;
        if (this->inAir) {
            if (this->boostUsed && this->boostTick < 0.5) {
                this->eggSprite.setHitbox(sf::FloatRect(4.f, 8, 56.f, 48.f));
                this->eggRectSprite->top = 5*64 + 1;
                if (this->eggRectSprite->left >= 64*7) {
                    this->eggRectSprite->left = 0;
                } else {
                    this->eggRectSprite->left += 64;
                }
            } else {
                this->eggSprite.setHitbox(sf::FloatRect(12.f, 0, 40.f, 64.f));
                this->eggRectSprite->top = 3*64 + 1;
                if (this->justJumped && this->doubleJumpUsed) {
                    if (this->eggRectSprite->left <= 64*2) {
                        this->eggRectSprite->left += 64;
                    } else {
                        this->eggRectSprite->left = 64*3;
                    }
                } else {
                    this->eggRectSprite->left = 0;
                }
                
            }
        } else {
            this->eggSprite.setHitbox(sf::FloatRect(14.f, 0, 36.f, 64.f));
            this->eggRectSprite->top = 65;
            if (this->eggRectSprite->left >= 64*7) {
                this->eggRectSprite->left = 0;
            } else {
                this->eggRectSprite->left += 64;
            }
        }
    } else if (this->xSpeed < 0) {
        this->textureTick = 0;
        if (this->inAir) {
            if (this->boostUsed && this->boostTick < 0.5) {
                this->eggSprite.setHitbox(sf::FloatRect(4.f, 8, 56.f, 48.f));
                this->eggRectSprite->top = 6*64 + 1;
                if (this->eggRectSprite->left >= 64*7) {
                    this->eggRectSprite->left = 0;
                } else {
                    this->eggRectSprite->left += 64;
                }
            } else {
                this->eggSprite.setHitbox(sf::FloatRect(12.f, 0, 40.f, 64.f));
                this->eggRectSprite->top = 4*64 + 1;
                if (this->justJumped && this->doubleJumpUsed) {
                    if (this->eggRectSprite->left <= 64*2) {
                        this->eggRectSprite->left += 64;
                    } else {
                        this->eggRectSprite->left = 64*3;
                    }
                } else {
                    this->eggRectSprite->left = 0;
                }
                
            }
        } else {
            this->eggSprite.setHitbox(sf::FloatRect(14.f, 0, 36.f, 64.f));
            this->eggRectSprite->top = 129;
            if (this->eggRectSprite->left >= 64*7) {
                this->eggRectSprite->left = 0;
            } else {
                this->eggRectSprite->left += 64;
            }
        }
    } else {
        this->eggSprite.setHitbox(sf::FloatRect(8.f, 0, 48.f, 64.f));
        this->eggRectSprite->top = 0;
        if (this->invincible) {
            this->eggRectSprite->left = 0;
        }
        if (this->textureTick == 1) {
            this->textureTick = 0;
        } else {
            if (this->eggRectSprite->left >= 64*3) {
                this->eggRectSprite->left = 0;
            } else {
                this->eggRectSprite->left += 64;
            }
            this->textureTick++;
        }
    }
    if (this->textureSwitch) {
        this->eggRectSprite->top = 0;
        this->eggRectSprite->left = 64*4;
    }
    this->eggSprite.setTextureRect(*this->eggRectSprite);
}

bool Player::damage(int damageAmount) {
    if (!this->invincible) {
        this->ySpeed = jumpSpeed;
        this->health -= damageAmount;
        if (this->health <= 0) {
            return false;
        }
        this->invincibleTimer = 0;
        this->invincible = true;
    }
    return true;
}

void Player::reset() {
    this->health = 100;
    this->movingDown = false;
    this->movingLeft = false;
    this->movingRight = false;
    this->movingUp = false;
}

HitBoxSprite Player::getSprite() {
    return this->eggSprite;
}