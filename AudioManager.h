#pragma once
#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H
#include "ISettingsObserver.h"
#include <SFML/Audio.hpp>
#include <vector>

// ---------------------------------------------------------------------------
// AudioManager: singleton que controla los volúmenes globales del juego y
// aplica cambios a todas las instancias de sf::Music registradas.
// Implementa ISettingsObserver para reaccionar automáticamente a cambios en
// GameSettings sin necesidad de polling.
// ---------------------------------------------------------------------------
class AudioManager : public ISettingsObserver {
public:
    static AudioManager& getInstance();

    // ISettingsObserver
    void onSettingChanged(const std::string& key, float value) override;

    // Registro de música activa (puntero no-owning — el caller gestiona lifetime)
    void registerMusic(sf::Music* music);
    void unregisterMusic(sf::Music* music);

    // Aplica los volúmenes actuales a todas las instancias registradas
    void applyVolumes();

    float getMasterVolume() const;
    float getMusicVolume()  const;
    float getSfxVolume()    const;

private:
    AudioManager();
    // Singleton — sin copias ni asignaciones
    AudioManager(const AudioManager&)            = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    std::vector<sf::Music*> activeMusics;

    float masterVolume = 0.8f;
    float musicVolume  = 0.6f;
    float sfxVolume    = 1.0f;

    // Volumen efectivo de música = master * music (ambos en [0,1])
    float effectiveMusicVolume() const;
};
#endif
