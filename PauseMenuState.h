#pragma once
#ifndef PAUSE_MENU_STATE_H
#define PAUSE_MENU_STATE_H
#include "IGameState.h"
#include "Button.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

// ---------------------------------------------------------------------------
// PauseMenuState: menú de pausa con opciones reales.
// Se empuja sobre PlayingState en el GSM stack, que sigue visible debajo.
// Principio OCP — este estado se añade sin modificar IGameState ni GSM.
// ---------------------------------------------------------------------------
class PauseMenuState : public IGameState {
public:
    explicit PauseMenuState(GameStateManager* gsm);
    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;

private:
    sf::Font font;
    sf::RectangleShape overlay;
    sf::RectangleShape panel;
    sf::RectangleShape panelBorder;
    sf::Text titleText;

    std::vector<Button> buttons;  // Reanudar, Opciones, Guardar, Menú principal

    // Notificación de guardado temporal
    sf::Text saveNotification;
    float    saveNotificationTimer = 0.f;
    static constexpr float NOTIF_DURATION = 2.f;

    void buildUI();
    void resume();
    void openOptions();
    void saveGame();
    void goToMainMenu();
};
#endif
