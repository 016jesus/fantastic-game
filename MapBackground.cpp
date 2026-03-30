#include "MapBackground.h"

void MapBackground::setColor(sf::Color color) {
    background.setFillColor(color);
}

bool MapBackground::loadFromFile(const std::string& path) {
    if (!bgTexture.loadFromFile(path)) {
        return false;
    }
    hasTexture = true;
    // Scale the texture to fill the rectangle exactly.
    const sf::Vector2u texSize = bgTexture.getSize();
    const sf::Vector2f bgSize  = background.getSize();

    if (texSize.x > 0 && texSize.y > 0) {
        background.setTexture(&bgTexture);
        // Use a texture rect that matches the rectangle's current size so the
        // image fills the shape without repeating (unless the user enables it).
        background.setTextureRect(sf::IntRect(
            0, 0,
            static_cast<int>(texSize.x),
            static_cast<int>(texSize.y)));
    }
    return true;
}

void MapBackground::draw(sf::RenderWindow& window) {
    window.draw(background);
}

void MapBackground::setSize(float width, float height) {
    background.setSize(sf::Vector2f(width, height));
    // If a texture is already loaded, re-apply so the rect stays consistent.
    if (hasTexture) {
        const sf::Vector2u texSize = bgTexture.getSize();
        background.setTextureRect(sf::IntRect(
            0, 0,
            static_cast<int>(texSize.x),
            static_cast<int>(texSize.y)));
    }
}
