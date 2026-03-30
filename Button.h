#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#include "UIWidget.h"
#include <string>
#include <functional>

// ---------------------------------------------------------------------------
// Button: botón con texto, estados hover/activo y callback al hacer click.
// ---------------------------------------------------------------------------
class Button : public UIWidget {
public:
    using Callback = std::function<void()>;

    Button();
    Button(const std::string& label, const sf::Font& font, unsigned charSize = 14);

    void setLabel(const std::string& label);
    void setFont(const sf::Font& font, unsigned charSize = 14);
    void setCallback(Callback cb);

    // Colores para cada estado visual
    void setNormalColor(sf::Color bg, sf::Color text = sf::Color::White);
    void setHoverColor(sf::Color bg,  sf::Color text = sf::Color::White);
    void setActiveColor(sf::Color bg, sf::Color text = sf::Color::Yellow);

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void setPosition(float x, float y) override;
    void setSize(float w, float h) override;
    sf::FloatRect getBounds() const override;
    bool isHovered() const override;

private:
    sf::RectangleShape bg;
    sf::Text           label;
    Callback           callback;
    bool hovered = false;
    bool pressed = false;

    sf::Color normalBg  {60,  60,  80};
    sf::Color hoverBg   {90,  90, 120};
    sf::Color activeBg  {120, 100,  40};
    sf::Color normalText{sf::Color::White};
    sf::Color hoverText {sf::Color::White};
    sf::Color activeText{sf::Color::Yellow};

    void centerText();
};

#endif
