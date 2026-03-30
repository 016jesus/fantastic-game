#pragma once
#ifndef OPTIONS_STATE_H
#define OPTIONS_STATE_H
#include "IGameState.h"
#include "ITab.h"
#include "TabBar.h"
#include "Button.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

// ---------------------------------------------------------------------------
// OptionsState: estado del menú de opciones.
// Principio Open/Closed — esta clase NUNCA se modifica para añadir pestañas.
// Cada nueva pestaña se crea como clase independiente que extiende ITab y se
// inyecta mediante addTab() antes de empujar el estado al stack.
// ---------------------------------------------------------------------------
class OptionsState : public IGameState {
public:
    // fromPause=true: al cerrar vuelve a la pantalla de pausa, no al menú
    explicit OptionsState(GameStateManager* gsm, bool fromPause = false);

    // OCP — punto de extensión único: nunca hay que modificar OptionsState
    // para añadir una nueva pestaña; sólo se llama addTab() externamente.
    void addTab(std::unique_ptr<ITab> tab);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;
    void onExit()  override;

private:
    std::vector<std::unique_ptr<ITab>> tabs;
    TabBar               tabBar;
    sf::RectangleShape   background;
    sf::RectangleShape   contentArea;
    sf::Text             titleText;
    sf::Font             font;
    Button               backButton;
    int                  activeTabIndex = 0;
    bool                 fromPause;

    void onTabChanged(int index);
    void buildUI();
};
#endif
