#include <SFML/Graphics.hpp>
#include <iostream>

class pauseMenu {
    public:
        pauseMenu();
        void init(float width, float height);
        void moveUp();
        void moveDown();
        void select();
        void draw(sf::RenderWindow *window);
        int update(sf::Vector2f center, float height);
        void restart();
        void escape();
    private:
        int currentItemIndex;
        int menuItems;
        sf::Font fontBulb;
        sf::Font fontClean;
        sf::Text text[5];
        bool startGame;
        bool returnMenu;
};