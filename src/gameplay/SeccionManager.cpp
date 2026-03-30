#include "gameplay/SeccionManager.h"

SeccionManager::SeccionManager(float windowWidth, float windowHeight)
    : winW(windowWidth), winH(windowHeight)
{
    buildDefaultSections();
}

void SeccionManager::buildDefaultSections() {
    sections.clear();
    index = 0;

    // Sección 0: Aldea — fondo verde bosque
    auto aldea = std::make_unique<Seccion>("Aldea", 0, sf::Color(34, 139, 34));
    aldea->loadBackground(winW, winH);
    sections.push_back(std::move(aldea));

    // Sección 1: Catacumbas — fondo gris oscuro azulado
    auto catacumbas = std::make_unique<Seccion>("Catacumbas", 0, sf::Color(60, 60, 80));
    catacumbas->loadBackground(winW, winH);
    sections.push_back(std::move(catacumbas));

    // Sección 2: Sala del Jefe — fondo rojo oscuro
    auto boss = std::make_unique<Seccion>("Sala del Jefe", 1, sf::Color(80, 20, 20));
    boss->loadBackground(winW, winH);
    sections.push_back(std::move(boss));
}

bool SeccionManager::nextSection() {
    if (index < static_cast<int>(sections.size()) - 1) {
        current()->stopMusica();
        ++index;
        current()->playMusica();
        return true;
    }
    return false;
}

Seccion* SeccionManager::current() {
    return sections[index].get();
}

const Seccion* SeccionManager::current() const {
    return sections[index].get();
}

int SeccionManager::currentIndex() const {
    return index;
}

int SeccionManager::totalSections() const {
    return static_cast<int>(sections.size());
}

bool SeccionManager::shouldTransition(float playerX, float transitionX) const {
    return playerX > transitionX;
}

void SeccionManager::update() {
    // no-op: reservado para lógica futura de mantenimiento de música
}
