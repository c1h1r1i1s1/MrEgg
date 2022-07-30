#include "pauseMenu.h"

pauseMenu::pauseMenu() {
}

void pauseMenu::init(float width, float height) {
    this->menuItems = 3;
    this->currentItemIndex = 0;
    this->startGame = false;
    this->returnMenu = false;
    if (!this->fontBulb.loadFromFile("src/fonts/Bulb.TTF")) {
        std::cout << "cannot load font" << std::endl;
    }
    if (!this->fontClean.loadFromFile("src/fonts/Clean.ttf")) {
        std::cout << "cannot load font" << std::endl;
    }

    this->text[0].setFont(this->fontClean);
    this->text[0].setFillColor(sf::Color::Yellow);
    this->text[0].setString("Resume");
    this->text[0].setCharacterSize(96);
    this->text[0].setOrigin(this->text[0].getLocalBounds().width/2, this->text[0].getLocalBounds().height);
    this->text[0].setPosition(sf::Vector2f(width/2, height/4));

    this->text[1].setFont(this->fontClean);
    this->text[1].setFillColor(sf::Color::Black);
    this->text[1].setString("Settings");
    this->text[1].setCharacterSize(96);
    this->text[1].setOrigin(this->text[1].getLocalBounds().width/2, this->text[1].getLocalBounds().height);
    this->text[1].setPosition(sf::Vector2f(width/2, 2*height/4));

    this->text[2].setFont(this->fontClean);
    this->text[2].setFillColor(sf::Color::Black);
    this->text[2].setString("Menu");
    this->text[2].setCharacterSize(96);
    this->text[2].setOrigin(this->text[2].getLocalBounds().width/2, this->text[2].getLocalBounds().height);
    this->text[2].setPosition(sf::Vector2f(width/2, 3*height/4));
}

void pauseMenu::restart() {
    this->currentItemIndex = 0;
    this->startGame = false;
    this->returnMenu = false;
}

void pauseMenu::escape() {
    this->startGame = true;
}

int pauseMenu::update(sf::Vector2f center, float height) {
    this->text[0].setPosition(sf::Vector2f(center.x, center.y-height/3));
    this->text[1].setPosition(center);
    this->text[2].setPosition(sf::Vector2f(center.x, center.y+height/3));
    for (int i=0; i<this->menuItems; i++) {
        if (i == this->currentItemIndex) {
            this->text[i].setScale(1.5, 1.5);
        } else {
            this->text[i].setScale(1, 1);
        }
    }

    if (this->startGame) {
        this->startGame = false;
        return 1;
    } else if (this->returnMenu) {
        this->returnMenu = false;
        return 2;
    } else {
        return 0;
    }
}

void pauseMenu::moveUp() {
    this->currentItemIndex--;
    if (this->currentItemIndex < 0) {
        this->currentItemIndex = this->menuItems-1;
    }
}
void pauseMenu::moveDown() {
    this->currentItemIndex++;
    if (this->currentItemIndex == this->menuItems) {
        this->currentItemIndex = 0;
    }
}
void pauseMenu::select() {
    switch (this->currentItemIndex) {
        case 0:
            this->startGame = true;
            break;
        case 1:
            std::cout << "settings" << std::endl;
            break;
        case 2:
            this->returnMenu = true;
            break;
    }
}

void pauseMenu::draw(sf::RenderWindow *window) {
    for (int i=0; i<this->menuItems; i++) {
        window->draw(this->text[i]);
    }
}