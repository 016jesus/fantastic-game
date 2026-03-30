#pragma once
#ifndef SECCION_HUD_H
#define SECCION_HUD_H
#include <SFML/Graphics.hpp>
#include <string>

class SeccionHUD {
public:
    void loadFont(const sf::Font& font);
    void showName(const std::string& name);  // muestra el nombre por 3 segundos
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    sf::Text sectionText;
    const sf::Font* font = nullptr;
    float displayTimer = 0.f;
    static constexpr float DISPLAY_TIME = 3.f;
};
#endif
