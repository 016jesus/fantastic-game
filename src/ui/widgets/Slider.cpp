#include "ui/widgets/Slider.h"
#include <algorithm>
#include <cmath>
#include <string>

Slider::Slider() {
    track.setFillColor(sf::Color(50, 50, 60));
    fill.setFillColor(sf::Color(80, 140, 200));
    thumb.setFillColor(sf::Color(200, 200, 220));
    thumb.setSize({thumbRadius * 2.f, thumbRadius * 2.f});
}

void Slider::setRange(float minV, float maxV) {
    minVal = minV;
    maxVal = maxV;
    // Clamp current value al nuevo rango
    currentVal = std::max(minVal, std::min(currentVal, maxVal));
    updateThumbPosition();
}

void Slider::setValue(float value) {
    currentVal = std::max(minVal, std::min(value, maxVal));
    updateThumbPosition();
}

float Slider::getValue() const {
    return currentVal;
}

float Slider::getNormalized() const {
    if (std::abs(maxVal - minVal) < 1e-6f) return 0.f;
    return (currentVal - minVal) / (maxVal - minVal);
}

void Slider::setOnValueChanged(ValueCallback cb) {
    callback = std::move(cb);
}

void Slider::setLabel(const std::string& lbl, const sf::Font& f) {
    font = &f;
    labelText.setFont(f);
    labelText.setString(lbl);
    labelText.setCharacterSize(12);
    labelText.setFillColor(sf::Color(200, 200, 200));

    valueText.setFont(f);
    valueText.setCharacterSize(12);
    valueText.setFillColor(sf::Color(200, 200, 200));
}

void Slider::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mouse(static_cast<float>(event.mouseMove.x),
                           static_cast<float>(event.mouseMove.y));
        hovered = getBounds().contains(mouse);

        if (dragging) {
            updateValueFromMouse(mouse.x);
        }
    }

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse(static_cast<float>(event.mouseButton.x),
                           static_cast<float>(event.mouseButton.y));
        // Permite comenzar el drag tanto en el thumb como en la track
        if (getBounds().contains(mouse)) {
            dragging = true;
            updateValueFromMouse(mouse.x);
        }
    }

    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {
        dragging = false;
    }
}

void Slider::update(float /*dt*/) {
    // Actualiza el texto del valor
    int pct = static_cast<int>(std::round(getNormalized() * 100.f));
    valueText.setString(std::to_string(pct) + "%");

    // Reposiciona el texto del valor a la derecha
    float trackRight = panX + panW;
    sf::FloatRect vb = valueText.getLocalBounds();
    valueText.setPosition(trackRight + 6.f,
                          panY + panH / 2.f - vb.height / 2.f);
}

void Slider::draw(sf::RenderWindow& window) {
    window.draw(track);
    window.draw(fill);
    window.draw(thumb);
    if (font) {
        window.draw(labelText);
        window.draw(valueText);
    }
}

void Slider::setPosition(float x, float y) {
    panX = x;
    panY = y;

    // Reserva espacio para el label a la izquierda
    float labelW = font ? 80.f : 0.f;
    float trackX = panX + labelW;
    float trackW = panW - labelW - 50.f; // 50 para texto de valor
    float trackH = 6.f;
    float trackY = panY + panH / 2.f - trackH / 2.f;

    track.setPosition(trackX, trackY);
    track.setSize({trackW, trackH});

    if (font) {
        sf::FloatRect lb = labelText.getLocalBounds();
        labelText.setPosition(panX,
                              panY + panH / 2.f - lb.height / 2.f);
    }

    updateThumbPosition();
}

void Slider::setSize(float w, float h) {
    panW = w;
    panH = h;
    setPosition(panX, panY); // recalcula layout
}

sf::FloatRect Slider::getBounds() const {
    return {panX, panY, panW, panH};
}

bool Slider::isHovered() const {
    return hovered;
}

void Slider::updateThumbPosition() {
    sf::FloatRect tb = track.getGlobalBounds();
    float norm       = getNormalized();
    float thumbX     = tb.left + norm * tb.width - thumbRadius;
    float thumbY     = panY + panH / 2.f - thumbRadius;

    thumb.setPosition(thumbX, thumbY);
    fill.setPosition(tb.left, tb.top);
    fill.setSize({norm * tb.width, tb.height});
}

void Slider::updateValueFromMouse(float mouseX) {
    sf::FloatRect tb = track.getGlobalBounds();
    float norm = (mouseX - tb.left) / tb.width;
    norm       = std::max(0.f, std::min(norm, 1.f));
    float newVal = minVal + norm * (maxVal - minVal);
    if (std::abs(newVal - currentVal) > 1e-6f) {
        currentVal = newVal;
        updateThumbPosition();
        if (callback) callback(currentVal);
    }
}
