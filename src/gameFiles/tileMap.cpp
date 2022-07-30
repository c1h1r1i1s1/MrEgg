#include "tileMap.h"

bool tileMap::load(const std::string& tileset, sf::Vector2u tileSize, int* tiles, unsigned int width, unsigned int height, int scaleFactor) {
    if (!this->tileSet.loadFromFile(tileset)) {
        return false;
    }
    this->tileVertices.setPrimitiveType(sf::Quads);
    this->tileVertices.resize(width * height * 4);

    // populate the vertex array, with one quad per tile
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            // get the current tile number
            int tileNumber = tiles[i + j * width];

            // find its position in the tileset texture
            int tu = tileNumber % (this->tileSet.getSize().x / tileSize.x);
            int tv = tileNumber / (this->tileSet.getSize().x / tileSize.x);

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &this->tileVertices[(i + j * width) * 4];

            // define its 4 corners
            quad[0].position = sf::Vector2f(scaleFactor*i * tileSize.x, scaleFactor*j * tileSize.y);
            quad[1].position = sf::Vector2f(scaleFactor*(i + 1) * tileSize.x, scaleFactor*j * tileSize.y);
            quad[2].position = sf::Vector2f(scaleFactor*(i + 1) * tileSize.x, scaleFactor*(j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(scaleFactor*i * tileSize.x, scaleFactor*(j + 1) * tileSize.y);

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }

    return true;
}

void tileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &this->tileSet;

    // draw the vertex array
    target.draw(this->tileVertices, states);
}