#pragma once
#ifndef SLIDER_H
#define SLIDER_H
#include "UIWidget.h"
#include <functional>
#include <string>

// ---------------------------------------------------------------------------
// Slider: control deslizante horizontal con rango configurable y callback
// onValueChanged. Muestra label a la izquierda y valor porcentual a la derecha.
// ---------------------------------------------------------------------------
class Slider : public UIWidget {
public:
    using ValueCallback = std::function<void(float)>;

    Slider();

    void setRange(float minVal, float maxVal);
    void setValue(float value);      // valor en [minVal, maxVal]
    float getValue() const;
    float getNormalized() const;     // valor normalizado en [0, 1]
    void setOnValueChanged(ValueCallback cb);
    void setLabel(const std::string& lbl, const sf::Font& font);

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void setPosition(float x, float y) override;
    void setSize(float w, float h) override;
    sf::FloatRect getBounds() const override;
    bool isHovered() const override;

private:
    sf::RectangleShape track;    // barra de fondo (gris oscuro)
    sf::RectangleShape fill;     // parte rellena hasta el thumb (azul claro)
    sf::RectangleShape thumb;    // cuadrado arrastrable
    sf::Text           labelText;
    sf::Text           valueText;
    const sf::Font*    font = nullptr;

    float minVal     = 0.f;
    float maxVal     = 1.f;
    float currentVal = 0.5f;
    bool  dragging   = false;
    bool  hovered    = false;
    ValueCallback callback;

    float thumbRadius = 7.f;  // mitad del lado del thumb cuadrado (14x14)

    // Posición y dimensiones del panel completo
    float panX = 0.f, panY = 0.f, panW = 200.f, panH = 24.f;

    void updateThumbPosition();
    void updateValueFromMouse(float mouseX);
};

#endif
