#include "TransitionSystem.h"
#include <algorithm>

// ---------------------------------------------------------------------------
TransitionSystem& TransitionSystem::getInstance() {
    static TransitionSystem instance;
    return instance;
}

// ---------------------------------------------------------------------------
// start: inicia una transición de fade. Llama antes de cambiar de estado.
// ---------------------------------------------------------------------------
void TransitionSystem::start(Type type, float dur,
                             std::function<void()> onComplete)
{
    currentType = type;
    duration    = dur;
    elapsed     = 0.f;
    active      = true;
    callback    = std::move(onComplete);

    // El overlay cubre toda la ventana lógica del juego (512x256)
    overlay.setSize(sf::Vector2f(512.f, 256.f));
    overlay.setPosition(0.f, 0.f);
}

// ---------------------------------------------------------------------------
bool TransitionSystem::isActive() const {
    return active;
}

// ---------------------------------------------------------------------------
// update: avanza el tiempo y dispara el callback al terminar
// ---------------------------------------------------------------------------
void TransitionSystem::update(float deltaTime) {
    if (!active) return;

    elapsed += deltaTime;

    if (elapsed >= duration) {
        active = false;

        if (callback) {
            auto cb = std::move(callback);
            callback = nullptr;
            cb();
        }
    }
}

// ---------------------------------------------------------------------------
// draw: pinta el overlay con el alpha calculado según tipo y progreso
// ---------------------------------------------------------------------------
void TransitionSystem::draw(sf::RenderWindow& window) {
    if (!active) return;

    const float t     = elapsed / duration;
    const float tClamped = std::max(0.f, std::min(t, 1.f));

    float alpha = 0.f;
    if (currentType == Type::FadeToBlack) {
        alpha = 255.f * tClamped;
    } else {
        // FadeFromBlack: empieza opaco y se hace transparente
        alpha = 255.f * (1.f - tClamped);
    }

    overlay.setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(alpha)));
    window.draw(overlay);
}
