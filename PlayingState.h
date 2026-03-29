#pragma once
#ifndef PLAYING_STATE_H
#define PLAYING_STATE_H
#include "IGameState.h"
#include "protagonista.h"
#include "MapBackground.h"
#include <SFML/Graphics.hpp>
#include <string>

class PlayingState : public IGameState {
public:
    PlayingState(GameStateManager* gsm, const std::string& playerName);
    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;

private:
    Protagonista player;
    MapBackground background;
    sf::Font font;
    sf::Text pauseHint;
    bool paused = false;

    // HUD
    sf::RectangleShape hpBarBg;
    sf::RectangleShape hpBarFill;
    sf::Text hpText;

    void handleMovement(float deltaTime);
    void updateHUD();
    void drawHUD(sf::RenderWindow& window);
    void applyGravity(float deltaTime);

    float groundY = 180.f;   // Y del suelo en coordenadas de mundo
    float velocityY = 0.f;
    bool onGround = true;
    static constexpr float GRAVITY    = 500.f;
    static constexpr float JUMP_FORCE = -300.f;
    static constexpr int   MAX_HP     = 100;
};
#endif
