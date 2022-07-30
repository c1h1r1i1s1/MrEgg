#include <SFML/Graphics.hpp>

class tileMap : public sf::Drawable, public sf::Transformable {

    private:
        sf::Texture tileSet;
        sf::VertexArray tileVertices;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
    public:
        bool load(const std::string& tileset, sf::Vector2u tileSize, int* tiles, unsigned int width, unsigned int height, int scaleFactor);

};