#include "Toggle.h"

Toggle::Toggle() {
    track.setSize({w, h});
    track.setFillColor(sf::Color(80, 80, 90));

    float radius = h / 2.f - 2.f;
    knob.setRadius(radius);
    knob.setFillColor(sf::Color::White);

    updateKnob();
}

void Toggle::setLabel(const std::string& lbl, const sf::Font& font, unsigned size) {
    labelText.setFont(font);
    labelText.setString(lbl);
    labelText.setCharacterSize(size);
    labelText.setFillColor(sf::Color(200, 200, 200));
}

void Toggle::setValue(bool value) {
    on = value;
    updateKnob();
}

bool Toggle::getValue() const {
    return on;
}

void Toggle::setOnChanged(ChangeCallback cb) {
    callback = std::move(cb);
}

void Toggle::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mouse(static_cast<float>(event.mouseMove.x),
                           static_cast<float>(event.mouseMove.y));
        hovered = getBounds().contains(mouse);
    }

    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse(static_cast<float>(event.mouseButton.x),
                           static_cast<float>(event.mouseButton.y));
        if (getBounds().contains(mouse)) {
            on = !on;
            updateKnob();
            if (callback) callback(on);
        }
    }
}

void Toggle::update(float /*dt*/) {
    track.setFillColor(on ? sf::Color(60, 180, 80) : sf::Color(80, 80, 90));
}

void Toggle::draw(sf::RenderWindow& window) {
    window.draw(track);
    window.draw(knob);
    if (!labelText.getString().isEmpty()) {
        window.draw(labelText);
    }
}

void Toggle::setPosition(float px, float py) {
    x = px;
    y = py;
    track.setPosition(x, y);

    // Label a la derecha del track
    sf::FloatRect lb = labelText.getLocalBounds();
    labelText.setPosition(x + w + 8.f,
                          y + h / 2.f - lb.height / 2.f);
    updateKnob();
}

void Toggle::setSize(float pw, float ph) {
    w = pw;
    h = ph;
    track.setSize({w, h});

    float radius = h / 2.f - 2.f;
    knob.setRadius(radius);

    setPosition(x, y);
}

sf::FloatRect Toggle::getBounds() const {
    return {x, y, w, h};
}

bool Toggle::isHovered() const {
    return hovered;
}

void Toggle::updateKnob() {
    float radius = knob.getRadius();
    float padding = 2.f;
    float knobX = on ? (x + w - radius * 2.f - padding)
                     : (x + padding);
    float knobY = y + h / 2.f - radius;
    knob.setPosition(knobX, knobY);
}
