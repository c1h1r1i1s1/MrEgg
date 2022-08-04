#include "Game.h"

Game::Game() {
    this->clock.restart();
    this->initVariables();
    this->initWindow();
}

Game::~Game() {
    delete this->window;
}

void Game::loadMenu() {
    this->currentState = inMenu;
    this->menu.init(this->windowView->getSize().x, this->windowView->getSize().y);
    // Init menus
    this->pMenu.init(this->windowView->getSize().x, this->windowView->getSize().y);
    this->fMenu.init(this->windowView->getSize().x, this->windowView->getSize().y);
}

void Game::initVariables() {
    this->window = nullptr;
    this->gravityAccel = 4000;
    this->timeChangeMilli = 0;
    this->oldTime = this->clock.getElapsedTime();
    this->frameCounter = oldTime;
    this->levelObjects = new std::vector<gameObject>;
    this->finishTimer = -1;
    this->finishSwitch = false;

    // Sound
    if (!this->frySoundBuffer.loadFromFile("src/audio/Frying.wav")) {
        std::cout << "Cannot load frying audio" << std::endl;
    }
    this->frySound.setBuffer(this->frySoundBuffer);
    this->frySound.setVolume(50);
    this->frySound.setID(0);

    if (!this->fryLoopSoundBuffer.loadFromFile("src/audio/FryingLoop.wav")) {
        std::cout << "Cannot load frying loop audio" << std::endl;
    }
    this->fryLoopSound.setBuffer(this->fryLoopSoundBuffer);
    this->fryLoopSound.setVolume(10);
    this->fryLoopSound.setLoop(true);
    this->fryLoopSound.setID(1);

    if (!this->inGameMusic.openFromFile("src/audio/Main Theme.wav")) {
        std::cout << "Cannot load main theme music" << std::endl;
    }
    this->inGameMusic.setVolume(50);
    this->inGameMusic.setLoop(true);
}

void Game::initWindow() {
    this->window = new sf::RenderWindow(sf::VideoMode(sf::VideoMode::getDesktopMode().height, 2*sf::VideoMode::getDesktopMode().height/3), "Mr Egg");
    this->window->setVerticalSyncEnabled(true);
    this->windowView = new sf::View(sf::FloatRect(0.f, 0.f, 2400.f, 1600.f));
    this->window->setView(*this->windowView);
    this->xScreenRatio = this->windowView->getSize().x/this->window->getSize().x;
    this->yScreenRatio = this->windowView->getSize().x/this->window->getSize().x;
}

void Game::initPlayer() {
    this->player = Player();
}

void Game::loadLevel(int levelNum) {
    if (!this->levelObjects->empty()) {
        this->levelObjects->clear();
    }
    this->finishTimer = 0;

    std::ifstream levelDataFile("src/levelData/level_" + std::to_string(levelNum) + ".data");
    std::string lineText;
    getline(levelDataFile, lineText); // Don't worry about first line
    
    // Get main info first
    getline(levelDataFile, lineText);
    int splitPoint = lineText.find("|");
    int width = std::stoi(lineText.substr(0, splitPoint));
    int height = std::stoi(lineText.substr(splitPoint+1, std::string::npos));
    this->levelRect = new sf::FloatRect(0.f, 0.f, width, height);
    
    getline(levelDataFile, lineText);
    this->objCount = std::stoi(lineText);

    getline(levelDataFile, lineText);
    int x = std::stoi(lineText);

    getline(levelDataFile, lineText);
    this->player.setLocation(x, std::stoi(lineText));

    getline(levelDataFile, lineText); // Skipping header

    // Defining variables to pass into constructor as read from file
    std::string textureFile;
    bool active;
    float xPos;
    float yPos;
    float hitBoxL;
    float hitBoxT;
    float hitBoxW;
    float hitBoxH;
    sf::FloatRect* hitBox;
    float rotation;
    int animationCycle;
    bool sharp;
    bool textured;
    bool moving;
    int moveType;
    float moveOffset;
    bool finisher;
    float boxScale;
    float scale;

    int lineCounter = 0; // out of 14
    std::string segment = "";
    while (1) { // Get all objects
        getline(levelDataFile, lineText);
        if (lineText[0] == *"/" && lineText[1] == *"/") {
            break;
        } else {
            lineCounter = 0;
            for (auto &ch : lineText) {
                if (ch == *"|") {
                    switch (lineCounter) {
                        case 0:
                            textureFile = segment;
                            break;
                        case 1:
                            if (segment.compare("true") == 0) {
                                active = true;
                            } else {
                                active = false;
                            }
                            break;
                        case 2:
                            xPos = std::stof(segment);
                            break;
                        case 3:
                            yPos = std::stof(segment);
                            break;
                        case 4:
                            hitBoxL = std::stof(segment);
                            break;
                        case 5:
                            hitBoxT = std::stof(segment);
                            break;
                        case 6:
                            hitBoxW = std::stof(segment);
                            break;
                        case 7:
                            hitBoxH = std::stof(segment);
                            hitBox = new sf::FloatRect(hitBoxL, hitBoxT, hitBoxW, hitBoxH);
                            break;
                        case 8:
                            rotation = std::stof(segment);
                            break;
                        case 9:
                            animationCycle = std::stoi(segment);
                            break;
                        case 10:
                            if (segment.compare("true") == 0) {
                                sharp = true;
                            } else {
                                sharp = false;
                            }
                            break;
                        case 11:
                            if (segment.compare("true") == 0) {
                                textured = true;
                            } else {
                                textured = false;
                            }
                            break;
                        case 12:
                            if (segment.compare("true") == 0) {
                                moving = true;
                            } else {
                                moving = false;
                            }
                            break;
                        case 13:
                            moveType = std::stoi(segment);
                            break;
                        case 14:
                            moveOffset = std::stof(segment);
                            break;
                        case 15:
                            if (segment.compare("true") == 0) {
                                finisher = true;
                            } else {
                                finisher = false;
                            }
                            break;
                        case 16:
                            boxScale = std::stof(segment);
                            break;
                        case 17:
                            scale = std::stof(segment);
                            break;
                    }
                    segment = "";
                    lineCounter++;
                } else {
                    segment += ch;
                }
            }
            this->levelObjects->push_back(gameObject(textureFile, active, xPos, yPos, *hitBox, rotation, animationCycle, sharp, textured, moving, moveType, moveOffset, finisher, boxScale, scale));
        }
    }

    // Get tileset dimensions, then data
    getline(levelDataFile, lineText);
    segment = lineText[0];
    int scaleFactor = std::stoi(segment);
    int *levelOld = new int[((width/64)/scaleFactor)*((height/64)/scaleFactor)];
    int counter = 0;
    for (int i=0; i<(height/64)/scaleFactor; i++) {
        getline(levelDataFile, lineText);
        for (auto &ch : lineText) {
            if (ch != *"," && ch != *" ") {
                if (ch == *"0") {
                    levelOld[counter++] = 0; //NEED TO FIGURE OUT CHAR TO INT properly?
                } else if (ch == *"1") {
                   levelOld[counter++] = 1; //NEED TO FIGURE OUT CHAR TO INT properly?
                }
            }
        }
    }

    // Create bounding boxes
    std::string thing = "";
    this->levelObjects->push_back(gameObject(thing, false, -50, -50, sf::FloatRect(0, 0, 50, this->levelRect->height + 100), 0, 0, false, false, false, 0, 0, false, 1, 1));
    this->levelObjects->push_back(gameObject(thing, false, 0, -50, sf::FloatRect(0, 0, this->levelRect->width, 50), 0, 0, false, false, false, 0, 0, false, 1, 1));
    this->levelObjects->push_back(gameObject(thing, false, this->levelRect->width, -50, sf::FloatRect(0, 0, 50, this->levelRect->height + 100), 0, 0, false, false, false, 0, 0, false, 1, 1));
    this->levelObjects->push_back(gameObject(thing, false, 0, this->levelRect->height, sf::FloatRect(0, 0, this->levelRect->width, 50), 0, 0, false, false, false, 0, 0, false, 1, 1));
    
    // Load Tileset
    if (!this->map.load("src/Images/main tiles practice.png", sf::Vector2u(64, 64), levelOld, (width/64)/scaleFactor, (height/64)/scaleFactor, 2)) {
        std::cout << "couldn't load tileset" << std::endl;
    }
}

void Game::loadObjects() {
    this->player.loadTexture();
    this->player.loadSprite();

    this->loadLevel(1);
}

bool Game::running() {
    return this->window->isOpen();
}

void Game::pollEvent() {
    while (this->window->pollEvent(this->event)) {
        if (this->event.type == sf::Event::Closed) {
            this->window->close();
            break;
        } else if (this->event.type == sf::Event::LostFocus) {
            if (this->currentState == inGame) {
                sf::Vector2u windowSize = this->window->getSize();
                this->pauseTexture.create(windowSize.x, windowSize.y);
                this->pauseTexture.update(*this->window);
                this->pauseSprite.setTexture(this->pauseTexture);
                this->pauseSprite.setColor(sf::Color(255, 255, 255, 200));
                this->pauseSprite.setOrigin(this->pauseSprite.getLocalBounds().width/2, this->pauseSprite.getLocalBounds().height/2);
                this->pauseSprite.setPosition(this->windowView->getCenter().x, this->windowView->getCenter().y);
                this->pauseSprite.setScale(this->xScreenRatio, this->yScreenRatio);
                this->currentState = paused;
                this->stateSwitch(2);
                this->pMenu.restart();
            }
        } else if (event.type == sf::Event::Resized) {
            // update the view to the new size of the window
            // Use standard ratio of pixels per screen pixel to scale
            sf::Vector2f oldPos = this->player.getLocation();
            this->windowView->setSize(event.size.width*this->xScreenRatio, event.size.height*this->yScreenRatio);
            this->window->setView(*this->windowView);
            this->player.setLocation(oldPos.x, oldPos.y);
        }
        if (this->currentState == inGame) {
            if (event.type == sf::Event::KeyPressed) {
                if (this->event.key.code == sf::Keyboard::A) {
                    this->player.movingLeft = true;
                } else if (this->event.key.code == sf::Keyboard::D) {
                    this->player.movingRight = true;
                } else if (this->event.key.code == sf::Keyboard::W) {
                    this->player.movingUp = true;
                } else if (this->event.key.code == sf::Keyboard::S) {
                    this->player.movingDown = true;
                } else if (this->event.key.code == sf::Keyboard::Space) {
                    this->player.jumping = true;
                } else if (this->event.key.code == sf::Keyboard::R) {
                    this->restartLevel();
                } else if (this->event.key.code == sf::Keyboard::Escape) {
                    sf::Vector2u windowSize = this->window->getSize();
                    this->pauseTexture.create(windowSize.x, windowSize.y);
                    this->pauseTexture.update(*this->window);
                    this->pauseSprite.setTexture(this->pauseTexture);
                    this->pauseSprite.setColor(sf::Color(255, 255, 255, 200));
                    this->pauseSprite.setOrigin(this->pauseSprite.getLocalBounds().width/2, this->pauseSprite.getLocalBounds().height/2);
                    this->pauseSprite.setPosition(this->windowView->getCenter().x, this->windowView->getCenter().y);
                    this->pauseSprite.setScale(this->xScreenRatio, this->yScreenRatio);
                    this->currentState = paused;
                    this->stateSwitch(2);
                    this->pMenu.restart();
                }
            } else if (this->event.type == sf::Event::KeyReleased) {
                if (this->event.key.code == sf::Keyboard::A) {
                    this->player.movingLeft = false;
                } else if (this->event.key.code == sf::Keyboard::D) {
                    this->player.movingRight = false;
                } else if (this->event.key.code == sf::Keyboard::W) {
                    this->player.movingUp = false;
                } else if (this->event.key.code == sf::Keyboard::S) {
                    this->player.movingDown = false;
                } else if (this->event.key.code == sf::Keyboard::Space) {
                    this->player.jumping = false;
                }
            }
        } else if (this->currentState == inMenu) {
            if (event.type == sf::Event::KeyPressed) {
                if (this->event.key.code == sf::Keyboard::W) {
                    this->menu.moveUp();
                } else if (this->event.key.code == sf::Keyboard::S) {
                    this->menu.moveDown();
                } else if (this->event.key.code == sf::Keyboard::Enter) {
                    this->menu.select();
                }
            }
        } else if (this->currentState == paused) {
            if (event.type == sf::Event::KeyPressed) {
                if (this->event.key.code == sf::Keyboard::W) {
                    this->pMenu.moveUp();
                } else if (this->event.key.code == sf::Keyboard::S) {
                    this->pMenu.moveDown();
                } else if (this->event.key.code == sf::Keyboard::Enter) {
                    this->pMenu.select();
                } else if (this->event.key.code == sf::Keyboard::Escape) {
                    this->pMenu.escape();
                }
            }
        } else if (this->currentState == won) {
            if (event.type == sf::Event::KeyPressed) {
                if (this->event.key.code == sf::Keyboard::W) {
                    this->fMenu.moveUp();
                } else if (this->event.key.code == sf::Keyboard::S) {
                    this->fMenu.moveDown();
                } else if (this->event.key.code == sf::Keyboard::Enter) {
                    this->fMenu.select();
                }
            }
        }
    }
}

void Game::update() {
    this->pollEvent();
    if (this->currentState == inGame || this->currentState == won) {
        //Tick movements
        this->elapsed = this->clock.getElapsedTime();
        this->timeChangeMilli = (elapsed.asMilliseconds() - oldTime.asMilliseconds())/1000.f;
        if (this->currentState == inGame) {
            this->player.calcMovement(timeChangeMilli);
        }
        for (int i=0; i<this->objCount; i++) {
            this->levelObjects->at(i).calcMovement(this->elapsed.asSeconds());
        }

        // Update sprite images
        float frameDifference = this->elapsed.asMilliseconds() - this->frameCounter.asMilliseconds();
        if (frameDifference > 70) {
            this->player.updateTexture();
            for (int i=0; i<this->objCount; i++) {
                if (this->levelObjects->at(i).active) {
                    this->levelObjects->at(i).updateTexture();
                }
            }
            this->frameCounter = this->elapsed;
            // Also update other sprites
        }

        this->player.affectGravity(this->timeChangeMilli);

        // Check collisions here
        bool colliding = false;
        for (int i=0; i<this->objCount; i++) {
            if (this->player.getSprite().getGlobalHitbox().intersects(this->levelObjects->at(i).getSprite().getGlobalHitbox())) {
                colliding = true;
                sf::FloatRect tempObjBox = this->levelObjects->at(i).getSprite().getGlobalHitbox();
                sf::FloatRect tempPlayerBox = this->player.getSprite().getGlobalHitbox();
                if (this->levelObjects->at(i).sharp) {
                    if (!this->player.invincible) {
                        if (!this->player.damage(50)) {
                            this->restartLevel();
                            this->player.reset();
                        }
                    }
                } else if (this->levelObjects->at(i).finisher) {
                    this->levelObjects->at(i).landing = true;
                    if (this->currentState != won) {
                        this->currentState = won;
                        this->stateSwitch(0);
                        this->finishTimer = this->clock.getElapsedTime().asMilliseconds();
                    }
                } else {
                    // From perspective of player, take minimum of overlaps 
                    float overlapL = tempObjBox.left + tempObjBox.width - tempPlayerBox.left;
                    float overlapR = tempPlayerBox.left + tempPlayerBox.width - tempObjBox.left;
                    float overlapT = tempObjBox.top + tempObjBox.height - tempPlayerBox.top;
                    float overlapB = tempPlayerBox.top + tempPlayerBox.height - tempObjBox.top;

                    float minVal = overlapB;
                    if (overlapT < overlapB) {
                        minVal = overlapT;
                    }
                    if (overlapL < minVal) {
                        minVal = overlapL;
                    }
                    if (overlapR < minVal) {
                        minVal = overlapR;
                    }
                    if (minVal == overlapL) {
                        this->player.setXSpeed(0);
                        this->player.movePlayer(tempObjBox.left + tempObjBox.width - tempPlayerBox.left, 0);
                    } else if (minVal == overlapR) {
                        this->player.setXSpeed(0);
                        this->player.movePlayer(-1*(tempPlayerBox.left + tempPlayerBox.width - tempObjBox.left), 0);
                    } else if (minVal == overlapT) {
                        this->player.setYSpeed(0);
                        this->player.movePlayer(0, tempObjBox.top + tempObjBox.height - tempPlayerBox.top);
                    } else {
                        this->player.setYSpeed(0);
                        this->player.movePlayer(0, -1*(tempPlayerBox.top + tempPlayerBox.height - tempObjBox.top));
                        this->player.inAir = false;
                        this->player.justJumped = false;
                        this->player.boostUsed = false;
                        this->player.doubleJumpUsed = false;
                    }
                }
            }
        }
        if (!colliding) {
            this->player.inAir = true;
        }
        
        // Update time changes
        this->oldTime = this->elapsed;

        // Update window view
        if (this->player.getLocation().x < this->windowView->getCenter().x - this->windowView->getSize().x/4) { // If hes running in the quarter left
            this->windowView->move(-1*(this->windowView->getCenter().x - this->windowView->getSize().x/4 - this->player.getLocation().x)/2, 0);
        } else if (this->player.getLocation().x > this->windowView->getCenter().x + this->windowView->getSize().x/4) { // If hes running in the quarter right
            this->windowView->move((this->player.getLocation().x - this->windowView->getCenter().x - (this->windowView->getSize().x/4))/2, 0);
        }
        if (this->player.getLocation().y < this->windowView->getCenter().y - this->windowView->getSize().y/4) { // If hes running in the quarter top
            this->windowView->move(0, -1*(this->windowView->getCenter().y - this->windowView->getSize().y/4 - this->player.getLocation().y)/2);
        } else if (this->player.getLocation().y > this->windowView->getCenter().y + 3*this->windowView->getSize().y/8) { // If hes running in the 10th bottom
            this->windowView->move(0, (this->player.getLocation().y - this->windowView->getCenter().y - ((3*this->windowView->getSize().y)/8))/2);
        }
        // Make sure we don't view OOB areas
        if (this->windowView->getCenter().x - this->windowView->getSize().x/2 < 0) {
            this->windowView->setCenter(this->windowView->getSize().x/2, this->windowView->getCenter().y);
        } else if (this->windowView->getCenter().x + this->windowView->getSize().x/2 > this->levelRect->width) {
            this->windowView->setCenter(this->levelRect->width - this->windowView->getSize().x/2, this->windowView->getCenter().y);
        }
        if (this->windowView->getCenter().y - this->windowView->getSize().y/2 < 0) {
            this->windowView->setCenter(this->windowView->getCenter().x, this->windowView->getSize().y/2);
        } else if (this->windowView->getCenter().y + this->windowView->getSize().y/2 > this->levelRect->height) {
            this->windowView->setCenter(this->windowView->getCenter().x,this->levelRect->height - this->windowView->getSize().y/2);
        }
        this->window->setView(*this->windowView);
        
        float timeDiff = this->clock.getElapsedTime().asMilliseconds() - this->finishTimer;
        if (this->currentState == won) {
            if (timeDiff >= 1000) {
                int ret = this->fMenu.update(this->windowView->getCenter(), this->windowView->getSize().y);
                if (ret == 1) {
                    this->currentState = inGame;
                    this->stateSwitch(0);
                    this->loadLevel(1);
                    this->restartLevel();
                } else if (ret == 2) {
                    this->windowView->setCenter(this->windowView->getSize().x/2, this->windowView->getSize().y/2);
                    this->window->setView(*this->windowView);
                    this->currentState = inMenu;
                    this->stateSwitch(0);
            }
        }
        }

    } else if (this->currentState == inMenu) {
        if (this->menu.update()) {
            this->currentState = inGame;
            this->stateSwitch(0);
            this->initPlayer();
            this->loadObjects();
        }
    } else if (this->currentState == paused) {
        int ret = this->pMenu.update(this->windowView->getCenter(), this->windowView->getSize().y);
        if (ret == 1) {
            this->currentState = inGame;
            this->stateSwitch(1);
        } else if (ret == 2) {
            this->windowView->setCenter(this->windowView->getSize().x/2, this->windowView->getSize().y/2);
            this->window->setView(*this->windowView);
            this->currentState = inMenu;
            this->stateSwitch(0);
        }
    }
    if (this->soundList.size() > 0) {
        this->updateSounds();
    }
}

void Game::updateSounds() {
    int i=-1;
    int removeSize = this->soundList.size();
    int removeList[removeSize];
    for (Sound2 *sound : this->soundList) {
        i++;
        if (sound->getStatus() == sf::SoundSource::Status::Stopped) {
            if (sound->getID() == 0) {
                if (this->currentState == won) {
                    this->fryLoopSound.play();
                    this->soundList.push_back(&this->fryLoopSound);
                }
            }
            removeList[i] = 1;
        }
    }
    for (int y=removeSize-1; y>=0; y--) {
        if (removeList[y] == 1) {
            this->soundList.erase(this->soundList.begin() + y);
        }
    }
}

void Game::render() {
    this->window->clear(sf::Color(255, 255, 255, 255));

    if (this->currentState == inGame) {
        this->window->draw(this->map);
        for (int i=0; i<this->objCount; i++) {
            this->window->draw(this->levelObjects->at(i).getSprite());
        }
        this->window->draw(this->player.getSprite());
    } else if (this->currentState == inMenu) {
        this->menu.draw(this->window);
    } else if (this->currentState == paused) {
        this->window->draw(this->pauseSprite);
        this->pMenu.draw(this->window);
    } else if (this->currentState == won) {
        if (!(this->finishSwitch)) {
            for (int i=0; i<this->objCount; i++) {
                if (this->levelObjects->at(i).finisher) {
                    this->levelObjects->at(i).updateTexture();
                }
            }
            this->finishSwitch = true;
            this->window->draw(this->player.getSprite());
        }
        this->window->draw(this->map);
        for (int i=0; i<this->objCount; i++) {
            this->window->draw(this->levelObjects->at(i).getSprite());
        }
        this->fMenu.draw(this->window);
    }

    this->window->display();
}

void Game::stateSwitch(int switchType) {
    if (switchType == 0) { // Full screen switch
        if (this->soundList.size() > 0) {
            for (Sound2 *sound : this->soundList) {
                sound->stop();
            }
        }
        if (this->currentState == inMenu) {
            this->inGameMusic.stop();
            // play menu music
        } else if (this->currentState == inGame) {
            this->inGameMusic.play();
        } else if (this->currentState == won) {
            this->frySound.play();
            this->soundList.push_back(&this->frySound);
        }
    } else if (switchType == 1) { // Resuming from pause
        //this->pauseMusic.stop();
        if (this->soundList.size() > 0) {
            for (Sound2 *sound : this->soundList) {
                sound->play();
            }
        }
        this->inGameMusic.play();
    } else if (switchType == 2) { // Starting pause
        if (this->soundList.size() > 0) {
            for (Sound2 *sound : this->soundList) {
                sound->pause();
            }
        }
        this->inGameMusic.pause();
        //this->pauseMusic.play();
    }
}

void Game::restartLevel() {
    this->player.reset();
    this->player.setLocation(7000, 200);
    this->finishTimer = -1;
}