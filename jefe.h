#pragma once
#ifndef JEFE_H
#define JEFE_H
#include "enemigos.h"
#include "EnemyState.h"
#include "skins.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Protagonista;

// ---------------------------------------------------------------------------
// Jefe — boss con IA de 6 estados y ataque especial de área.
//
// Estados: Idle -> Chase -> Attack / SpecialAttack -> Hit -> Dead
//
// El Jefe tiene su propia barra de HP visible sobre el sprite.
// getTipo() retorna 2 para diferenciarlo de Skeleton (1) y Normales (0).
// ---------------------------------------------------------------------------
class Jefe : public Enemigos {
public:
    Jefe(float x, float y);
    ~Jefe() = default;

    // No copiable (unique_ptr<Skins>). Move por defecto es suficiente.
    Jefe(Jefe&&) = default;
    Jefe& operator=(Jefe&&) = default;
    Jefe(const Jefe&) = delete;
    Jefe& operator=(const Jefe&) = delete;

    // Enemigos interface
    int getTipo() override;     // retorna 2

    // AI update — llamar cada frame
    void update(float deltaTime, const Protagonista& player);
    void draw(sf::RenderWindow& window);

    // Combat
    void takeDamage(int amount);
    bool isDead() const;
    float getAttackDamage() const;
    float getSpecialAttackDamage() const;
    bool canAttackPlayer(const Protagonista& player) const;
    bool canSpecialAttackPlayer(const Protagonista& player) const;
    void resetAttackCooldown();
    void resetSpecialAttackCooldown();

    // Combate interface (virtuales puros heredados de Combate via Enemigos)
    int ataque(Armas* arma) override;
    sf::Sprite* barraDeVida() override;

    // Position
    float getX() const;
    float getY() const;
    sf::FloatRect getBounds() const;

    // Animaciones interface — no-op: el boss se mueve por IA, no por teclado
    void movimientos(sf::Keyboard* key) override { (void)key; }

private:
    EnemyState state = EnemyState::Idle;
    float x, y;
    float stateTimer            = 0.f;
    float attackCooldown        = 0.f;
    float specialAttackCooldown = 0.f;
    bool  attackHit             = false;
    bool  specialHit            = false;

    std::unique_ptr<Skins> skin;
    sf::RectangleShape debugRect;  // fallback visual magenta

    // Barra de HP visible sobre el boss
    sf::RectangleShape hpBarBg;
    sf::RectangleShape hpBarFill;

    // --- Constantes de diseño ---
    static constexpr float DETECTION_RANGE      = 200.f;
    static constexpr float ATTACK_RANGE         = 40.f;
    static constexpr float SPECIAL_ATTACK_RANGE = 80.f;   // área más amplia
    static constexpr float SPEED                = 45.f;
    static constexpr float ATTACK_COOLDOWN      = 1.5f;
    static constexpr float SPECIAL_COOLDOWN     = 5.0f;   // cada 5 segundos
    static constexpr float ATTACK_DAMAGE        = 15.f;
    static constexpr float SPECIAL_DAMAGE       = 35.f;
    static constexpr int   HP_MAX               = 200;
    static constexpr int   WIDTH                = 48;
    static constexpr int   HEIGHT               = 48;

    // --- Helpers de IA ---
    void transitionTo(EnemyState newState);
    void updateIdle(float dt, const Protagonista& player);
    void updateChase(float dt, const Protagonista& player);
    void updateAttack(float dt, const Protagonista& player);
    void updateSpecialAttack(float dt, const Protagonista& player);
    void updateHit(float dt);
    void updateDead(float dt);
    float distanceTo(const Protagonista& player) const;
    void syncSpritePosition();
    void updateHealthBar();
};
#endif
