#include "GamepadTab.h"
#include "GamepadManager.h"
#include "GameSettings.h"

// ---------------------------------------------------------------------------
// Orden lógico de las acciones y sus etiquetas en pantalla.
// Se usa un vector de pares para que el orden de presentación sea consistente
// independientemente del orden del unordered_map interno.
// ---------------------------------------------------------------------------
static const std::vector<std::pair<std::string, std::string>> GAMEPAD_ACTION_ORDER = {
    {"moveLeft",   "Mover izquierda"},
    {"moveRight",  "Mover derecha"},
    {"jump",       "Saltar"},
    {"attack",     "Atacar"},
    {"interact",   "Interactuar"},
    {"inventory",  "Inventario"},
    {"pause",      "Pausa"}
};

// ---------------------------------------------------------------------------
// Helpers: convierte el enum Type y el índice a una cadena legible.
// ---------------------------------------------------------------------------
static std::string bindingTypeStr(const GamepadBinding& b) {
    switch (b.type) {
    case GamepadBinding::Type::Button:
        return "Boton[" + std::to_string(b.index) + "]";
    case GamepadBinding::Type::AxisPositive:
        return "EjePos[" + std::to_string(b.index) + "]";
    case GamepadBinding::Type::AxisNegative:
        return "EjeNeg[" + std::to_string(b.index) + "]";
    }
    return "?";
}

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
GamepadTab::GamepadTab(const sf::Font& f)
    : font(f)
{
    buildLayout();
}

// ---------------------------------------------------------------------------
// buildLayout: crea y posiciona todos los widgets.
//
// Coordenadas de referencia (iguales que las otras tabs):
//   Panel   : (0, 24) → tamaño (512, 222)
//   lblStatus: (10, 32)
//   toggle  : (10, 56)   — "Activar mando"
//   bindings: fila inicial y=90, paso 18px
//   btnReset: (340, 210)
// ---------------------------------------------------------------------------
void GamepadTab::buildLayout() {
    // Fondo del panel
    panel.setSize({512.f, 222.f});
    panel.setFillColor(sf::Color(20, 20, 30));
    panel.setPosition(0.f, 24.f);

    constexpr unsigned LABEL_SIZE = 11u;

    // Etiqueta de estado de conexión
    lblStatus = Label("Mandos conectados: 0", font, LABEL_SIZE, sf::Color(160, 200, 160));
    lblStatus.setPosition(10.f, 32.f);
    lblStatus.setSize(280.f, 16.f);

    // Toggle: activar/desactivar soporte de mando
    toggleEnabled.setLabel("Activar mando", font, LABEL_SIZE);
    toggleEnabled.setPosition(10.f, 56.f);
    toggleEnabled.setSize(40.f, 20.f);
    toggleEnabled.setOnChanged([](bool v) {
        GameSettings::getInstance().setBool("gamepadEnabled", v);
    });

    // Lista de bindings actuales
    // Construimos las etiquetas a partir del mapa de GamepadManager.
    // Usamos ACTION_ORDER para garantizar orden consistente.
    bindingLabels.clear();
    auto& gm = GamepadManager::getInstance();
    constexpr float ROW_Y_START = 90.f;
    constexpr float ROW_HEIGHT  = 18.f;

    for (std::size_t i = 0; i < GAMEPAD_ACTION_ORDER.size(); ++i) {
        const std::string& actionId    = GAMEPAD_ACTION_ORDER[i].first;
        const std::string& actionLabel = GAMEPAD_ACTION_ORDER[i].second;
        const GamepadBinding* b = gm.getBinding(actionId);

        std::string text = actionLabel + " -> ";
        text += b ? bindingTypeStr(*b) : "?";

        Label lbl(text, font, 10u, sf::Color(200, 200, 220));
        lbl.setPosition(20.f, ROW_Y_START + static_cast<float>(i) * ROW_HEIGHT);
        lbl.setSize(300.f, ROW_HEIGHT - 2.f);
        bindingLabels.push_back(std::move(lbl));
    }

    // Botón de restaurar defaults
    btnReset = Button("Restaurar defaults", font, 10u);
    btnReset.setPosition(340.f, 210.f);
    btnReset.setSize(155.f, 18.f);
    btnReset.setCallback([this]() {
        GamepadManager::getInstance().resetToDefaults();
        // Reconstruye las etiquetas con los nuevos valores
        buildLayout();
    });
}

// ---------------------------------------------------------------------------
// onActivate: se llama cuando el usuario selecciona esta pestaña.
// Refresca el estado de conexión y sincroniza el toggle con GameSettings.
// ---------------------------------------------------------------------------
void GamepadTab::onActivate() {
    refreshStatus();
    toggleEnabled.setValue(
        GameSettings::getInstance().getBool("gamepadEnabled", false));
}

// ---------------------------------------------------------------------------
// refreshStatus: actualiza la etiqueta de mandos conectados.
// Se llama en onActivate y puede llamarse en update si se desea polling.
// ---------------------------------------------------------------------------
void GamepadTab::refreshStatus() {
    lblStatus.setText("Mandos conectados: " +
                      std::to_string(GamepadManager::getInstance().connectedCount()));
}

// ---------------------------------------------------------------------------
// handleEvent: distribuye a toggle y botón.
// Las etiquetas de binding son puramente informativas (Label.handleEvent es no-op).
// ---------------------------------------------------------------------------
void GamepadTab::handleEvent(const sf::Event& event) {
    toggleEnabled.handleEvent(event);
    btnReset.handleEvent(event);
}

// ---------------------------------------------------------------------------
// update: actualiza animaciones de toggle y botón.
// ---------------------------------------------------------------------------
void GamepadTab::update(float dt) {
    toggleEnabled.update(dt);
    btnReset.update(dt);
}

// ---------------------------------------------------------------------------
// draw: dibuja panel, widgets de estado/control y lista de bindings.
// ---------------------------------------------------------------------------
void GamepadTab::draw(sf::RenderWindow& window) {
    window.draw(panel);

    lblStatus.draw(window);
    toggleEnabled.draw(window);

    for (auto& lbl : bindingLabels) {
        lbl.draw(window);
    }

    btnReset.draw(window);
}
