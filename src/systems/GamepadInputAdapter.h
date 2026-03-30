#pragma once
#ifndef GAMEPAD_INPUT_ADAPTER_H
#define GAMEPAD_INPUT_ADAPTER_H
#include "systems/KeyBindings.h"
#include "systems/GamepadManager.h"

// ---------------------------------------------------------------------------
// GamepadInputAdapter: fachada estática que unifica teclado y mando bajo una
// única consulta de acción lógica.
//
// Principio OCP — los sistemas de juego (PlayingState, PauseMenuState, etc.)
// no necesitan modificarse; basta con que consulten isPressed() en lugar de
// KeyBindings::isPressed() directamente para obtener soporte de mando.
//
// Solo header — no tiene estado propio; delega completamente a los singletons.
// ---------------------------------------------------------------------------
class GamepadInputAdapter {
public:
    // Retorna true si teclado O mando activan la acción.
    // joystickId permite manejar multijugador local (por defecto, mando 0).
    static bool isPressed(const std::string& action, unsigned int joystickId = 0) {
        return KeyBindings::getInstance().isPressed(action)
            || GamepadManager::getInstance().isPressed(action, joystickId);
    }

    // Comprueba si un evento puntual (KeyPressed o JoystickButtonPressed)
    // corresponde a la acción indicada. Útil para menus que procesan eventos
    // discretos en lugar de polling continuo.
    static bool matchesEvent(const sf::Event& event, const std::string& action) {
        if (event.type == sf::Event::KeyPressed)
            return event.key.code == KeyBindings::getInstance().get(action);

        if (event.type == sf::Event::JoystickButtonPressed) {
            const auto* b = GamepadManager::getInstance().getBinding(action);
            return b
                && b->type == GamepadBinding::Type::Button
                && (int)event.joystickButton.button == b->index;
        }

        return false;
    }
};

#endif
