#include "core/GameStateManager.h"
#include "states/MenuState.h"
#include "core/TransitionSystem.h"
#include "systems/GamepadManager.h"
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(512, 256), "The Legend of Tilin",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    GameStateManager gsm;
    gsm.push(std::make_unique<MenuState>(&gsm));

    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        // Cap de delta para evitar espirales de muerte ante lag o breakpoints
        if (deltaTime > 0.05f) deltaTime = 0.05f;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (!gsm.isEmpty())
                gsm.handleEvent(event);
        }

        gsm.update(deltaTime);
        GamepadManager::getInstance().update();
        TransitionSystem::getInstance().update(deltaTime);

        window.clear(sf::Color::Black);
        gsm.render(window);
        TransitionSystem::getInstance().draw(window);
        window.display();

        // Si el stack queda vacío (todos los estados salieron) se cierra la ventana
        if (gsm.isEmpty())
            window.close();
    }

    return 0;
}
