#include <SFML/Graphics.hpp>
#include <iostream>

class Menu {
    public:
        Menu();
        void init(float width, float height);
        void moveUp();
        void moveDown();
        void select();
        void draw(sf::RenderWindow *window);
        bool update();
    private:
        int currentItemIndex;
        int menuItems;
        sf::Font fontBulb;
        sf::Font fontClean;
        sf::Text text[5];
        bool startGame;
};