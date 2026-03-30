#include "ControlsTab.h"
#include "KeyBindings.h"
#include <algorithm>

// ---------------------------------------------------------------------------
// Orden lógico de las acciones y sus etiquetas en pantalla.
// Se usan arrays paralelos para garantizar el orden de presentación,
// ya que getAll() devuelve un unordered_map (orden no garantizado).
// ---------------------------------------------------------------------------
static const std::vector<std::pair<std::string, std::string>> ACTION_ORDER = {
    {"moveLeft",   "Mover izquierda"},
    {"moveRight",  "Mover derecha"},
    {"jump",       "Saltar"},
    {"attack",     "Atacar"},
    {"interact",   "Interactuar"},
    {"inventory",  "Inventario"},
    {"pause",      "Pausa"}
};

ControlsTab::ControlsTab(const sf::Font& f)
    : font(f)
{
    buildLayout();
}

// ---------------------------------------------------------------------------
// buildLayout: construye el panel, título, lista de KeyBindWidgets y botón
// de reset. Se llama también después de restaurar defaults para refrescar.
// ---------------------------------------------------------------------------
void ControlsTab::buildLayout() {
    // Fondo del panel
    panel.setPosition(0.f, 24.f);
    panel.setSize({512.f, 246.f});
    panel.setFillColor(sf::Color(20, 20, 30));

    // Etiqueta de título
    lblTitle = Label("Controles — Clic en la tecla para reasignar",
                     font, 10, sf::Color(160, 160, 180));
    lblTitle.setPosition(10.f, 28.f);
    lblTitle.setSize(400.f, 14.f);

    // Etiqueta de ayuda de scroll
    lblHint = Label("Arriba/Abajo: desplazar", font, 9, sf::Color(90, 90, 110));
    lblHint.setPosition(10.f, 232.f);
    lblHint.setSize(200.f, 14.f);

    // Reconstruye la lista de widgets de teclas
    auto& kb = KeyBindings::getInstance();
    bindWidgets.clear();
    bindWidgets.reserve(ACTION_ORDER.size());

    for (std::size_t i = 0; i < ACTION_ORDER.size(); ++i) {
        const std::string& actionId    = ACTION_ORDER[i].first;
        const std::string& actionLabel = ACTION_ORDER[i].second;

        KeyBindWidget w;
        w.setFont(font, 10);
        // Columna izquierda: etiqueta de acción (x=20, ancho=160)
        // Columna derecha: tecla asignada (x=200, ancho=90)
        // El widget ocupa el ancho total de ambas columnas (160+90=250)
        w.setPosition(20.f, 50.f + static_cast<float>(i) * ROW_HEIGHT);
        w.setSize(250.f, ROW_HEIGHT - 2.f);
        w.setAction(actionLabel);
        w.setCurrentKey(kb.get(actionId));

        // Captura el actionId por valor para el callback de remapeo
        const std::string id = actionId;
        w.setOnKeyBound([id](sf::Keyboard::Key key) {
            KeyBindings::getInstance().set(id, key);
            KeyBindings::getInstance().save();
        });

        bindWidgets.push_back(std::move(w));
    }

    // Botón de restaurar valores por defecto
    btnReset = Button("Restaurar defaults", font, 9);
    btnReset.setPosition(340.f, 230.f);
    btnReset.setSize(150.f, 18.f);
    btnReset.setCallback([this]() {
        KeyBindings::getInstance().resetToDefaults();
        KeyBindings::getInstance().save();
        scrollOffset = 0.f;
        buildLayout();   // refresca los widgets con los nuevos valores
    });
}

// ---------------------------------------------------------------------------
// onActivate: se llama cuando el usuario selecciona esta pestaña.
// Resetea el scroll y sincroniza cada widget con el valor actual de KeyBindings.
// ---------------------------------------------------------------------------
void ControlsTab::onActivate() {
    scrollOffset = 0.f;

    auto& kb = KeyBindings::getInstance();
    for (std::size_t i = 0; i < bindWidgets.size() && i < ACTION_ORDER.size(); ++i) {
        bindWidgets[i].setCurrentKey(kb.get(ACTION_ORDER[i].first));
    }
}

// ---------------------------------------------------------------------------
// handleEvent: distribuye eventos a los widgets y gestiona el scroll
// con teclas de flecha. El scroll se limita entre 0 y el máximo calculado.
// ---------------------------------------------------------------------------
void ControlsTab::handleEvent(const sf::Event& event) {
    // Distribuye a todos los KeyBindWidgets (alguno puede estar en modo escucha)
    for (auto& w : bindWidgets) {
        w.handleEvent(event);
    }

    // Botón de reset
    btnReset.handleEvent(event);

    // Scroll con teclas Up/Down
    if (event.type == sf::Event::KeyPressed) {
        // Calcular desplazamiento máximo: contenido total menos área visible
        const float contentHeight = static_cast<float>(ACTION_ORDER.size()) * ROW_HEIGHT;
        const float visibleHeight = 180.f;   // altura del área de lista en el panel
        const float maxScroll = std::max(0.f, contentHeight - visibleHeight);

        if (event.key.code == sf::Keyboard::Up) {
            scrollOffset = std::max(0.f, scrollOffset - SCROLL_SPEED);
        } else if (event.key.code == sf::Keyboard::Down) {
            scrollOffset = std::min(maxScroll, scrollOffset + SCROLL_SPEED);
        }
    }
}

// ---------------------------------------------------------------------------
// update: actualiza todos los widgets y el botón (animaciones, hover, etc.)
// ---------------------------------------------------------------------------
void ControlsTab::update(float dt) {
    for (auto& w : bindWidgets) {
        w.update(dt);
    }
    btnReset.update(dt);
}

// ---------------------------------------------------------------------------
// draw: recorta la vista al área del panel para aplicar el scroll
// y luego la restaura antes de dibujar los elementos fijos (hint, botón).
// ---------------------------------------------------------------------------
void ControlsTab::draw(sf::RenderWindow& window) {
    // Dibuja el fondo del panel sin recorte
    window.draw(panel);
    lblTitle.draw(window);

    // --- Vista recortada para la lista de widgets con scroll ---
    const sf::View originalView = window.getView();
    const sf::Vector2u winSize  = window.getSize();

    // Región de la ventana donde se dibuja la lista (coordenadas de píxeles)
    // El área visible de la lista empieza en Y=50 (bajo el título) y termina
    // antes de la barra de botones (~Y=228).
    const float listTop    = 50.f;
    const float listHeight = 178.f;   // 228 - 50
    const float listLeft   = 0.f;
    const float listWidth  = 512.f;

    // sf::View viewport en coordenadas normalizadas [0,1]
    sf::View listView;
    listView.setViewport({
        listLeft  / static_cast<float>(winSize.x),
        listTop   / static_cast<float>(winSize.y),
        listWidth  / static_cast<float>(winSize.x),
        listHeight / static_cast<float>(winSize.y)
    });
    // El centro de la vista en coordenadas del mundo incluye el scroll
    listView.setCenter(listWidth / 2.f, listTop + listHeight / 2.f + scrollOffset);
    listView.setSize(listWidth, listHeight);

    window.setView(listView);

    for (auto& w : bindWidgets) {
        w.draw(window);
    }

    // Restaura la vista original
    window.setView(originalView);

    // Elementos fijos (fuera del área de scroll)
    lblHint.draw(window);
    btnReset.draw(window);
}
