#include "ui/tabs/SoundTab.h"
#include "systems/GameSettings.h"

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
SoundTab::SoundTab(const sf::Font& font)
    : font(font)
{
    buildLayout();
}

// ---------------------------------------------------------------------------
// buildLayout: crea y posiciona todos los widgets.
//
// Coordenadas de referencia:
//   - El panel ocupa (0,24)→(512,246) — debajo del TabBar.
//   - Labels en x=10, sliders/toggles en x=190, ancho ~140px.
//   - Filas: y = 50, 90, 130, 170 (paso de 40px para sliders más grandes).
// ---------------------------------------------------------------------------
void SoundTab::buildLayout() {
    // Panel de fondo
    panel.setSize(sf::Vector2f(512.f, 222.f));
    panel.setFillColor(sf::Color(20, 20, 30));
    panel.setPosition(0.f, 24.f);

    constexpr float LABEL_X   = 10.f;
    constexpr float WIDGET_X  = 190.f;
    constexpr float WIDGET_W  = 140.f;
    constexpr float WIDGET_H  = 20.f;
    constexpr unsigned LABEL_SIZE = 11u;

    // -----------------------------------------------------------------------
    // Fila 1 — Volumen maestro (y=50)
    // -----------------------------------------------------------------------
    lblMaster = Label("Vol. Maestro", font, LABEL_SIZE, sf::Color::White);
    lblMaster.setPosition(LABEL_X, 50.f);
    lblMaster.setSize(170.f, WIDGET_H);

    sliderMaster.setRange(0.f, 1.f);
    sliderMaster.setValue(0.8f);
    sliderMaster.setPosition(WIDGET_X, 50.f);
    sliderMaster.setSize(WIDGET_W, WIDGET_H);
    sliderMaster.setOnValueChanged([this](float value) {
        // Al mover el slider maestro manualmente, desactiva el mute
        toggleMute.setValue(false);
        savedMasterVolume = value;
        GameSettings::getInstance().setFloat("masterVolume", value);
    });

    // -----------------------------------------------------------------------
    // Fila 2 — Volumen de música (y=90)
    // -----------------------------------------------------------------------
    lblMusic = Label("Musica", font, LABEL_SIZE, sf::Color::White);
    lblMusic.setPosition(LABEL_X, 90.f);
    lblMusic.setSize(170.f, WIDGET_H);

    sliderMusic.setRange(0.f, 1.f);
    sliderMusic.setValue(0.6f);
    sliderMusic.setPosition(WIDGET_X, 90.f);
    sliderMusic.setSize(WIDGET_W, WIDGET_H);
    sliderMusic.setOnValueChanged([](float value) {
        GameSettings::getInstance().setFloat("musicVolume", value);
    });

    // -----------------------------------------------------------------------
    // Fila 3 — Volumen de efectos (y=130)
    // -----------------------------------------------------------------------
    lblSfx = Label("Efectos", font, LABEL_SIZE, sf::Color::White);
    lblSfx.setPosition(LABEL_X, 130.f);
    lblSfx.setSize(170.f, WIDGET_H);

    sliderSfx.setRange(0.f, 1.f);
    sliderSfx.setValue(1.f);
    sliderSfx.setPosition(WIDGET_X, 130.f);
    sliderSfx.setSize(WIDGET_W, WIDGET_H);
    sliderSfx.setOnValueChanged([](float value) {
        GameSettings::getInstance().setFloat("sfxVolume", value);
    });

    // -----------------------------------------------------------------------
    // Fila 4 — Silenciar todo (y=170)
    // -----------------------------------------------------------------------
    lblMute = Label("Silenciar todo", font, LABEL_SIZE, sf::Color::White);
    lblMute.setPosition(LABEL_X, 170.f);
    lblMute.setSize(170.f, WIDGET_H);

    toggleMute.setLabel("Silenciar", font, LABEL_SIZE);
    toggleMute.setPosition(WIDGET_X, 170.f);
    toggleMute.setSize(WIDGET_W, WIDGET_H);
    toggleMute.setOnChanged([this](bool on) {
        if (on) {
            // Guarda el volumen actual antes de silenciar
            savedMasterVolume = GameSettings::getInstance().getFloat("masterVolume", 0.8f);
            GameSettings::getInstance().setFloat("masterVolume", 0.f);
            // Refleja el cambio en el slider sin disparar el callback del slider
            sliderMaster.setValue(0.f);
        } else {
            // Restaura el volumen anterior
            GameSettings::getInstance().setFloat("masterVolume", savedMasterVolume);
            sliderMaster.setValue(savedMasterVolume);
        }
    });
}

// ---------------------------------------------------------------------------
// onActivate: sincroniza los widgets con los valores actuales de GameSettings
// ---------------------------------------------------------------------------
void SoundTab::onActivate() {
    GameSettings& cfg = GameSettings::getInstance();

    const float master = cfg.getFloat("masterVolume", 0.8f);
    const float music  = cfg.getFloat("musicVolume",  0.6f);
    const float sfx    = cfg.getFloat("sfxVolume",    1.0f);

    sliderMaster.setValue(master);
    sliderMusic.setValue(music);
    sliderSfx.setValue(sfx);

    // Refleja si el juego está actualmente silenciado
    const bool muted = (master <= 0.f);
    toggleMute.setValue(muted);

    // Guarda el volumen "real" sólo si no está silenciado
    if (!muted) {
        savedMasterVolume = master;
    }
}

// ---------------------------------------------------------------------------
// handleEvent
// ---------------------------------------------------------------------------
void SoundTab::handleEvent(const sf::Event& event) {
    sliderMaster.handleEvent(event);
    sliderMusic.handleEvent(event);
    sliderSfx.handleEvent(event);
    toggleMute.handleEvent(event);
}

// ---------------------------------------------------------------------------
// update
// ---------------------------------------------------------------------------
void SoundTab::update(float dt) {
    sliderMaster.update(dt);
    sliderMusic.update(dt);
    sliderSfx.update(dt);
    toggleMute.update(dt);
}

// ---------------------------------------------------------------------------
// draw
// ---------------------------------------------------------------------------
void SoundTab::draw(sf::RenderWindow& window) {
    window.draw(panel);

    lblMaster.draw(window);
    lblMusic.draw(window);
    lblSfx.draw(window);
    lblMute.draw(window);

    sliderMaster.draw(window);
    sliderMusic.draw(window);
    sliderSfx.draw(window);
    toggleMute.draw(window);
}
