#pragma once
#ifndef GAMEPAD_MANAGER_H
#define GAMEPAD_MANAGER_H
#include <SFML/Window/Joystick.hpp>
#include <unordered_map>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// GamepadBinding: describe cómo se mapea una acción a un input de mando.
// Type::Button      — botón digital (isButtonPressed)
// Type::AxisPositive — eje analógico cuyo valor positivo supera threshold
// Type::AxisNegative — eje analógico cuyo valor negativo supera threshold
// ---------------------------------------------------------------------------
struct GamepadBinding {
    enum class Type { Button, AxisPositive, AxisNegative };
    Type  type      = Type::Button;
    int   index     = 0;
    float threshold = 50.f;
};

// ---------------------------------------------------------------------------
// GamepadManager: singleton que abstrae sf::Joystick con el mismo API de
// acción-lógica que KeyBindings, permitiendo que GamepadInputAdapter combine
// ambas fuentes de input de forma transparente.
//
// Principio Open/Closed: para añadir nuevas acciones basta llamar a bind()
// o ampliar resetToDefaults() sin modificar la interfaz pública.
// ---------------------------------------------------------------------------
class GamepadManager {
public:
    static GamepadManager& getInstance();

    // Debe llamarse una vez por frame antes de consultar inputs
    void update();

    // Retorna true si la acción está activa en el joystick indicado
    bool isPressed(const std::string& action, unsigned int joystickId = 0) const;

    // Asigna un binding a una acción (crea o sobreescribe)
    void bind(const std::string& action, GamepadBinding binding);

    // Retorna el binding de una acción, o nullptr si no existe
    const GamepadBinding* getBinding(const std::string& action) const;

    // Estado de conexión
    bool isConnected(unsigned int id = 0) const;
    int  connectedCount() const;

    // Restaura el mapeo por defecto del juego
    void resetToDefaults();

    // Persistencia — mismo formato texto que KeyBindings para coherencia
    bool save(const std::string& path = "gamepad.cfg") const;
    bool load(const std::string& path = "gamepad.cfg");

    // Valor crudo del eje (−100..100)
    float getAxisValue(unsigned int joystickId, int axis) const;

    // Nombres descriptivos de los mandos conectados
    std::vector<std::string> getConnectedNames() const;

    // Expone el mapa completo (solo lectura) para GamepadTab
    const std::unordered_map<std::string, GamepadBinding>& getAll() const;

private:
    GamepadManager() { resetToDefaults(); }

    std::unordered_map<std::string, GamepadBinding> bindings;
};

#endif
