#pragma once
#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H
#include "IGameState.h"
#include <memory>
#include <stack>

class GameStateManager {
public:
    void push(std::unique_ptr<IGameState> state);
    void pop();
    void replace(std::unique_ptr<IGameState> state);
    void handleEvent(const sf::Event& event);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    bool isEmpty() const;

private:
    std::stack<std::unique_ptr<IGameState>> states;
};
#endif
