#pragma once
#ifndef SKELETON_H
#define SKELETON_H

#include "entities/base/normales.h"
#include "entities/enemies/EnemyState.h"
#include "entities/player/skins.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Protagonista; // forward declaration — evita inclusión circular

// ---------------------------------------------------------------------------
// Skeleton — enemigo terrestre con IA de máquina de estados completa.
//
// Estados: Idle -> Patrol -> Chase -> Attack -> Hit -> Dead
//
// Física:
//   - Se mueve en el eje X únicamente (no aplica gravedad propia; el
//     PlayingState lo sitúa sobre groundY al hacer spawn).
//   - getBounds() devuelve un AABB de WIDTH x HEIGHT para detección de
//     colisiones con el jugador.
// ---------------------------------------------------------------------------
class Skeleton : public Normales {
public:
    Skeleton(float x, float y);
    ~Skeleton() = default;

    // Skeleton contiene un std::unique_ptr<Skins> — no es copiable.
    // Declaramos move explícitamente para que std::vector pueda reubicar
    // elementos internamente y std::remove_if pueda mover al borrar.
    Skeleton(Skeleton&&) = default;
    Skeleton& operator=(Skeleton&&) = default;
    Skeleton(const Skeleton&) = delete;
    Skeleton& operator=(const Skeleton&) = delete;

    // Normales/Enemigos interface
    int getTipo() override;

    // --- IA principal — llamar cada frame ---
    void update(float deltaTime, const Protagonista& player);
    void draw(sf::RenderWindow& window);

    // --- Combat ---
    void takeDamage(int amount);
    bool isDead() const;
    float getAttackDamage() const;
    // Retorna true una sola vez por ciclo de ataque (flag se limpia solo).
    bool canAttackPlayer(const Protagonista& player) const;
    // Debe llamarse desde PlayingState después de aplicar el daño al jugador.
    void resetAttackCooldown();

    // Combate interface (virtual puro heredado).
    // barraDeVida() retorna Sprite* (sin sf:: para coincidir con la firma
    // de la clase base Combate que usa nombres SFML sin calificar).
    int ataque(Armas* arma) override;
    sf::Sprite* barraDeVida() override { return nullptr; }

    // --- Position / Bounds ---
    float getX() const;
    float getY() const;
    sf::FloatRect getBounds() const;

private:
    EnemyState state = EnemyState::Idle;

    float x;
    float y;
    float patrolOriginX;
    float patrolDir  = 1.f;  // +1 derecha, -1 izquierda
    float stateTimer = 0.f;
    float attackCooldown = 0.f;
    float idleTimer  = 0.f;

    // Se activa en updateAttack() cuando el golpe debe aplicarse;
    // se lee con canAttackPlayer() y se limpia con resetAttackCooldown().
    mutable bool attackHit = false;

    std::unique_ptr<Skins> skin;
    sf::RectangleShape debugRect; // respaldo visual si no hay sprites

    // --- Constantes de diseño ---
    static constexpr float DETECTION_RANGE  = 120.f;
    static constexpr float ATTACK_RANGE     = 28.f;
    static constexpr float SPEED            = 60.f;
    static constexpr float ATTACK_COOLDOWN  = 1.2f;
    static constexpr float ATTACK_DAMAGE    = 10.f;
    static constexpr int   WIDTH            = 32;
    static constexpr int   HEIGHT           = 32;

    // Dimensiones reales de los sprite sheets del Skeleton
    // (medidas de los PNG en Sprites/Skeleton/Sprite_Sheets/)
    static constexpr int   SK_FRAME_W_IDLE   = 33;   // 264 / 8 frames
    static constexpr int   SK_FRAME_H_IDLE   = 32;
    static constexpr int   SK_FRAMES_IDLE    = 8;

    static constexpr int   SK_FRAME_W_WALK   = 26;   // 286 / 11 frames
    static constexpr int   SK_FRAME_H_WALK   = 33;
    static constexpr int   SK_FRAMES_WALK    = 11;

    static constexpr int   SK_FRAME_W_ATTACK = 86;   // 774 / 9 frames
    static constexpr int   SK_FRAME_H_ATTACK = 37;
    static constexpr int   SK_FRAMES_ATTACK  = 9;

    static constexpr int   SK_FRAME_W_HIT    = 24;   // 240 / 10 frames
    static constexpr int   SK_FRAME_H_HIT    = 32;
    static constexpr int   SK_FRAMES_HIT     = 10;

    static constexpr int   SK_FRAME_W_DEAD   = 55;   // 495 / 9 frames
    static constexpr int   SK_FRAME_H_DEAD   = 32;
    static constexpr int   SK_FRAMES_DEAD    = 9;

    // --- Helpers de IA ---
    void transitionTo(EnemyState newState);
    void updateIdle(float deltaTime, const Protagonista& player);
    void updatePatrol(float deltaTime, const Protagonista& player);
    void updateChase(float deltaTime, const Protagonista& player);
    void updateAttack(float deltaTime, const Protagonista& player);
    void updateHit(float deltaTime);
    void updateDead(float deltaTime);
    float distanceTo(const Protagonista& player) const;

    // --- Sprites ---
    void loadSprites();
    void syncSpritePosition();
};

#endif
