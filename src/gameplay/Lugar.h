#pragma once
#ifndef LUGAR_H
#define LUGAR_H
#include <SFML/Graphics.hpp>
#include <string>

class Lugar {
public:
    virtual ~Lugar() = default;
    // Carga el fondo de la sección (textura o color)
    virtual void loadBackground(float width, float height) = 0;
    // Dibuja el fondo
    virtual void drawBackground(sf::RenderWindow& window) = 0;
    // Retorna el tipo de sección (0=normal, 1=boss, 2=tienda)
    virtual int getTipo() const = 0;
    // Nombre de la sección
    virtual std::string getNombre() const = 0;
};
#endif
