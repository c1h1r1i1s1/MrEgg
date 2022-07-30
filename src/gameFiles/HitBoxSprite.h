#pragma once

#include <SFML/Graphics.hpp>

class HitBoxSprite : public sf::Sprite {
    private:
        sf::FloatRect hitbox;
    public:
        void setHitbox(const sf::FloatRect& hitbox);
        sf::FloatRect getGlobalHitbox() const;
    
};