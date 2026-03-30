#pragma once
#ifndef GAMEPAD_TAB_H
#define GAMEPAD_TAB_H
#include "ui/tabs/ITab.h"
#include "ui/widgets/Label.h"
#include "ui/widgets/Button.h"
#include "ui/widgets/Toggle.h"
#include <SFML/Graphics.hpp>
#include <vector>

// ---------------------------------------------------------------------------
// GamepadTab: pestaña del menú de opciones para configuración de mando.
// Muestra el estado de conexión, permite activar/desactivar el soporte de
// mando y restaurar el mapeo de botones a los valores por defecto.
// ---------------------------------------------------------------------------
class GamepadTab : public ITab {
public:
    explicit GamepadTab(const sf::Font& font);

    std::string getTitle() const override { return "Mando"; }

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void onActivate() override;

private:
    Label  lblStatus;       // "Mandos conectados: N"
    Toggle toggleEnabled;   // Activar/desactivar soporte de mando
    Button btnReset;        // Restaurar bindings por defecto
    std::vector<Label> bindingLabels;  // Lista de "accion → tipo[indice]"
    sf::RectangleShape panel;
    const sf::Font& font;

    void buildLayout();
    void refreshStatus();
};

#endif
