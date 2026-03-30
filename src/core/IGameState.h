#pragma once
#ifndef IGAMESTATE_H
#define IGAMESTATE_H
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

class GameStateManager;

class IGameState {
public:
    virtual ~IGameState() = default;
    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void onEnter() {}
    virtual void onExit() {}
protected:
    GameStateManager* gsm = nullptr;
};
#endif
