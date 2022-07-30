#include "Menu.h"

Menu::Menu() {
}

void Menu::init(float width, float height) {
    this->menuItems = 3;
    this->currentItemIndex = 1;
    this->startGame = false;
    if (!this->fontBulb.loadFromFile("src/fonts/Bulb.TTF")) {
        std::cout << "cannot load font" << std::endl;
    }
    if (!this->fontClean.loadFromFile("src/fonts/Clean.ttf")) {
        std::cout << "cannot load font" << std::endl;
    }

    this->text[0].setFont(this->fontBulb);
    this->text[0].setFillColor(sf::Color::Yellow);
    this->text[0].setString("Mr Egg");
    this->text[0].setCharacterSize(192);
    this->text[0].setOrigin(this->text[0].getLocalBounds().width/2, this->text[0].getLocalBounds().height);
    this->text[0].setPosition(sf::Vector2f(width/2, height/4));

    this->text[1].setFont(this->fontClean);
    this->text[1].setFillColor(sf::Color::Black);
    this->text[1].setString("Play");
    this->text[1].setCharacterSize(96);
    this->text[1].setOrigin(this->text[1].getLocalBounds().width/2, this->text[1].getLocalBounds().height);
    this->text[1].setPosition(sf::Vector2f(width/2, 2*height/4));

    this->text[2].setFont(this->fontClean);
    this->text[2].setFillColor(sf::Color::Black);
    this->text[2].setString("Settings");
    this->text[2].setCharacterSize(96);
    this->text[2].setOrigin(this->text[2].getLocalBounds().width/2, this->text[2].getLocalBounds().height);
    this->text[2].setPosition(sf::Vector2f(width/2, 3*height/4));
}

bool Menu::update() {
    for (int i=0; i<this->menuItems; i++) {
        if (i == this->currentItemIndex) {
            this->text[i].setScale(1.5, 1.5);
        } else {
            this->text[i].setScale(1, 1);
        }
    }

    if (this->startGame) {
        this->startGame = false;
        return true;
    } else {
        return false;
    }
}

void Menu::moveUp() {
    this->currentItemIndex--;
    if (this->currentItemIndex == 0) {
        this->currentItemIndex = this->menuItems-1;
    }
}
void Menu::moveDown() {
    this->currentItemIndex++;
    if (this->currentItemIndex == this->menuItems) {
        this->currentItemIndex = 1;
    }
}
void Menu::select() {
    if (this->currentItemIndex == 1) {
        this->startGame = true;
    } else if (this->currentItemIndex == 2) {
        std::cout << "settings" << std::endl;
    }
}

void Menu::draw(sf::RenderWindow *window) {
    for (int i=0; i<this->menuItems; i++) {
        window->draw(this->text[i]);
    }
}