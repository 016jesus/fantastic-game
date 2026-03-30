#include "core/NotificationSystem.h"
#include <algorithm>
#include <cmath>

// ---------------------------------------------------------------------------
// getInstance — singleton de Meyer.
// ---------------------------------------------------------------------------
NotificationSystem& NotificationSystem::getInstance() {
    static NotificationSystem instance;
    return instance;
}

// ---------------------------------------------------------------------------
// setFont — inyecta la fuente compartida; configura toastText una sola vez.
// ---------------------------------------------------------------------------
void NotificationSystem::setFont(const sf::Font& f) {
    font = &f;
    if (font) {
        toastText.setFont(*font);
        toastText.setCharacterSize(10u);
    }
}

// ---------------------------------------------------------------------------
// push — añade un toast al frente de la cola.
// Si la cola está llena, elimina el más antiguo (el último del deque).
// ---------------------------------------------------------------------------
void NotificationSystem::push(const std::string& message,
                               sf::Color color,
                               float duration)
{
    if (static_cast<int>(queue.size()) >= MAX_NOTIFICATIONS) {
        queue.pop_back();
    }

    Notification n;
    n.message  = message;
    n.color    = color;
    n.duration = duration;
    n.elapsed  = 0.f;
    n.y        = -30.f;  // fuera de pantalla, animará hacia su target

    queue.push_front(n);
}

// ---------------------------------------------------------------------------
// update — avanza el tiempo de cada notificación, anima Y y expira las viejas.
// ---------------------------------------------------------------------------
void NotificationSystem::update(float deltaTime) {
    for (int i = 0; i < static_cast<int>(queue.size()); ++i) {
        Notification& n = queue[static_cast<std::size_t>(i)];
        n.elapsed += deltaTime;

        // Target Y: el toast 0 (más reciente) va arriba; los siguientes bajan.
        const float targetY = TARGET_Y_BASE +
                              static_cast<float>(i) * (TOAST_HEIGHT + TOAST_GAP);

        // Lerp suave hacia targetY.
        const float diff = targetY - n.y;
        const float step = ANIM_SPEED * deltaTime / (std::abs(diff) + 0.01f);
        n.y += diff * std::min(1.f, step);
    }

    // Eliminar notificaciones expiradas.
    while (!queue.empty() && queue.back().elapsed >= queue.back().duration) {
        queue.pop_back();
    }
}

// ---------------------------------------------------------------------------
// draw — dibuja todos los toasts en la esquina superior derecha.
// Ventana efectiva: 480px de ancho (igual que en PlayingState).
// ---------------------------------------------------------------------------
void NotificationSystem::draw(sf::RenderWindow& window) {
    if (!font) return;

    for (const auto& n : queue) {
        // Calcular alpha: fade-out en los últimos FADE_TIME segundos.
        float alpha = 255.f;
        if (n.elapsed > n.duration - FADE_TIME) {
            alpha = 255.f * (n.duration - n.elapsed) / FADE_TIME;
        }
        if (alpha < 0.f) alpha = 0.f;
        if (alpha > 255.f) alpha = 255.f;

        const sf::Uint8 a = static_cast<sf::Uint8>(alpha);

        // Medir el texto para calcular posiciones.
        toastText.setString(n.message);
        const float textWidth = toastText.getLocalBounds().width;

        // --- Fondo semitransparente ---
        const float bgX = 480.f - textWidth - 16.f;
        toastBg.setSize(sf::Vector2f(textWidth + 16.f, TOAST_HEIGHT));
        toastBg.setPosition(bgX, n.y);
        toastBg.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(alpha * 0.7f)));
        window.draw(toastBg);

        // --- Borde izquierdo de 3px del color de la notificación ---
        sf::RectangleShape border(sf::Vector2f(3.f, TOAST_HEIGHT));
        border.setPosition(bgX, n.y);
        border.setFillColor(sf::Color(n.color.r, n.color.g, n.color.b, a));
        window.draw(border);

        // --- Texto del mensaje ---
        toastText.setFillColor(sf::Color(n.color.r, n.color.g, n.color.b, a));
        toastText.setPosition(480.f - textWidth - 8.f, n.y + 4.f);
        window.draw(toastText);
    }
}
