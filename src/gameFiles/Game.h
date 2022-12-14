#include <SFML/Graphics.hpp>
#include "Player.h"
#include <iostream>
#include <fstream>
#include "gameObject.h"
#include <vector>
#include "tileMap.h"
#include "Menu.h"
#include "pauseMenu.h"
#include "finishMenu.h"
#include <SFML/Audio.hpp>
#include "Sound2.h"

class Game {
    private:
        // Big things
        sf::Clock clock;
        sf::RenderWindow* window;
        sf::View* windowView;
        sf::FloatRect* levelRect;
        sf::Event event;
        Player player;
        bool levelInit;
        float xScreenRatio;
        float yScreenRatio;
        tileMap map;
        enum gameState { inMenu, inGame, won, paused };
        gameState currentState;
        Menu menu;
        pauseMenu pMenu;
        finishMenu fMenu;

        // Textures
        sf::Texture eggTexture;
        sf::Texture backgroundTexture;
        sf::Texture knifeTexture;
        sf::Image pauseImage;
        sf::Texture pauseTexture;
        sf::Sprite pauseSprite;

        // Sprites
        //sf::Sprite knifeSprite;
        HitBoxSprite knifeSprite;
        sf::Sprite backgroundSprite;

        int objCount;
        std::vector<gameObject>* levelObjects;

        // Physics
        int gravityAccel;
        sf::Time elapsed;
        sf::Time oldTime;
        sf::Time frameCounter;
        float timeChangeMilli;

        // Sounds
        sf::SoundBuffer frySoundBuffer;
        sf::SoundBuffer fryLoopSoundBuffer;
        Sound2 frySound;
        Sound2 fryLoopSound;
        sf::Music inGameMusic;
        std::vector<Sound2*> soundList;
        
        float finishTimer;
        bool finishSwitch;

        void initVariables();
        void initWindow();
        void restartLevel();
        void loadLevel(int levelNum);
        void stateSwitch(int switchType);
        void updateSounds();
    
    public:
        Game();
        virtual ~Game();
        void update();
        void render();
        bool running();
        void pollEvent();
        void initPlayer();
        void loadObjects();
        void loadMenu();
};