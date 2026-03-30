#pragma once
#ifndef MAP_BACKGROUND_H
#define MAP_BACKGROUND_H
#include <SFML/Graphics.hpp>
#include <string>

class MapBackground {
public:
    MapBackground() = default;

    // Sets a solid fill colour when no texture is in use.
    void setColor(sf::Color color);

    // Loads a background image and tiles/scales it to fill the current size.
    // Returns true on success, false if the file could not be opened.
    bool loadFromFile(const std::string& path);

    // Draws the background rectangle (with or without texture) to the window.
    void draw(sf::RenderWindow& window);

    // Resizes the background rectangle. Call before loadFromFile / setColor
    // so the texture is fitted correctly.
    void setSize(float width, float height);

private:
    sf::RectangleShape background;
    sf::Texture bgTexture;
    bool hasTexture = false;
};
#endif
