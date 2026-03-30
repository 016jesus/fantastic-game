#pragma once
#ifndef KEY_BIND_WIDGET_H
#define KEY_BIND_WIDGET_H
#include "UIWidget.h"
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <functional>

// ---------------------------------------------------------------------------
// KeyBindWidget: fila de dos columnas para remapeo de teclas.
// Columna izquierda: nombre de la acción. Columna derecha: tecla asignada.
// Click en la columna derecha activa modo escucha; la siguiente tecla
// pulsada queda asignada. Escape cancela sin cambiar.
// ---------------------------------------------------------------------------
class KeyBindWidget : public UIWidget {
public:
    using BindCallback = std::function<void(sf::Keyboard::Key)>;

    KeyBindWidget();
    void setFont(const sf::Font& font, unsigned size = 12);
    void setAction(const std::string& actionName);
    void setCurrentKey(sf::Keyboard::Key key);
    sf::Keyboard::Key getCurrentKey() const;
    bool isListening() const;

    void setOnKeyBound(BindCallback cb);

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void setPosition(float x, float y) override;
    void setSize(float w, float h) override;
    sf::FloatRect getBounds() const override;
    bool isHovered() const override;

    // Convierte sf::Keyboard::Key a string legible
    static std::string keyToString(sf::Keyboard::Key key);

private:
    sf::RectangleShape actionBg;
    sf::RectangleShape keyBg;
    sf::Text           actionText;
    sf::Text           keyText;

    std::string       actionName;
    sf::Keyboard::Key boundKey  = sf::Keyboard::Unknown;
    bool              listening = false;
    bool              hovered   = false;
    float             blinkTimer = 0.f;
    BindCallback      callback;
    const sf::Font*   font = nullptr;

    float x = 0.f, y = 0.f, w = 280.f, h = 24.f;

    void enterListeningMode();
    void exitListeningMode(sf::Keyboard::Key newKey);
    void rebuildLayout();
};

#endif
