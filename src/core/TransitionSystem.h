#pragma once
#ifndef TRANSITION_SYSTEM_H
#define TRANSITION_SYSTEM_H
#include <SFML/Graphics.hpp>
#include <functional>

// ---------------------------------------------------------------------------
// TransitionSystem: singleton de fade global entre cambios de estado.
// Se integra en el game loop sin modificar los estados existentes.
// Principio OCP — los estados no necesitan conocer este sistema.
// ---------------------------------------------------------------------------
class TransitionSystem {
public:
    static TransitionSystem& getInstance();

    enum class Type { FadeToBlack, FadeFromBlack };

    // Inicia una transición. onComplete se llama cuando el fade termina.
    void start(Type type, float duration, std::function<void()> onComplete = nullptr);

    bool isActive() const;

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    TransitionSystem() = default;

    Type  currentType = Type::FadeToBlack;
    float duration    = 0.3f;
    float elapsed     = 0.f;
    bool  active      = false;
    std::function<void()> callback;

    sf::RectangleShape overlay;
};
#endif
