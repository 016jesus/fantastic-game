#pragma once
#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H
#include "ISettingsObserver.h"
#include <unordered_map>
#include <vector>
#include <string>

// Almacén central de configuración del juego.
// Principio Open/Closed: se extiende añadiendo observadores o nuevas claves,
// sin modificar la clase. Singleton de Meyer (thread-safe desde C++11).
class GameSettings {
public:
    static GameSettings& getInstance();

    // -----------------------------------------------------------------
    // Getters / Setters genéricos por clave
    // -----------------------------------------------------------------
    void        setFloat(const std::string& key, float value);
    float       getFloat(const std::string& key, float defaultValue = 0.f) const;

    void        setBool(const std::string& key, bool value);
    bool        getBool(const std::string& key, bool defaultValue = false) const;

    void        setString(const std::string& key, const std::string& value);
    std::string getString(const std::string& key, const std::string& def = "") const;

    // -----------------------------------------------------------------
    // Persistencia (secciones [float], [bool], [string] en el fichero)
    // -----------------------------------------------------------------
    bool save(const std::string& path = "settings.cfg") const;
    bool load(const std::string& path = "settings.cfg");

    // -----------------------------------------------------------------
    // Patrón Observer
    // -----------------------------------------------------------------
    void addObserver(ISettingsObserver* obs);
    void removeObserver(ISettingsObserver* obs);

    // Inicializa con valores por defecto. Se llama en el constructor y
    // también al inicio de load() para preservar claves no guardadas.
    void loadDefaults();

private:
    GameSettings() { loadDefaults(); }

    // Notifica a todos los observadores registrados de un cambio de valor.
    void notifyObservers(const std::string& key, float value);

    std::unordered_map<std::string, float>       floats;
    std::unordered_map<std::string, bool>        bools;
    std::unordered_map<std::string, std::string> strings;
    std::vector<ISettingsObserver*>              observers;
};
#endif
