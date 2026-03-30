#pragma once
#ifndef GRAPHICS_TAB_H
#define GRAPHICS_TAB_H
#include "ITab.h"
#include "Label.h"
#include "Toggle.h"
#include "Dropdown.h"
#include "Slider.h"
#include "Button.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// ---------------------------------------------------------------------------
// GraphicsTab: pestaña de configuración gráfica inyectable en OptionsState.
// Extiende ITab; OptionsState nunca necesita conocer esta clase directamente.
// ---------------------------------------------------------------------------
class GraphicsTab : public ITab {
public:
    explicit GraphicsTab(const sf::Font& font);

    std::string getTitle() const override { return "Graficas"; }

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    // Sincroniza widgets con los valores actuales de GameSettings
    void onActivate() override;

private:
    // Etiquetas de cada fila
    Label lblResolution;
    Label lblFullscreen;
    Label lblVSync;
    Label lblFPS;
    Label lblBrightness;

    // Controles de cada fila
    Dropdown dropResolution;
    Toggle   toggleFullscreen;
    Toggle   toggleVSync;
    Dropdown dropFPS;
    Slider   sliderBrightness;

    // Botón de aplicar (resolución y fullscreen requieren recrear ventana)
    Button btnApply;

    sf::RectangleShape panel;
    const sf::Font&    font;

    void buildLayout();
    void applySettings();
};
#endif
