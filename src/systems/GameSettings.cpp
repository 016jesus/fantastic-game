#include "systems/GameSettings.h"
#include <fstream>
#include <sstream>
#include <algorithm>

// ---------------------------------------------------------------------------
// Singleton — instancia estática garantizada una sola vez (C++11)
// ---------------------------------------------------------------------------
GameSettings& GameSettings::getInstance() {
    static GameSettings instance;
    return instance;
}

// ---------------------------------------------------------------------------
// loadDefaults: establece valores por defecto para todas las claves conocidas.
// Se llama desde el constructor y al inicio de load() para no perder claves
// que no estén presentes en el fichero guardado.
// ---------------------------------------------------------------------------
void GameSettings::loadDefaults() {
    // --- Volúmenes y ajustes de pantalla ---
    floats["masterVolume"] = 0.8f;
    floats["musicVolume"]  = 0.6f;
    floats["sfxVolume"]    = 1.0f;
    floats["brightness"]   = 1.0f;

    // --- Opciones booleanas ---
    bools["fullscreen"] = false;
    bools["vsync"]      = false;

    // --- Cadenas de texto ---
    strings["resolution"] = "512x256";
    strings["language"]   = "es";
}

// ---------------------------------------------------------------------------
// notifyObservers: itera el vector y notifica a cada observer registrado.
// ---------------------------------------------------------------------------
void GameSettings::notifyObservers(const std::string& key, float value) {
    for (ISettingsObserver* obs : observers) {
        if (obs) obs->onSettingChanged(key, value);
    }
}

// ---------------------------------------------------------------------------
// Getters / Setters — float
// ---------------------------------------------------------------------------
void GameSettings::setFloat(const std::string& key, float value) {
    floats[key] = value;
    notifyObservers(key, value);
}

float GameSettings::getFloat(const std::string& key, float defaultValue) const {
    auto it = floats.find(key);
    if (it == floats.end()) return defaultValue;
    return it->second;
}

// ---------------------------------------------------------------------------
// Getters / Setters — bool
// ---------------------------------------------------------------------------
void GameSettings::setBool(const std::string& key, bool value) {
    bools[key] = value;
    // Convierte el bool a float para notificar a través de la interfaz común
    notifyObservers(key, value ? 1.0f : 0.0f);
}

bool GameSettings::getBool(const std::string& key, bool defaultValue) const {
    auto it = bools.find(key);
    if (it == bools.end()) return defaultValue;
    return it->second;
}

// ---------------------------------------------------------------------------
// Getters / Setters — string
// ---------------------------------------------------------------------------
void GameSettings::setString(const std::string& key, const std::string& value) {
    strings[key] = value;
    // Las cadenas no tienen representación float directa; no se notifica
    // (los observers reciben solo cambios numéricos según la interfaz)
}

std::string GameSettings::getString(const std::string& key,
                                    const std::string& def) const {
    auto it = strings.find(key);
    if (it == strings.end()) return def;
    return it->second;
}

// ---------------------------------------------------------------------------
// Observer — registro y baja
// ---------------------------------------------------------------------------
void GameSettings::addObserver(ISettingsObserver* obs) {
    if (!obs) return;
    observers.push_back(obs);
}

void GameSettings::removeObserver(ISettingsObserver* obs) {
    observers.erase(
        std::remove(observers.begin(), observers.end(), obs),
        observers.end()
    );
}

// ---------------------------------------------------------------------------
// save: escribe el fichero de configuración con secciones [float], [bool]
// y [string]. Formato por línea: key=value
// ---------------------------------------------------------------------------
bool GameSettings::save(const std::string& path) const {
    std::ofstream f(path);
    if (!f.is_open()) return false;

    f << "[float]\n";
    for (const auto& kv : floats) {
        f << kv.first << "=" << kv.second << "\n";
    }

    f << "[bool]\n";
    for (const auto& kv : bools) {
        f << kv.first << "=" << (kv.second ? "1" : "0") << "\n";
    }

    f << "[string]\n";
    for (const auto& kv : strings) {
        f << kv.first << "=" << kv.second << "\n";
    }

    return f.good();
}

// ---------------------------------------------------------------------------
// load: parsea el fichero de configuración.
// Llama a loadDefaults() primero para no perder claves no guardadas.
// Retorna false si el fichero no se puede abrir.
// ---------------------------------------------------------------------------
bool GameSettings::load(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return false;

    // Primero restauramos defaults para que las claves ausentes en el fichero
    // conserven sus valores por defecto.
    loadDefaults();

    // Sección activa durante el parseo
    enum class Section { None, Float, Bool, String };
    Section current = Section::None;

    std::string line;
    while (std::getline(f, line)) {
        // Ignora líneas vacías y comentarios (#)
        if (line.empty() || line[0] == '#') continue;

        // Detecta cambio de sección
        if (line == "[float]")  { current = Section::Float;  continue; }
        if (line == "[bool]")   { current = Section::Bool;   continue; }
        if (line == "[string]") { current = Section::String; continue; }

        // Parsea key=value
        auto sep = line.find('=');
        if (sep == std::string::npos) continue;

        std::string key = line.substr(0, sep);
        std::string val = line.substr(sep + 1);

        switch (current) {
            case Section::Float:
                try { floats[key] = std::stof(val); } catch (...) {}
                break;
            case Section::Bool:
                bools[key] = (val == "1" || val == "true");
                break;
            case Section::String:
                strings[key] = val;
                break;
            default:
                break;
        }
    }

    return true;
}
