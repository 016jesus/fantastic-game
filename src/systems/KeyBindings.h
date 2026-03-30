#pragma once
#ifndef KEY_BINDINGS_H
#define KEY_BINDINGS_H
#include <SFML/Window/Keyboard.hpp>
#include <unordered_map>
#include <string>

// Mapeo de acciones lógicas a teclas físicas.
// Principio Open/Closed: se añaden nuevas acciones llamando a set() o
// ampliando resetToDefaults() sin modificar la interfaz pública.
// Singleton de Meyer (thread-safe desde C++11).
class KeyBindings {
public:
    static KeyBindings& getInstance();

    // Obtiene la tecla asignada a una acción.
    // Retorna sf::Keyboard::Unknown si la acción no existe.
    sf::Keyboard::Key get(const std::string& action) const;

    // Asigna una tecla a una acción (crea la entrada si no existe).
    void set(const std::string& action, sf::Keyboard::Key key);

    // Comprueba en tiempo real si la tecla de una acción está pulsada.
    // Usa sf::Keyboard::isKeyPressed internamente.
    bool isPressed(const std::string& action) const;

    // Devuelve el mapa completo (para el widget de remapeo de teclas).
    const std::unordered_map<std::string, sf::Keyboard::Key>& getAll() const;

    // -----------------------------------------------------------------
    // Persistencia
    // -----------------------------------------------------------------
    bool save(const std::string& path = "keybindings.cfg") const;
    bool load(const std::string& path = "keybindings.cfg");

    // Restaura el mapeo de teclas a los valores por defecto del juego.
    void resetToDefaults();

private:
    KeyBindings() { resetToDefaults(); }

    std::unordered_map<std::string, sf::Keyboard::Key> bindings;
};
#endif
