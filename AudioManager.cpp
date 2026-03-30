#include "AudioManager.h"
#include "GameSettings.h"
#include <algorithm>

// ---------------------------------------------------------------------------
// getInstance: instancia Meyer garantizada thread-safe desde C++11
// ---------------------------------------------------------------------------
AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

// ---------------------------------------------------------------------------
// Constructor: se registra como observer de GameSettings y sincroniza
// los volúmenes miembro con los valores ya presentes en la configuración.
// ---------------------------------------------------------------------------
AudioManager::AudioManager() {
    GameSettings& settings = GameSettings::getInstance();

    // Sincroniza valores iniciales desde GameSettings (pueden haber sido
    // cargados desde fichero antes de que este singleton exista)
    masterVolume = settings.getFloat("masterVolume", 0.8f);
    musicVolume  = settings.getFloat("musicVolume",  0.6f);
    sfxVolume    = settings.getFloat("sfxVolume",    1.0f);

    // Registro como observer para recibir cambios futuros
    settings.addObserver(this);

    applyVolumes();
}

// ---------------------------------------------------------------------------
// onSettingChanged: actualiza el miembro correspondiente y re-aplica volúmenes
// ---------------------------------------------------------------------------
void AudioManager::onSettingChanged(const std::string& key, float value) {
    if (key == "masterVolume") {
        masterVolume = value;
        applyVolumes();
    } else if (key == "musicVolume") {
        musicVolume = value;
        applyVolumes();
    } else if (key == "sfxVolume") {
        sfxVolume = value;
        // sfxVolume afecta a sf::Sound instances; las Music usan musicVolume
        // Se propaga a través de applyVolumes para futura extensión
        applyVolumes();
    }
}

// ---------------------------------------------------------------------------
// registerMusic / unregisterMusic — gestión de la lista no-owning
// ---------------------------------------------------------------------------
void AudioManager::registerMusic(sf::Music* music) {
    if (!music) return;
    // Evita duplicados
    auto it = std::find(activeMusics.begin(), activeMusics.end(), music);
    if (it == activeMusics.end()) {
        activeMusics.push_back(music);
        // Aplica volumen inmediatamente al registrar
        music->setVolume(effectiveMusicVolume() * 100.f);
    }
}

void AudioManager::unregisterMusic(sf::Music* music) {
    activeMusics.erase(
        std::remove(activeMusics.begin(), activeMusics.end(), music),
        activeMusics.end()
    );
}

// ---------------------------------------------------------------------------
// applyVolumes: propaga el volumen efectivo a todas las músicas registradas.
// SFML usa rango [0, 100] mientras que internamente trabajamos en [0, 1].
// ---------------------------------------------------------------------------
void AudioManager::applyVolumes() {
    const float vol = effectiveMusicVolume() * 100.f;
    for (sf::Music* music : activeMusics) {
        if (music) music->setVolume(vol);
    }
}

// ---------------------------------------------------------------------------
// Getters
// ---------------------------------------------------------------------------
float AudioManager::getMasterVolume() const { return masterVolume; }
float AudioManager::getMusicVolume()  const { return musicVolume;  }
float AudioManager::getSfxVolume()    const { return sfxVolume;    }

// ---------------------------------------------------------------------------
// effectiveMusicVolume: master * music, ambos normalizados a [0,1]
// ---------------------------------------------------------------------------
float AudioManager::effectiveMusicVolume() const {
    return masterVolume * musicVolume;
}
