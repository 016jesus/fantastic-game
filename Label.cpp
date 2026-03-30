#include "Label.h"

Label::Label() {
    text.setFillColor(sf::Color::White);
}

Label::Label(const std::string& str, const sf::Font& font, unsigned size,
             sf::Color color) {
    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setString(str);
    applyAlignment();
}

void Label::setText(const std::string& str) {
    text.setString(str);
    applyAlignment();
}

void Label::setAlign(Align a) {
    align = a;
    applyAlignment();
}

void Label::setColor(sf::Color color) {
    text.setFillColor(color);
}

void Label::draw(sf::RenderWindow& window) {
    window.draw(text);
}

void Label::setPosition(float px, float py) {
    x = px;
    y = py;
    applyAlignment();
}

void Label::setSize(float pw, float ph) {
    w = pw;
    h = ph;
    applyAlignment();
}

sf::FloatRect Label::getBounds() const {
    return {x, y, w, h};
}

void Label::applyAlignment() {
    sf::FloatRect tb = text.getLocalBounds();
    float textH = tb.top + tb.height;

    switch (align) {
        case Align::Left:
            text.setOrigin(tb.left, tb.top);
            text.setPosition(x, y + (h - textH) / 2.f);
            break;
        case Align::Center:
            text.setOrigin(tb.left + tb.width / 2.f, tb.top);
            text.setPosition(x + w / 2.f, y + (h - textH) / 2.f);
            break;
        case Align::Right:
            text.setOrigin(tb.left + tb.width, tb.top);
            text.setPosition(x + w, y + (h - textH) / 2.f);
            break;
    }
}
