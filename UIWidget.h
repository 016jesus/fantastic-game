#pragma once
#ifndef UI_WIDGET_H
#define UI_WIDGET_H
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

// ---------------------------------------------------------------------------
// UIWidget: interfaz raíz de todo el sistema de widgets de UI.
// Principio OCP — esta clase NUNCA se modifica; la funcionalidad
// se añade creando nuevas subclases.
// ---------------------------------------------------------------------------
class UIWidget {
public:
    virtual ~UIWidget() = default;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float deltaTime)              = 0;
    virtual void draw(sf::RenderWindow& window)       = 0;

    virtual void setPosition(float x, float y)        = 0;
    virtual void setSize(float width, float height)   = 0;
    virtual sf::FloatRect getBounds() const           = 0;
    virtual bool isHovered() const                    = 0;

    // Helpers no-virtuales — disponibles para todos los widgets
    bool contains(sf::Vector2f point) const {
        return getBounds().contains(point);
    }
    bool contains(float x, float y) const {
        return contains({x, y});
    }
};

#endif
