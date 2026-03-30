#include "states/OptionsState.h"
#include "core/GameStateManager.h"
#include <SFML/Graphics.hpp>

// ---------------------------------------------------------------------------
// Constructor: almacena el gestor de estados y el flag fromPause.
// La construcción de la UI se difiere a onEnter() para garantizar que la
// fuente y los recursos estén disponibles cuando el estado se activa.
// ---------------------------------------------------------------------------
OptionsState::OptionsState(GameStateManager* gsm, bool fromPause)
    : fromPause(fromPause)
{
    this->gsm = gsm;
}

// ---------------------------------------------------------------------------
// addTab: punto de extensión OCP — puede llamarse antes o después de onEnter.
// Si se llama después de buildUI() la pestaña se añade al vector pero el
// TabBar ya fue construido; el llamador es responsable de añadir tabs antes
// de empujar el estado.
// ---------------------------------------------------------------------------
void OptionsState::addTab(std::unique_ptr<ITab> tab) {
    tabs.push_back(std::move(tab));
}

// ---------------------------------------------------------------------------
// onEnter: carga fuente y construye la UI
// ---------------------------------------------------------------------------
void OptionsState::onEnter() {
    // Intenta cargar la fuente usada en todo el proyecto
    if (!font.loadFromFile("minecraft.otf")) {
        font.loadFromFile("Minecraft.ttf");
    }
    buildUI();
}

// ---------------------------------------------------------------------------
// onExit: no es necesario liberar recursos explícitamente (RAII)
// ---------------------------------------------------------------------------
void OptionsState::onExit() {}

// ---------------------------------------------------------------------------
// buildUI: construye todos los elementos visuales del estado
// ---------------------------------------------------------------------------
void OptionsState::buildUI() {
    // --- Fondo semitransparente 512x256 ---
    background.setSize(sf::Vector2f(512.f, 256.f));
    background.setFillColor(sf::Color(0, 0, 0, 200));
    background.setPosition(0.f, 0.f);

    // --- Área de contenido: debajo del TabBar (y=24), altura=222 ---
    contentArea.setSize(sf::Vector2f(512.f, 222.f));
    contentArea.setFillColor(sf::Color(20, 20, 30));
    contentArea.setPosition(0.f, 24.f);

    // --- Título: visible sólo si no hay tabs cargadas ---
    titleText.setFont(font);
    titleText.setString("OPCIONES");
    titleText.setCharacterSize(12u);
    titleText.setFillColor(sf::Color(180, 180, 180));
    titleText.setPosition(6.f, 4.f);

    // --- Botón Volver: esquina inferior derecha ---
    backButton = Button("< Volver", font, 11u);
    backButton.setPosition(420.f, 236.f);
    backButton.setSize(80.f, 18.f);
    backButton.setCallback([this]() {
        gsm->pop();
    });

    // --- TabBar: ocupa la franja superior de 512x24 ---
    tabBar.setFont(font, 12u);
    tabBar.setPosition(0.f, 0.f);
    tabBar.setSize(512.f, 24.f);
    tabBar.setOnTabChanged([this](int index) {
        onTabChanged(index);
    });

    // Registra las pestañas ya añadidas via addTab()
    for (const auto& tab : tabs) {
        tabBar.addTab(tab->getTitle());
    }

    // Activa la primera pestaña si existe
    if (!tabs.empty()) {
        activeTabIndex = 0;
        tabBar.setActiveTab(0);
        tabs[0]->onActivate();
    }
}

// ---------------------------------------------------------------------------
// addTab: delegada al vector; si buildUI() ya se ejecutó el TabBar no se
// actualiza automáticamente — se presupone uso previo a onEnter().
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// onTabChanged: desactiva la pestaña anterior y activa la nueva
// ---------------------------------------------------------------------------
void OptionsState::onTabChanged(int index) {
    if (index < 0 || index >= static_cast<int>(tabs.size())) return;
    if (activeTabIndex >= 0 && activeTabIndex < static_cast<int>(tabs.size())) {
        tabs[activeTabIndex]->onDeactivate();
    }
    activeTabIndex = index;
    tabs[activeTabIndex]->onActivate();
}

// ---------------------------------------------------------------------------
// handleEvent
// ---------------------------------------------------------------------------
void OptionsState::handleEvent(const sf::Event& event) {
    // Escape cierra el panel de opciones
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape) {
        gsm->pop();
        return;
    }

    tabBar.handleEvent(event);
    backButton.handleEvent(event);

    if (activeTabIndex >= 0 && activeTabIndex < static_cast<int>(tabs.size())) {
        tabs[activeTabIndex]->handleEvent(event);
    }
}

// ---------------------------------------------------------------------------
// update
// ---------------------------------------------------------------------------
void OptionsState::update(float deltaTime) {
    tabBar.update(deltaTime);
    backButton.update(deltaTime);

    if (activeTabIndex >= 0 && activeTabIndex < static_cast<int>(tabs.size())) {
        tabs[activeTabIndex]->update(deltaTime);
    }
}

// ---------------------------------------------------------------------------
// render: orden back-to-front para transparencia correcta
// ---------------------------------------------------------------------------
void OptionsState::render(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(contentArea);

    // El título sólo es informativo cuando no hay pestañas
    if (tabs.empty()) {
        window.draw(titleText);
    }

    // Pestaña activa va antes que el TabBar para que éste quede encima
    if (activeTabIndex >= 0 && activeTabIndex < static_cast<int>(tabs.size())) {
        tabs[activeTabIndex]->draw(window);
    }

    tabBar.draw(window);
    backButton.draw(window);
}
