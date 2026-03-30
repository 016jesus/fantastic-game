#include "GameStateManager.h"

// Empuja un nuevo estado al stack y llama su onEnter
void GameStateManager::push(std::unique_ptr<IGameState> state) {
    state->onEnter();
    states.push(std::move(state));
}

// Saca el estado del tope: llama onExit al que sale,
// luego onEnter al nuevo tope si existe (reanuda)
void GameStateManager::pop() {
    if (states.empty()) return;

    states.top()->onExit();
    states.pop();

    if (!states.empty()) {
        states.top()->onEnter();
    }
}

// Reemplaza el estado actual sin llamar onEnter del que sale,
// luego hace push normal del nuevo estado
void GameStateManager::replace(std::unique_ptr<IGameState> state) {
    if (!states.empty()) {
        states.top()->onExit();
        states.pop();
    }
    state->onEnter();
    states.push(std::move(state));
}

// Delega el evento al estado del tope del stack
void GameStateManager::handleEvent(const sf::Event& event) {
    if (!states.empty()) {
        states.top()->handleEvent(event);
    }
}

// Delega la actualización al estado del tope
void GameStateManager::update(float deltaTime) {
    if (!states.empty()) {
        states.top()->update(deltaTime);
    }
}

// Delega el renderizado al estado del tope
void GameStateManager::render(sf::RenderWindow& window) {
    if (!states.empty()) {
        states.top()->render(window);
    }
}

// Retorna verdadero si el stack está vacío
bool GameStateManager::isEmpty() const {
    return states.empty();
}
