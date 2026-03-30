#pragma once
#ifndef LABEL_H
#define LABEL_H
#include "ui/widgets/UIWidget.h"
#include <string>

// ---------------------------------------------------------------------------
// Label: texto estático con alineación Left/Center/Right.
// handleEvent y update son no-ops; isHovered devuelve siempre false.
// ---------------------------------------------------------------------------
class Label : public UIWidget {
public:
    enum class Align { Left, Center, Right };

    Label();
    Label(const std::string& text, const sf::Font& font, unsigned size = 13,
          sf::Color color = sf::Color::White);

    void setText(const std::string& text);
    void setAlign(Align align);
    void setColor(sf::Color color);

    void handleEvent(const sf::Event&) override {}
    void update(float) override {}
    void draw(sf::RenderWindow& window) override;
    void setPosition(float x, float y) override;
    void setSize(float w, float h) override;
    sf::FloatRect getBounds() const override;
    bool isHovered() const override { return false; }

private:
    sf::Text text;
    Align    align = Align::Left;
    float    x = 0.f, y = 0.f, w = 200.f, h = 20.f;

    void applyAlignment();
};

#endif
