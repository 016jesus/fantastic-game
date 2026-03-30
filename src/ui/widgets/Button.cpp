#include "ui/widgets/Button.h"

Button::Button() {
    bg.setFillColor(normalBg);
}

Button::Button(const std::string& lbl, const sf::Font& font, unsigned charSize) {
    bg.setFillColor(normalBg);
    setFont(font, charSize);
    setLabel(lbl);
}

void Button::setLabel(const std::string& lbl) {
    label.setString(lbl);
    centerText();
}

void Button::setFont(const sf::Font& font, unsigned charSize) {
    label.setFont(font);
    label.setCharacterSize(charSize);
    label.setFillColor(normalText);
    centerText();
}

void Button::setCallback(Callback cb) {
    callback = std::move(cb);
}

void Button::setNormalColor(sf::Color bgColor, sf::Color textColor) {
    normalBg   = bgColor;
    normalText = textColor;
}

void Button::setHoverColor(sf::Color bgColor, sf::Color textColor) {
    hoverBg   = bgColor;
    hoverText = textColor;
}

void Button::setActiveColor(sf::Color bgColor, sf::Color textColor) {
    activeBg   = bgColor;
    activeText = textColor;
}

void Button::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mouse(static_cast<float>(event.mouseMove.x),
                           static_cast<float>(event.mouseMove.y));
        hovered = getBounds().contains(mouse);
    }

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse(static_cast<float>(event.mouseButton.x),
                           static_cast<float>(event.mouseButton.y));
        if (getBounds().contains(mouse)) {
            pressed = true;
        }
    }

    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse(static_cast<float>(event.mouseButton.x),
                           static_cast<float>(event.mouseButton.y));
        if (pressed && getBounds().contains(mouse)) {
            if (callback) callback();
        }
        pressed = false;
    }
}

void Button::update(float /*dt*/) {
    // Actualiza color según estado
    if (pressed) {
        bg.setFillColor(activeBg);
        label.setFillColor(activeText);
    } else if (hovered) {
        bg.setFillColor(hoverBg);
        label.setFillColor(hoverText);
    } else {
        bg.setFillColor(normalBg);
        label.setFillColor(normalText);
    }
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(bg);
    window.draw(label);
}

void Button::setPosition(float x, float y) {
    bg.setPosition(x, y);
    centerText();
}

void Button::setSize(float w, float h) {
    bg.setSize({w, h});
    centerText();
}

sf::FloatRect Button::getBounds() const {
    return bg.getGlobalBounds();
}

bool Button::isHovered() const {
    return hovered;
}

void Button::centerText() {
    sf::FloatRect bounds = bg.getGlobalBounds();
    sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin(textBounds.left + textBounds.width  / 2.f,
                    textBounds.top  + textBounds.height / 2.f);
    label.setPosition(bounds.left + bounds.width  / 2.f,
                      bounds.top  + bounds.height / 2.f);
}
