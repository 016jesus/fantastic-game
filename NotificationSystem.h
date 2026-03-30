#pragma once
#ifndef NOTIFICATION_SYSTEM_H
#define NOTIFICATION_SYSTEM_H
#include <SFML/Graphics.hpp>
#include <deque>
#include <string>
#include <functional>

// ---------------------------------------------------------------------------
// Notification — dato de un toast individual en pantalla.
// ---------------------------------------------------------------------------
struct Notification {
    std::string  message;
    sf::Color    color    = sf::Color::White;
    float        duration = 2.5f;  // segundos totales
    float        elapsed  = 0.f;
    float        y        = 0.f;   // posición Y actual (animada)
};

// ---------------------------------------------------------------------------
// NotificationSystem — sistema de toast messages en esquina superior derecha.
//
// Principio Open/Closed: nuevos tipos de notificación se añaden llamando a
// push() con el color deseado sin modificar esta clase.
// Singleton de Meyer (thread-safe desde C++11).
// ---------------------------------------------------------------------------
class NotificationSystem {
public:
    static NotificationSystem& getInstance();

    // Muestra un toast. color=White por defecto.
    void push(const std::string& message,
              sf::Color color  = sf::Color::White,
              float duration   = 2.5f);

    // Shortcuts semánticos — OCP: extensible sin tocar push().
    void info(const std::string& msg)    { push(msg, sf::Color::White); }
    void success(const std::string& msg) { push(msg, sf::Color(100, 220, 100)); }
    void warning(const std::string& msg) { push(msg, sf::Color(255, 200, 50)); }
    void error(const std::string& msg)   { push(msg, sf::Color(255, 80, 80)); }

    // Inyecta la fuente (debe llamarse antes del primer draw).
    void setFont(const sf::Font& font);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    NotificationSystem() = default;

    // No copiable ni movible — es un singleton.
    NotificationSystem(const NotificationSystem&)            = delete;
    NotificationSystem& operator=(const NotificationSystem&) = delete;

    static constexpr int   MAX_NOTIFICATIONS = 5;
    static constexpr float FADE_TIME         = 0.5f;   // tiempo de fade-out al final
    static constexpr float ANIM_SPEED        = 200.f;  // px/s de animación vertical
    static constexpr float TARGET_Y_BASE     = 50.f;   // Y del primer toast
    static constexpr float TOAST_HEIGHT      = 22.f;
    static constexpr float TOAST_GAP         = 4.f;

    std::deque<Notification> queue;
    const sf::Font*    font      = nullptr;
    sf::RectangleShape toastBg;
    sf::Text           toastText;
};
#endif
