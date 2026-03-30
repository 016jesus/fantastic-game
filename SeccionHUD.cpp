#include "SeccionHUD.h"
#include <algorithm>

void SeccionHUD::loadFont(const sf::Font& f) {
    font = &f;
    sectionText.setFont(*font);
    sectionText.setCharacterSize(16);
    sectionText.setFillColor(sf::Color(255, 255, 255, 200));
}

void SeccionHUD::showName(const std::string& name) {
    if (!font) return;

    sectionText.setString(name);
    sectionText.setCharacterSize(16);
    sectionText.setFillColor(sf::Color(255, 255, 255, 200));

    // Centrar horizontalmente en la parte superior
    sf::FloatRect bounds = sectionText.getLocalBounds();
    sectionText.setOrigin(bounds.left + bounds.width / 2.f,
                          bounds.top);
    // La posición X se ajustará en draw() usando el ancho de la ventana;
    // usamos 240.f como posición de pantalla ancha por defecto (mitad de 480).
    sectionText.setPosition(240.f, 50.f);

    displayTimer = DISPLAY_TIME;
}

void SeccionHUD::update(float deltaTime) {
    if (displayTimer > 0.f) {
        displayTimer -= deltaTime;
        if (displayTimer < 0.f) {
            displayTimer = 0.f;
        }
    }
}

void SeccionHUD::draw(sf::RenderWindow& window) {
    if (displayTimer <= 0.f) return;

    // Fade out durante el último segundo
    sf::Uint8 alpha = 200;
    if (displayTimer < 1.f) {
        // Interpola de 200 a 0 en el último segundo
        alpha = static_cast<sf::Uint8>(200.f * (displayTimer / 1.f));
    }

    sf::Color col = sectionText.getFillColor();
    col.a = alpha;
    sectionText.setFillColor(col);

    window.draw(sectionText);
}
