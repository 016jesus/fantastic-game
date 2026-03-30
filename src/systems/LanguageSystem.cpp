#include "systems/LanguageSystem.h"
#include <fstream>
#include <sstream>

// ---------------------------------------------------------------------------
// Singleton de Meyer — instancia única, destruida al finalizar el programa.
// ---------------------------------------------------------------------------
LanguageSystem& LanguageSystem::getInstance() {
    static LanguageSystem instance;
    return instance;
}

// ---------------------------------------------------------------------------
// load: abre el fichero en la ruta indicada y rellena el mapa de strings.
// Ignora líneas vacías y las que empiezan con '#'.
// Retorna false si el fichero no puede abrirse.
// ---------------------------------------------------------------------------
bool LanguageSystem::load(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return false;

    strings.clear();

    std::string line;
    while (std::getline(f, line)) {
        // Ignorar líneas vacías o comentarios
        if (line.empty() || line[0] == '#') continue;

        auto eq = line.find('=');
        if (eq == std::string::npos) continue;

        std::string key   = line.substr(0, eq);
        std::string value = line.substr(eq + 1);
        strings[key] = value;
    }
    return true;
}

// ---------------------------------------------------------------------------
// get: devuelve la cadena traducida para la clave dada.
// Si la clave no existe retorna un fallback de la forma "[clave]".
// Se usa un miembro mutable para poder retornar una referencia constante.
// ---------------------------------------------------------------------------
const std::string& LanguageSystem::get(const std::string& key) const {
    auto it = strings.find(key);
    if (it != strings.end()) return it->second;

    // Fallback visible: [clave] en lugar de cadena vacía
    fallback = "[" + key + "]";
    return fallback;
}

// ---------------------------------------------------------------------------
// setLanguage: cambia el idioma activo cargando el fichero <langCode>.lang.
// Guarda el código aunque el fichero no exista (permite reintentar después).
// ---------------------------------------------------------------------------
bool LanguageSystem::setLanguage(const std::string& langCode) {
    currentLang = langCode;
    return load(langCode + ".lang");
}

// ---------------------------------------------------------------------------
// currentLanguage: devuelve el código del idioma actualmente cargado.
// ---------------------------------------------------------------------------
std::string LanguageSystem::currentLanguage() const {
    return currentLang;
}
