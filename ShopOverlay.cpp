#include "ShopOverlay.h"
#include "Sanadoras.h"
#include "cac.h"

// ---------------------------------------------------------------------------
// ArmaBasica: subclase concreta de CAC para instanciar en la tienda.
// CAC hereda de Armas que declara getTamanio() puro; esta clase lo implementa.
// ---------------------------------------------------------------------------
class ArmaBasica : public CAC {
public:
    ArmaBasica(int danio, float rango) : CAC(danio, rango) {}
    int getTamanio() override { return 1; }
};

// ---------------------------------------------------------------------------
// Panel centrado en 512x256: 320x180 en (96, 38)
// ---------------------------------------------------------------------------
static constexpr float PANEL_X = 96.f;
static constexpr float PANEL_Y = 38.f;
static constexpr float PANEL_W = 320.f;
static constexpr float PANEL_H = 180.f;

// Color dorado para título y borde
static const sf::Color COLOR_GOLD(218, 165, 32);
// Fondo azul-negro oscuro
static const sf::Color COLOR_BG(10, 10, 40, 220);

// ---------------------------------------------------------------------------
// Constructor: inicializa formas y carga el catálogo
// ---------------------------------------------------------------------------
ShopOverlay::ShopOverlay() {
    // Panel principal con fondo azul-negro oscuro
    panel.setSize(sf::Vector2f(PANEL_W, PANEL_H));
    panel.setFillColor(COLOR_BG);
    panel.setPosition(PANEL_X, PANEL_Y);

    // Borde dorado de 1px
    border.setSize(sf::Vector2f(PANEL_W, PANEL_H));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(COLOR_GOLD);
    border.setOutlineThickness(1.f);
    border.setPosition(PANEL_X, PANEL_Y);

    // Highlight del ítem seleccionado
    highlight.setSize(sf::Vector2f(PANEL_W - 12.f, 14.f));
    highlight.setFillColor(sf::Color(218, 165, 32, 60));

    // Título dorado centrado en la parte superior del panel
    titleText.setCharacterSize(11u);
    titleText.setFillColor(COLOR_GOLD);
    titleText.setString("TIENDA DEL VENDEDOR");
    titleText.setPosition(PANEL_X + 8.f, PANEL_Y + 6.f);

    // Dinero del jugador en esquina superior derecha (se actualiza en draw)
    moneyText.setCharacterSize(9u);
    moneyText.setFillColor(COLOR_GOLD);

    // Hint de controles en la parte inferior
    hintText.setCharacterSize(8u);
    hintText.setFillColor(sf::Color(160, 160, 160));
    hintText.setString("Flechas: navegar  ENTER: comprar  ESC: cerrar");
    hintText.setPosition(PANEL_X + 6.f, PANEL_Y + PANEL_H - 14.f);

    // Texto de feedback transitorio (compra exitosa / sin fondos)
    feedbackText.setCharacterSize(9u);
    feedbackText.setFillColor(sf::Color::Green);
    feedbackText.setPosition(PANEL_X + 8.f, PANEL_Y + PANEL_H - 26.f);

    initCatalog();
}

// ---------------------------------------------------------------------------
// initCatalog: define los productos disponibles en la tienda
// ---------------------------------------------------------------------------
void ShopOverlay::initCatalog() {
    catalog.clear();
    catalog.push_back({"Pocion Sanadora",    10, 1});
    catalog.push_back({"Pocion Sanadora++",  20, 1});
    catalog.push_back({"Arma Basica",        30, 2});
}

// ---------------------------------------------------------------------------
// loadFont: asigna la fuente a todos los textos del overlay
// ---------------------------------------------------------------------------
void ShopOverlay::loadFont(const sf::Font& f) {
    font = &f;
    titleText.setFont(f);
    moneyText.setFont(f);
    hintText.setFont(f);
    feedbackText.setFont(f);

    // Easter egg text — dorado, centrado en la parte inferior del panel
    easterEggText.setFont(f);
    easterEggText.setCharacterSize(8u);
    easterEggText.setFillColor(sf::Color(218, 165, 32));   // dorado
    easterEggText.setPosition(PANEL_X + 8.f, PANEL_Y + PANEL_H - 38.f);

    buildUI();
}

// ---------------------------------------------------------------------------
// buildUI: construye los sf::Text del catálogo (se llama al cargar fuente)
// ---------------------------------------------------------------------------
void ShopOverlay::buildUI() {
    if (!font) return;

    itemTexts.clear();
    for (std::size_t i = 0; i < catalog.size(); ++i) {
        sf::Text t;
        t.setFont(*font);
        t.setCharacterSize(9u);
        t.setFillColor(sf::Color::White);
        // Formato: "1. Pocion Sanadora    [10 mon]"
        t.setString(std::to_string(i + 1) + ". " + catalog[i].name +
                    "  [" + std::to_string(catalog[i].price) + " mon]");
        t.setPosition(PANEL_X + 10.f,
                      PANEL_Y + 28.f + static_cast<float>(i) * 18.f);
        itemTexts.push_back(std::move(t));
    }
}

// ---------------------------------------------------------------------------
// open / close / isOpen
// ---------------------------------------------------------------------------
void ShopOverlay::open() {
    open_         = true;
    selectedIndex = 0;
    showFeedback  = false;
}

void ShopOverlay::close() {
    open_ = false;
}

bool ShopOverlay::isOpen() const {
    return open_;
}

// ---------------------------------------------------------------------------
// buySelected: intenta comprar el ítem actualmente seleccionado
// ---------------------------------------------------------------------------
void ShopOverlay::buySelected(Protagonista& player) {
    if (selectedIndex < 0 || selectedIndex >= static_cast<int>(catalog.size())) return;

    const ShopItem& item = catalog[selectedIndex];

    if (player.getDinero() < item.price) {
        // Sin fondos suficientes
        feedbackText.setString("Sin fondos");
        feedbackText.setFillColor(sf::Color::Red);
        showFeedback = true;
        return;
    }

    // Descontar el precio
    player.setDinero(player.getDinero() - item.price);

    // Añadir el objeto al inventario del jugador según el tipo
    if (item.type == 1) {
        // Poción sanadora — aumento de 20 HP para la básica, 40 para la ++
        int aumento = (selectedIndex == 0) ? 20 : 40;
        player.actualizarInventario(true, new Sanadoras(true, item.price, aumento));
    } else if (item.type == 2) {
        // Arma básica con daño 15 y rango 30
        player.actualizarInventario(true, new ArmaBasica(15, 30.f));
    }

    feedbackText.setString("!Comprado!");
    feedbackText.setFillColor(sf::Color::Green);
    showFeedback = true;

    // Easter egg: tras 3 compras exitosas, obsequiar 50 monedas + poción gratis
    purchaseCount++;
    if (purchaseCount >= 3 && !easterEggShown) {
        easterEggShown = true;
        easterEggText.setString("!Sabia que volveras!\n  Toma esto... shh.");
        easterEggTimer = 4.f;
        player.setDinero(player.getDinero() + 50);
        player.actualizarInventario(true, new Sanadoras(true, 0, 20));
    }
}

// ---------------------------------------------------------------------------
// update: decrementa el temporizador del easter egg
// ---------------------------------------------------------------------------
void ShopOverlay::update(float deltaTime) {
    if (easterEggTimer > 0.f) {
        easterEggTimer -= deltaTime;
        if (easterEggTimer < 0.f) easterEggTimer = 0.f;
    }
}

// ---------------------------------------------------------------------------
// handleEvent: gestiona la entrada de teclado dentro de la tienda
// ---------------------------------------------------------------------------
bool ShopOverlay::handleEvent(const sf::Event& event, Protagonista& player) {
    if (!open_) return false;
    if (event.type != sf::Event::KeyPressed) return false;

    const int count = static_cast<int>(catalog.size());

    switch (event.key.code) {
        case sf::Keyboard::Up:
            selectedIndex = (selectedIndex - 1 + count) % count;
            showFeedback  = false;
            return true;

        case sf::Keyboard::Down:
            selectedIndex = (selectedIndex + 1) % count;
            showFeedback  = false;
            return true;

        case sf::Keyboard::Return:
            buySelected(player);
            return true;

        case sf::Keyboard::Escape:
            open_ = false;
            return true;

        default:
            return false;
    }
}

// ---------------------------------------------------------------------------
// draw: dibuja el panel completo de la tienda
// ---------------------------------------------------------------------------
void ShopOverlay::draw(sf::RenderWindow& window, Protagonista& player) {
    if (!open_) return;

    // Actualizar texto de dinero del jugador
    moneyText.setString("Monedas: " + std::to_string(player.getDinero()));
    // Posicionar en esquina superior derecha del panel con margen
    sf::FloatRect mb = moneyText.getLocalBounds();
    moneyText.setPosition(PANEL_X + PANEL_W - mb.width - 10.f, PANEL_Y + 6.f);

    window.draw(panel);
    window.draw(border);
    window.draw(titleText);
    window.draw(moneyText);
    window.draw(hintText);

    // Highlight del ítem seleccionado
    if (selectedIndex < static_cast<int>(itemTexts.size())) {
        highlight.setPosition(PANEL_X + 6.f,
                              PANEL_Y + 26.f + static_cast<float>(selectedIndex) * 18.f);
        window.draw(highlight);
    }

    for (auto& t : itemTexts) {
        window.draw(t);
    }

    // Mensaje de feedback transitorio (compra / sin fondos)
    if (showFeedback) {
        window.draw(feedbackText);
        // El mensaje se limpia en el siguiente draw para que persista un frame
        showFeedback = false;
    }

    // Easter egg: texto dorado visible mientras el temporizador esté activo
    if (easterEggTimer > 0.f) {
        window.draw(easterEggText);
    }
}
