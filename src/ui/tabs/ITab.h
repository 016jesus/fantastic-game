#pragma once
#ifndef ITAB_H
#define ITAB_H
#include <SFML/Graphics.hpp>
#include <string>

// ---------------------------------------------------------------------------
// ITab: interfaz base para todas las pestañas del menú de opciones.
// Principio OCP — esta clase NUNCA se modifica; las pestañas concretas
// (GraphicsTab, SoundTab, ControlsTab, etc.) la extienden.
// ---------------------------------------------------------------------------
class ITab {
public:
    virtual ~ITab() = default;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float deltaTime)              = 0;
    virtual void draw(sf::RenderWindow& window)       = 0;
    virtual std::string getTitle() const              = 0;

    // Hooks opcionales llamados al activar/desactivar la pestaña
    virtual void onActivate()   {}
    virtual void onDeactivate() {}
};

#endif
