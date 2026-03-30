#pragma once
#ifndef MENU_STATE_H
#define MENU_STATE_H
#include "core/IGameState.h"
#include "core/SaveSystem.h"
#include "states/OptionsState.h"
#include <SFML/Graphics.hpp>
#include <memory>
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

    // Género del jugador seleccionado en el menú
    enum class Gender { Male, Female };
    Gender selectedGender = Gender::Male;
    sf::Text genderLabel;
    sf::Text genderMaleBtn;
    sf::Text genderFemaleBtn;
    sf::RectangleShape genderHighlight;

    // "Continuar partida" — visible sólo si existe un archivo de save
    bool hasSaveGame = false;
    sf::Text continueText;

    // Hint de acceso rápido a Opciones (tecla O)
    sf::Text optionsHint;

    void setupUI(const sf::Vector2u& windowSize);
    void startGame();
    void updateGenderHighlight();   // reposiciona el rectángulo según selección
    std::unique_ptr<OptionsState> createOptionsState();
};
#endif
