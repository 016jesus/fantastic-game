#pragma once
#ifndef ISETTINGS_OBSERVER_H
#define ISETTINGS_OBSERVER_H
#include <string>

// Interfaz Observer para GameSettings.
// Cualquier sistema que necesite reaccionar a cambios de configuración
// (ej. AudioManager, RenderSystem) implementa esta interfaz y se registra
// en GameSettings::addObserver().
class ISettingsObserver {
public:
    virtual ~ISettingsObserver() = default;

    // Llamado cuando cambia cualquier configuración.
    // key   — identificador de la configuración cambiada ("masterVolume", etc.)
    // value — nuevo valor convertido a float (los bool se convierten a 0.0/1.0)
    virtual void onSettingChanged(const std::string& key, float value) = 0;
};
#endif
