#pragma once
#ifndef SECCION_H
#define SECCION_H
#include "gameplay/Lugar.h"
#include <SFML/Audio.hpp>
#include <string>

class Seccion : public Lugar {
public:
    Seccion(std::string nombre, int tipo, sf::Color bgColor);
    ~Seccion() override;

    void loadBackground(float width, float height) override;
    void drawBackground(sf::RenderWindow& window) override;
    int getTipo() const override;
    std::string getNombre() const override;

    // Música
    void setMusica(const std::string& path);
    void playMusica();
    void stopMusica();
    bool isMusicPlaying() const;

private:
    std::string nombre;
    int tipo;
    sf::Color bgColor;
    sf::RectangleShape background;

    sf::Music music;
    std::string musicPath;
    bool musicLoaded = false;
};
#endif
