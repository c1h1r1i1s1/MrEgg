#include <SFML/Audio.hpp>

class Sound2 : public sf::Sound {
    public:
        int soundID;

        void setID(int ID);
        int getID();
};