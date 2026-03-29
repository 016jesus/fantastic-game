#pragma once
#ifndef PLAYING_STATE_H
#define PLAYING_STATE_H
#include "IGameState.h"
#include "protagonista.h"
#include "MapBackground.h"
#include "skeleton.h"
#include "jefe.h"
#include "DialogBox.h"
#include "InventoryOverlay.h"
#include "ShopOverlay.h"
#include "SeccionManager.h"
#include "SeccionHUD.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <vector>

class PlayingState : public IGameState {
public:
    PlayingState(GameStateManager* gsm, const std::string& playerName,
                 const std::string& gender = "male");
    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;

private:
    std::string gender;                       // "male" o "female"
    Protagonista player;
    MapBackground background;
    sf::Font font;
    sf::Text pauseHint;
    bool paused = false;

    // Gestor de secciones del mapa y HUD de nombre de sección
    SeccionManager seccionManager{512.f, 256.f};
    SeccionHUD seccionHUD;

    // HUD
    sf::RectangleShape hpBarBg;
    sf::RectangleShape hpBarFill;
    sf::Text hpText;
    sf::Text moneyText;   // muestra las monedas del jugador

    // Enemies
    std::vector<Skeleton> enemies;
    bool attackPressed = false;  // true mientras J/Espacio esté pulsado

    // Boss (Sala del Jefe, sección 2)
    std::optional<Jefe> boss;
    bool bossDefeated = false;
    sf::Text victoryText;

    // -----------------------------------------------------------------------
    // Overlays de UI: diálogo, inventario, tienda
    // -----------------------------------------------------------------------
    DialogBox dialogBox;
    InventoryOverlay inventoryOverlay;
    ShopOverlay shopOverlay;

    // -----------------------------------------------------------------------
    // Puntos de interacción en el mapa
    // -----------------------------------------------------------------------
    struct NpcSpot {
        float x, y;
        std::string name;
        std::vector<std::string> dialog;
    };
    std::vector<NpcSpot> npcSpots;

    struct VendorSpot { float x, y; };
    VendorSpot vendorSpot{380.f, 148.f};

    struct ChestSpot {
        float x, y;
        bool opened;
        int money;
    };
    std::vector<ChestSpot> chestSpots;

    // -----------------------------------------------------------------------
    // Helpers de movimiento, HUD, combate y transición de secciones
    // -----------------------------------------------------------------------
    void handleMovement(float deltaTime);
    void updateHUD();
    void drawHUD(sf::RenderWindow& window);
    void applyGravity(float deltaTime);
    void checkSectionTransition();   // detecta si el jugador cruza al área siguiente

    // Enemy / combat helpers
    void spawnEnemies();
    void updateCombat(float deltaTime);
    void drawEnemies(sf::RenderWindow& window);

    // -----------------------------------------------------------------------
    // Helpers de NPCs, cofres e interacciones
    // -----------------------------------------------------------------------
    void setupNpcsAndChests();
    void checkInteractions();
    void drawNpcsAndChests(sf::RenderWindow& window);
    float distanceToPlayer(float ox, float oy);

    float groundY = 180.f;   // Y del suelo en coordenadas de mundo
    float velocityY = 0.f;
    bool onGround = true;
    static constexpr float GRAVITY    = 500.f;
    static constexpr float JUMP_FORCE = -300.f;
    static constexpr int   MAX_HP     = 100;

    // Rango de ataque del jugador en píxeles (distancia centro a AABB enemigo)
    static constexpr float PLAYER_ATTACK_RANGE  = 40.f;
    static constexpr int   PLAYER_ATTACK_DAMAGE = 20;

    // Radio de detección de interacción con objetos del mundo
    static constexpr float INTERACT_RADIUS = 40.f;
};
#endif
