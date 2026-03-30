#pragma once
#ifndef SECCION_MANAGER_H
#define SECCION_MANAGER_H
#include "gameplay/Seccion.h"
#include <vector>
#include <memory>
#include <functional>

class SeccionManager {
public:
    SeccionManager(float windowWidth, float windowHeight);

    // Crea las secciones predefinidas del juego
    void buildDefaultSections();

    // Avanza a la siguiente sección. Retorna false si es la última.
    bool nextSection();

    // Retorna la sección actual
    Seccion* current();
    const Seccion* current() const;

    int currentIndex() const;
    int totalSections() const;

    // Comprueba si el jugador llegó al borde derecho (x > limit)
    bool shouldTransition(float playerX, float transitionX = 470.f) const;

    void update();  // mantenimiento de música (no-op por ahora)

private:
    std::vector<std::unique_ptr<Seccion>> sections;
    int index = 0;
    float winW, winH;
};
#endif
