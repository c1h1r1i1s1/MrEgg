#include "HitBoxSprite.h"

void HitBoxSprite::setHitbox(const sf::FloatRect& hitbox) {
    this->hitbox = hitbox;
}

sf::FloatRect HitBoxSprite::getGlobalHitbox() const {
    return getTransform().transformRect(this->hitbox);
}