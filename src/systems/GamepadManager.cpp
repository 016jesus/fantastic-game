#include "systems/GamepadManager.h"
#include <fstream>
#include <sstream>
#include <string>

// ---------------------------------------------------------------------------
// Singleton — instancia estática garantizada una sola vez (C++11)
// ---------------------------------------------------------------------------
GamepadManager& GamepadManager::getInstance() {
    static GamepadManager instance;
    return instance;
}

// ---------------------------------------------------------------------------
// resetToDefaults: mapeo estándar para un mando tipo Xbox/genérico.
//
// Ejes sf::Joystick:
//   X (0) — palanca izquierda horizontal
//   Y (1) — palanca izquierda vertical
// Botones:
//   0 = A / Cruz    — saltar
//   1 = B / Círculo — interactuar
//   2 = X / Cuadrado— atacar
//   3 = Y / Triángulo— inventario
//   7 = Start       — pausa
// ---------------------------------------------------------------------------
void GamepadManager::resetToDefaults() {
    bind("moveLeft",  {GamepadBinding::Type::AxisNegative, (int)sf::Joystick::X,  50.f});
    bind("moveRight", {GamepadBinding::Type::AxisPositive, (int)sf::Joystick::X,  50.f});
    bind("jump",      {GamepadBinding::Type::Button,       0,                      50.f});
    bind("attack",    {GamepadBinding::Type::Button,       2,                      50.f});
    bind("interact",  {GamepadBinding::Type::Button,       1,                      50.f});
    bind("inventory", {GamepadBinding::Type::Button,       3,                      50.f});
    bind("pause",     {GamepadBinding::Type::Button,       7,                      50.f});
}

// ---------------------------------------------------------------------------
// update: refresca el estado interno de todos los joysticks conectados.
// Debe llamarse una vez por frame, antes de cualquier consulta isPressed().
// ---------------------------------------------------------------------------
void GamepadManager::update() {
    sf::Joystick::update();
}

// ---------------------------------------------------------------------------
// bind / getBinding / getAll
// ---------------------------------------------------------------------------
void GamepadManager::bind(const std::string& action, GamepadBinding binding) {
    bindings[action] = binding;
}

const GamepadBinding* GamepadManager::getBinding(const std::string& action) const {
    auto it = bindings.find(action);
    if (it == bindings.end()) return nullptr;
    return &it->second;
}

const std::unordered_map<std::string, GamepadBinding>& GamepadManager::getAll() const {
    return bindings;
}

// ---------------------------------------------------------------------------
// isPressed: comprueba si la acción está activa en el joystick indicado.
//
// Lógica de evaluación por tipo:
//   Button       — sf::Joystick::isButtonPressed()
//   AxisPositive — eje > +threshold
//   AxisNegative — eje < -threshold  (el eje devuelve −100..100)
//
// Retorna false de forma segura si el joystick no está conectado o la acción
// no tiene binding registrado.
// ---------------------------------------------------------------------------
bool GamepadManager::isPressed(const std::string& action, unsigned int joystickId) const {
    if (!sf::Joystick::isConnected(joystickId)) return false;

    const GamepadBinding* b = getBinding(action);
    if (!b) return false;

    switch (b->type) {
    case GamepadBinding::Type::Button:
        return sf::Joystick::isButtonPressed(joystickId,
                                             static_cast<unsigned int>(b->index));

    case GamepadBinding::Type::AxisPositive: {
        const float val = sf::Joystick::getAxisPosition(
            joystickId, static_cast<sf::Joystick::Axis>(b->index));
        return val > b->threshold;
    }

    case GamepadBinding::Type::AxisNegative: {
        const float val = sf::Joystick::getAxisPosition(
            joystickId, static_cast<sf::Joystick::Axis>(b->index));
        return val < -b->threshold;
    }
    }

    return false;
}

// ---------------------------------------------------------------------------
// isConnected / connectedCount
// ---------------------------------------------------------------------------
bool GamepadManager::isConnected(unsigned int id) const {
    return sf::Joystick::isConnected(id);
}

int GamepadManager::connectedCount() const {
    int count = 0;
    for (unsigned int i = 0; i < sf::Joystick::Count; ++i) {
        if (sf::Joystick::isConnected(i)) ++count;
    }
    return count;
}

// ---------------------------------------------------------------------------
// getAxisValue: valor crudo del eje en el rango −100..100.
// ---------------------------------------------------------------------------
float GamepadManager::getAxisValue(unsigned int joystickId, int axis) const {
    return sf::Joystick::getAxisPosition(joystickId,
                                         static_cast<sf::Joystick::Axis>(axis));
}

// ---------------------------------------------------------------------------
// getConnectedNames: genera etiquetas descriptivas para la UI.
// sf::Joystick no expone nombres de dispositivo en SFML 2.x, por lo que se
// usa "Joystick N" como identificador único y suficiente para la tab de mando.
// ---------------------------------------------------------------------------
std::vector<std::string> GamepadManager::getConnectedNames() const {
    std::vector<std::string> names;
    for (unsigned int i = 0; i < sf::Joystick::Count; ++i) {
        if (sf::Joystick::isConnected(i)) {
            names.push_back("Joystick " + std::to_string(i));
        }
    }
    return names;
}

// ---------------------------------------------------------------------------
// save: escribe el fichero de bindings de mando.
//
// Formato por línea:
//   action=type,index,threshold
// donde type es 0=Button, 1=AxisPositive, 2=AxisNegative.
// ---------------------------------------------------------------------------
bool GamepadManager::save(const std::string& path) const {
    std::ofstream f(path);
    if (!f.is_open()) return false;

    for (const auto& kv : bindings) {
        f << kv.first << "="
          << static_cast<int>(kv.second.type)  << ","
          << kv.second.index                   << ","
          << kv.second.threshold               << "\n";
    }

    return f.good();
}

// ---------------------------------------------------------------------------
// load: parsea el fichero de bindings de mando.
//
// Si el fichero no existe o una línea está malformada, se conservan los
// defaults cargados en el constructor sin resetear el resto del mapa.
// ---------------------------------------------------------------------------
bool GamepadManager::load(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return false;

    std::string line;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;

        const auto sep = line.find('=');
        if (sep == std::string::npos) continue;

        const std::string action = line.substr(0, sep);
        const std::string rest   = line.substr(sep + 1);

        // Parseo de "type,index,threshold"
        std::istringstream ss(rest);
        std::string tok;
        try {
            int typeInt = 0, idx = 0;
            float thr = 50.f;

            if (!std::getline(ss, tok, ',')) continue;
            typeInt = std::stoi(tok);

            if (!std::getline(ss, tok, ',')) continue;
            idx = std::stoi(tok);

            if (std::getline(ss, tok, ',')) {
                thr = std::stof(tok);
            }

            GamepadBinding b;
            b.type      = static_cast<GamepadBinding::Type>(typeInt);
            b.index     = idx;
            b.threshold = thr;
            bindings[action] = b;

        } catch (...) {
            // Línea malformada — se ignora, se conserva el default
        }
    }

    return true;
}
