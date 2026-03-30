#include "Seccion.h"
#include <iostream>

Seccion::Seccion(std::string nombre, int tipo, sf::Color bgColor)
    : nombre(std::move(nombre)), tipo(tipo), bgColor(bgColor)
{
}

Seccion::~Seccion() {
    stopMusica();
}

void Seccion::loadBackground(float width, float height) {
    background.setSize(sf::Vector2f(width, height));
    background.setPosition(0.f, 0.f);
    background.setFillColor(bgColor);
}

void Seccion::drawBackground(sf::RenderWindow& window) {
    window.draw(background);
}

int Seccion::getTipo() const {
    return tipo;
}

std::string Seccion::getNombre() const {
    return nombre;
}

void Seccion::setMusica(const std::string& path) {
    musicPath = path;
    if (path.empty()) {
        musicLoaded = false;
        return;
    }
    if (music.openFromFile(path)) {
        musicLoaded = true;
    } else {
        std::cerr << "Seccion: no se pudo abrir música: " << path << "\n";
        musicLoaded = false;
    }
}

void Seccion::playMusica() {
    if (musicLoaded) {
        music.setLoop(true);
        music.play();
    }
}

void Seccion::stopMusica() {
    music.stop();
}

bool Seccion::isMusicPlaying() const {
    return music.getStatus() == sf::Music::Playing;
}
