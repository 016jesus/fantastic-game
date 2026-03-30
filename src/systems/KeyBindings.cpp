#include "systems/KeyBindings.h"
#include <fstream>
#include <string>

// ---------------------------------------------------------------------------
// Singleton — instancia estática garantizada una sola vez (C++11)
// ---------------------------------------------------------------------------
KeyBindings& KeyBindings::getInstance() {
    static KeyBindings instance;
    return instance;
}

// ---------------------------------------------------------------------------
// resetToDefaults: define el mapeo base de acciones del juego.
// Para añadir nuevas acciones basta con agregar una línea aquí.
// ---------------------------------------------------------------------------
void KeyBindings::resetToDefaults() {
    bindings["moveLeft"]   = sf::Keyboard::A;
    bindings["moveRight"]  = sf::Keyboard::D;
    bindings["jump"]       = sf::Keyboard::Space;
    bindings["attack"]     = sf::Keyboard::J;
    bindings["interact"]   = sf::Keyboard::E;
    bindings["inventory"]  = sf::Keyboard::I;
    bindings["pause"]      = sf::Keyboard::Escape;
}

// ---------------------------------------------------------------------------
// get: retorna la tecla asociada a una acción.
// Si la acción no existe retorna sf::Keyboard::Unknown.
// ---------------------------------------------------------------------------
sf::Keyboard::Key KeyBindings::get(const std::string& action) const {
    auto it = bindings.find(action);
    if (it == bindings.end()) return sf::Keyboard::Unknown;
    return it->second;
}

// ---------------------------------------------------------------------------
// set: asigna o actualiza la tecla para una acción.
// ---------------------------------------------------------------------------
void KeyBindings::set(const std::string& action, sf::Keyboard::Key key) {
    bindings[action] = key;
}

// ---------------------------------------------------------------------------
// isPressed: consulta en tiempo real si la tecla de la acción está pulsada.
// ---------------------------------------------------------------------------
bool KeyBindings::isPressed(const std::string& action) const {
    return sf::Keyboard::isKeyPressed(get(action));
}

// ---------------------------------------------------------------------------
// getAll: expone el mapa completo (solo lectura) para el widget de remapeo.
// ---------------------------------------------------------------------------
const std::unordered_map<std::string, sf::Keyboard::Key>& KeyBindings::getAll() const {
    return bindings;
}

// ---------------------------------------------------------------------------
// save: escribe el fichero de keybindings.
// Formato por línea: action=keyCode  (keyCode es el valor int de la enum)
// ---------------------------------------------------------------------------
bool KeyBindings::save(const std::string& path) const {
    std::ofstream f(path);
    if (!f.is_open()) return false;

    for (const auto& kv : bindings) {
        f << kv.first << "=" << static_cast<int>(kv.second) << "\n";
    }

    return f.good();
}

// ---------------------------------------------------------------------------
// load: parsea el fichero de keybindings.
// Si el fichero no existe o está corrupto, mantiene los defaults ya cargados
// en el constructor (no llama a resetToDefaults() para no borrar un remapeo
// parcialmente válido; si se desea un reset completo se llama explícitamente).
// ---------------------------------------------------------------------------
bool KeyBindings::load(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return false;

    std::string line;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;

        auto sep = line.find('=');
        if (sep == std::string::npos) continue;

        std::string action = line.substr(0, sep);
        std::string val    = line.substr(sep + 1);

        try {
            int code = std::stoi(val);
            bindings[action] = static_cast<sf::Keyboard::Key>(code);
        } catch (...) {
            // Línea malformada — se ignora y se conserva el default
        }
    }

    return true;
}
