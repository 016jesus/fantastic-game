#pragma once
#ifndef MENU_STATE_H
#define MENU_STATE_H
#include "IGameState.h"
#include <SFML/Graphics.hpp>
#include <string>

class MenuState : public IGameState {
public:
    explicit MenuState(GameStateManager* gsm);
    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;

private:
    sf::Font font;
    sf::Text titleText;
    sf::Text promptText;
    sf::Text nameText;
    sf::Text instructionText;
    sf::RectangleShape inputBox;
    std::string playerName;
    bool fontLoaded = false;

    void setupUI(const sf::Vector2u& windowSize);
    void startGame();
};
#endif
