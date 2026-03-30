#pragma once
#ifndef SOUND_TAB_H
#define SOUND_TAB_H
#include "ITab.h"
#include "Label.h"
#include "Slider.h"
#include "Toggle.h"
#include <SFML/Graphics.hpp>
#include <memory>

// ---------------------------------------------------------------------------
// SoundTab: pestaña de configuración de audio inyectable en OptionsState.
// Controla masterVolume, musicVolume y sfxVolume via GameSettings; el
// AudioManager reacciona automáticamente a través del patrón Observer.
// ---------------------------------------------------------------------------
class SoundTab : public ITab {
public:
    explicit SoundTab(const sf::Font& font);

    std::string getTitle() const override { return "Sonido"; }

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    // Sincroniza sliders con los valores actuales de GameSettings
    void onActivate() override;

private:
    Label  lblMaster;
    Label  lblMusic;
    Label  lblSfx;
    Label  lblMute;

    Slider sliderMaster;
    Slider sliderMusic;
    Slider sliderSfx;

    Toggle toggleMute;

    sf::RectangleShape panel;
    const sf::Font&    font;

    // Guarda el volumen maestro antes de silenciar para poder restaurarlo
    float savedMasterVolume = 0.8f;

    void buildLayout();
};
#endif
