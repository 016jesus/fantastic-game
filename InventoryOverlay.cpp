#include "InventoryOverlay.h"
#include "Sanadoras.h"
#include <algorithm>

// ---------------------------------------------------------------------------
// Dimensiones y posición del panel centrado en 512x256
// Panel: 300x180 en (106, 38) — centrado horizontalmente y verticalmente
// ---------------------------------------------------------------------------
static constexpr float PANEL_X = 106.f;
static constexpr float PANEL_Y = 38.f;
static constexpr float PANEL_W = 300.f;
static constexpr float PANEL_H = 180.f;

// ---------------------------------------------------------------------------
// Constructor: inicializa formas y textos con valores por defecto
// ---------------------------------------------------------------------------
InventoryOverlay::InventoryOverlay() {
    // Panel principal
    panel.setSize(sf::Vector2f(PANEL_W, PANEL_H));
    panel.setFillColor(sf::Color(0, 0, 0, 210));
    panel.setPosition(PANEL_X, PANEL_Y);

    // Borde blanco de 1px
    border.setSize(sf::Vector2f(PANEL_W, PANEL_H));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::White);
    border.setOutlineThickness(1.f);
    border.setPosition(PANEL_X, PANEL_Y);

    // Highlight del ítem seleccionado (fila semitransparente)
    highlight.setSize(sf::Vector2f(PANEL_W - 12.f, 14.f));
    highlight.setFillColor(sf::Color(255, 255, 255, 50));

    // Texto de título
    titleText.setCharacterSize(11u);
    titleText.setFillColor(sf::Color::White);
    titleText.setString("INVENTARIO");
    titleText.setPosition(PANEL_X + 8.f, PANEL_Y + 6.f);

    // Texto cuando no hay ítems
    emptyText.setCharacterSize(9u);
    emptyText.setFillColor(sf::Color(180, 180, 180));
    emptyText.setString("Sin items");
    emptyText.setPosition(PANEL_X + 8.f, PANEL_Y + 30.f);

    // Hint de controles en la parte inferior del panel
    hintText.setCharacterSize(8u);
    hintText.setFillColor(sf::Color(160, 160, 160));
    hintText.setString("Flechas: navegar  ENTER: usar  I: cerrar");
    hintText.setPosition(PANEL_X + 6.f, PANEL_Y + PANEL_H - 14.f);
}

// ---------------------------------------------------------------------------
// loadFont: asigna la fuente a todos los textos
// ---------------------------------------------------------------------------
void InventoryOverlay::loadFont(const sf::Font& f) {
    font = &f;
    titleText.setFont(f);
    emptyText.setFont(f);
    hintText.setFont(f);
}

// ---------------------------------------------------------------------------
// toggle: abre o cierra el overlay; resetea selección al abrir
// ---------------------------------------------------------------------------
void InventoryOverlay::toggle() {
    open_ = !open_;
    if (open_) selectedIndex = 0;
}

// ---------------------------------------------------------------------------
// isOpen: consulta si el overlay está activo
// ---------------------------------------------------------------------------
bool InventoryOverlay::isOpen() const {
    return open_;
}

// ---------------------------------------------------------------------------
// getItemName: convierte el tipo numérico del objeto a su nombre legible
// Tipo 1 = Pociones (Sanadoras), Tipo 2 = Armas, Tipo 3 = Pociones Malignas
// ---------------------------------------------------------------------------
std::string InventoryOverlay::getItemName(Objetos* obj) const {
    if (!obj) return "Objeto desconocido";
    switch (obj->getTipo()) {
        case 1: return "Pocion Sanadora";
        case 2: return "Arma";
        case 3: return "Pocion Maligna";
        default: return "Objeto (" + std::to_string(obj->getTipo()) + ")";
    }
}

// ---------------------------------------------------------------------------
// buildItemList: reconstruye el vector de sf::Text a partir del inventario
// ---------------------------------------------------------------------------
void InventoryOverlay::buildItemList(const Protagonista& player) {
    if (!font) return;

    itemTexts.clear();
    const auto& inv = player.getInventario();
    for (std::size_t i = 0; i < inv.size(); ++i) {
        sf::Text t;
        t.setFont(*font);
        t.setCharacterSize(9u);
        t.setFillColor(sf::Color::White);
        t.setString(std::to_string(i + 1) + ". " + getItemName(inv[i].get()));
        // Cada fila ocupa 16px comenzando en y=30 desde el borde del panel
        t.setPosition(PANEL_X + 10.f,
                      PANEL_Y + 28.f + static_cast<float>(i) * 16.f);
        itemTexts.push_back(std::move(t));
    }
}

// ---------------------------------------------------------------------------
// handleEvent: gestiona navegación y uso de ítems dentro del overlay
// ---------------------------------------------------------------------------
bool InventoryOverlay::handleEvent(const sf::Event& event, Protagonista& player) {
    if (!open_) return false;
    if (event.type != sf::Event::KeyPressed) return false;

    const auto& inv = player.getInventario();
    const int count = static_cast<int>(inv.size());

    switch (event.key.code) {
        case sf::Keyboard::Up:
            if (count > 0) {
                selectedIndex = (selectedIndex - 1 + count) % count;
            }
            return true;

        case sf::Keyboard::Down:
            if (count > 0) {
                selectedIndex = (selectedIndex + 1) % count;
            }
            return true;

        case sf::Keyboard::Return:
            // Usar el ítem seleccionado
            if (count > 0 && selectedIndex < count) {
                Objetos* obj = inv[selectedIndex].get();
                if (obj && obj->getTipo() == 1) {
                    // Poción sanadora: restaura 20 de vida (máximo 100)
                    int nuevaVida = player.getVida() + 20;
                    if (nuevaVida > 100) nuevaVida = 100;
                    player.setVida(nuevaVida);
                    player.removeFromInventario(selectedIndex);

                    // Ajustar selección si era el último ítem
                    const int newCount = static_cast<int>(player.getInventario().size());
                    if (selectedIndex >= newCount && selectedIndex > 0) {
                        selectedIndex = newCount - 1;
                    }
                }
            }
            return true;

        case sf::Keyboard::I:
            // La tecla I también cierra el overlay
            open_ = false;
            return true;

        default:
            return false;
    }
}

// ---------------------------------------------------------------------------
// draw: dibuja el panel completo del inventario
// ---------------------------------------------------------------------------
void InventoryOverlay::draw(sf::RenderWindow& window, const Protagonista& player) {
    if (!open_) return;

    // Reconstruye la lista de ítems en cada frame (inventario puede cambiar)
    buildItemList(player);

    window.draw(panel);
    window.draw(border);
    window.draw(titleText);
    window.draw(hintText);

    const auto& inv = player.getInventario();
    if (inv.empty()) {
        window.draw(emptyText);
    } else {
        // Dibuja highlight detrás del ítem seleccionado
        if (selectedIndex < static_cast<int>(itemTexts.size())) {
            highlight.setPosition(PANEL_X + 6.f,
                                  PANEL_Y + 26.f + static_cast<float>(selectedIndex) * 16.f);
            window.draw(highlight);
        }

        for (auto& t : itemTexts) {
            window.draw(t);
        }
    }
}
