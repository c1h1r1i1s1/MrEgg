#include <SFML/Graphics.hpp>
#include <iostream>

void pause() {
    //Do stuff
    return;
}

void resume() {
    //Do stuff
    return;
}

int main()
{
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(3000, 1800), "Mr Egg");
    window.setVerticalSyncEnabled(true);

    bool movingRight = false;
    bool movingLeft = false;
    bool movingUp = false;
    bool movingDown = false;
    bool paused = false;
    bool jumping = false;


    sf::Texture eggTexture;
    if (!eggTexture.loadFromFile("src/Egg Animations 3.png")) {
        std::cout << "Didn't load image" << std::endl;
    }
    eggTexture.setSmooth(true);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("src/First level background.png")) {
        std::cout << "Didn't load background" << std::endl;
    }

    sf::Texture knifeTexture;
    if (!knifeTexture.loadFromFile("src/Knife.png")) {
        std::cout << "Didn't load knife" << std::endl;
    }

    sf::Sprite knifeSprite;
    knifeSprite.setTexture(knifeTexture);
    knifeSprite.setPosition(1000, 1600);
    knifeSprite.rotate(180);

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    sf::IntRect eggRectSprite(0, 64, 64, 64);
    sf::Sprite eggSprite(eggTexture, eggRectSprite);
    eggSprite.setOrigin(sf::Vector2f(32.f, 32.f));
    eggSprite.setScale(sf::Vector2f(3.f, 3.f));
    eggSprite.setPosition(sf::Vector2f(96.f, 96.f));

    // Physics work
    int gravityAccel = 3000;
    int moveAccel = 3000;
    int moveDeccel = 4000;
    int turnAccel = 6000;
    float xSpeed = 0;
    float ySpeed = 0;
    float maxRunSpeed = 600;
    sf::Clock physicsClock;
    sf::Time oldTime = physicsClock.getElapsedTime();
    sf::Time elapsed;
    sf::Time physicsTime;


    while (window.isOpen())
    {
        elapsed = clock.getElapsedTime();
        physicsTime = physicsClock.getElapsedTime();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::LostFocus) {
                pause();
                paused = true;
            } else if (event.type == sf::Event::GainedFocus) {
                resume();
                paused = false;
            }
            if (!paused) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::A) {
                        movingLeft = true;
                    } else if (event.key.code == sf::Keyboard::D) {
                        movingRight = true;
                    } else if (event.key.code == sf::Keyboard::W) {
                        movingUp = true;
                    } else if (event.key.code == sf::Keyboard::S) {
                        movingDown = true;
                    } else if (event.key.code == sf::Keyboard::Space) {
                        jumping = true;
                    }
                } else if (event.type == sf::Event::KeyReleased) {
                    if (event.key.code == sf::Keyboard::A) {
                        movingLeft = false;
                    } else if (event.key.code == sf::Keyboard::D) {
                        movingRight = false;
                    } else if (event.key.code == sf::Keyboard::W) {
                        movingUp = false;
                    } else if (event.key.code == sf::Keyboard::S) {
                        movingDown = false;
                    } else if (event.key.code == sf::Keyboard::Space) {
                        jumping = false;
                    }
                }
                
            }
        }

        float timeChangeMilli = (physicsTime.asMilliseconds() - oldTime.asMilliseconds())/1000.f;

        if (movingLeft && !movingRight) { // Moving Left
            float xChange;
            if (xSpeed > 0) {
                xChange = xSpeed * timeChangeMilli + 0.5 * -1 * turnAccel * timeChangeMilli * timeChangeMilli;
                xSpeed = xSpeed + -1 * turnAccel * timeChangeMilli;
            } else {
                xChange = xSpeed * timeChangeMilli + 0.5 * -1 * moveAccel * timeChangeMilli * timeChangeMilli;
                xSpeed = xSpeed + -1 * moveAccel * timeChangeMilli;
            }
            if (xSpeed < -1 * maxRunSpeed) {
                xSpeed = -1 * maxRunSpeed;
            }
            eggSprite.move(xChange, 0);
            eggRectSprite.top = 129;
            if (elapsed.asMilliseconds() > 70) {
                if (eggRectSprite.left >= 64*7) {
                    eggRectSprite.left = 0;
                } else {
                    eggRectSprite.left += 64;
                }
                clock.restart();
                eggSprite.setTextureRect(eggRectSprite);
            }
        } else if (movingRight && !movingLeft) { // Moving Right
            float xChange;
            if (xSpeed < 0) {
                xChange = xSpeed * timeChangeMilli + 0.5 * turnAccel * timeChangeMilli * timeChangeMilli;
                xSpeed = xSpeed + turnAccel * timeChangeMilli;
            } else {
                xChange = xSpeed * timeChangeMilli + 0.5 * moveAccel * timeChangeMilli * timeChangeMilli;
                xSpeed = xSpeed + moveAccel * timeChangeMilli;
            }
            if (xSpeed > maxRunSpeed) {
                xSpeed = maxRunSpeed;
            }
            eggSprite.move(xChange, 0);
            eggRectSprite.top = 65;
            if (elapsed.asMilliseconds() > 70) {
                if (eggRectSprite.left >= 64*7) {
                    eggRectSprite.left = 0;
                } else {
                    eggRectSprite.left += 64;
                }
                clock.restart();
                eggSprite.setTextureRect(eggRectSprite);
            }
        } else { // Still
            if (xSpeed > 0) { // If moving right, force accel left
                float xChange = xSpeed * timeChangeMilli + 0.5 * -1 * moveDeccel * timeChangeMilli * timeChangeMilli;
                xSpeed = xSpeed + -1 * moveDeccel * timeChangeMilli;
                if (xSpeed > 0) {
                    eggSprite.move(xChange, 0);
                    eggRectSprite.top = 65;
                    if (elapsed.asMilliseconds() > 70) {
                        if (eggRectSprite.left >= 64*7) {
                            eggRectSprite.left = 0;
                        } else {
                            eggRectSprite.left += 64;
                        }
                        clock.restart();
                        eggSprite.setTextureRect(eggRectSprite);
                    }
                } else {
                    xSpeed = 0;
                    eggRectSprite.top = 0;
                    if (elapsed.asMilliseconds() > 100) {
                        if (eggRectSprite.left >= 64*3) {
                            eggRectSprite.left = 0;
                        } else {
                            eggRectSprite.left += 64;
                        }
                        clock.restart();
                        eggSprite.setTextureRect(eggRectSprite);
                    }
                }
            } else if (xSpeed < 0) { // If moving left, force accel right
                float xChange = xSpeed * timeChangeMilli + 0.5 * moveDeccel * timeChangeMilli * timeChangeMilli;
                xSpeed = xSpeed + moveDeccel * timeChangeMilli;
                if (xSpeed < 0) {
                    eggSprite.move(xChange, 0);
                    eggRectSprite.top = 129;
                    if (elapsed.asMilliseconds() > 70) {
                        if (eggRectSprite.left >= 64*7) {
                            eggRectSprite.left = 0;
                        } else {
                            eggRectSprite.left += 64;
                        }
                        clock.restart();
                        eggSprite.setTextureRect(eggRectSprite);
                    }
                } else {
                    xSpeed = 0;
                    eggRectSprite.top = 0;
                    if (elapsed.asMilliseconds() > 100) {
                        if (eggRectSprite.left >= 64*3) {
                            eggRectSprite.left = 0;
                        } else {
                            eggRectSprite.left += 64;
                        }
                        clock.restart();
                        eggSprite.setTextureRect(eggRectSprite);
                    }
                }
            } else {
                eggRectSprite.top = 0;
                if (elapsed.asMilliseconds() > 100) {
                    if (eggRectSprite.left >= 64*3) {
                        eggRectSprite.left = 0;
                    } else {
                        eggRectSprite.left += 64;
                    }
                    clock.restart();
                    eggSprite.setTextureRect(eggRectSprite);
                }
            }
        }

        // if (movingUp && !movingDown) {
        //     eggSprite.move(0, -6);
        // } else if (movingDown && !movingUp) {
        //     eggSprite.move(0, 6);
        // }
        
        float yChange = ySpeed * timeChangeMilli + 0.5 * gravityAccel * timeChangeMilli * timeChangeMilli;
        if (eggSprite.getPosition().y + yChange < (window.getSize().y - eggSprite.getGlobalBounds().height/2)) {
            eggSprite.move(0, yChange);
            ySpeed = ySpeed + gravityAccel * timeChangeMilli;
        } else {
            eggSprite.setPosition(eggSprite.getPosition().x, (window.getSize().y - eggSprite.getGlobalBounds().height/2));
            ySpeed = 0;
        }

        if (jumping) {
            jumping = false;
            ySpeed = -1200;
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(knifeSprite);
        if (!eggSprite.getGlobalBounds().intersects(knifeSprite.getGlobalBounds())) {
            window.draw(eggSprite);
        }
        window.display();

        oldTime = physicsTime;
        
    }

    return 0;
}