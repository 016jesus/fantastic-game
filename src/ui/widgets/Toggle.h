#pragma once
#ifndef TOGGLE_H
#define TOGGLE_H
#include "ui/widgets/UIWidget.h"
#include <functional>
#include <string>

// ---------------------------------------------------------------------------
// Toggle: switch on/off binario. Track verde (on) o gris (off);
// knob blanco que se desplaza izquierda/derecha. Soporta label.
// ---------------------------------------------------------------------------
class Toggle : public UIWidget {
public:
    using ChangeCallback = std::function<void(bool)>;

    Toggle();
    void setLabel(const std::string& lbl, const sf::Font& font, unsigned size = 13);
    void setValue(bool on);
    bool getValue() const;
    void setOnChanged(ChangeCallback cb);

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void setPosition(float x, float y) override;
    void setSize(float w, float h) override;
    sf::FloatRect getBounds() const override;
    bool isHovered() const override;

private:
    sf::RectangleShape track;   // fondo del toggle
    sf::CircleShape    knob;    // circulo deslizante
    sf::Text           labelText;

    bool on      = false;
    bool hovered = false;
    ChangeCallback callback;

    float x = 0.f, y = 0.f;
    float w = 40.f, h = 20.f;  // dimensiones del track

    void updateKnob();
};

#endif
