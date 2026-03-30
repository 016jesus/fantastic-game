#include "ui/tabs/CreditsTab.h"
#include <algorithm>

// ---------------------------------------------------------------------------
// Paleta y métricas de la pestaña de créditos
// ---------------------------------------------------------------------------
static const sf::Color COLOR_HEADING  {255, 215,   0};  // amarillo dorado
static const sf::Color COLOR_TEXT     {255, 255, 255};  // blanco
static const sf::Color COLOR_PANEL_BG { 20,  20,  30};  // fondo oscuro
static const sf::Color COLOR_FADE     { 20,  20,  30};  // mismo color que fondo → ilusión de fade

static constexpr unsigned SIZE_HEADING  = 14u;
static constexpr unsigned SIZE_TEXT     = 10u;
static constexpr float    H_HEADING     = 22.f;  // altura de línea para cabeceras
static constexpr float    H_TEXT        = 16.f;  // altura de línea para texto normal
static constexpr float    H_SEPARATOR   =  8.f;  // altura de separador vacío

// Área visible del panel de créditos (coordenadas ventana)
static constexpr float PANEL_X      =   0.f;
static constexpr float PANEL_Y      =  24.f;
static constexpr float PANEL_W      = 512.f;
static constexpr float PANEL_H      = 222.f;  // y=24 → y=246
static constexpr float FADE_BAND_H  =  20.f;  // altura de las bandas de enmascarado

// ---------------------------------------------------------------------------
CreditsTab::CreditsTab(const sf::Font& f)
    : font(f)
{
    // Panel de fondo
    panel.setPosition(PANEL_X, PANEL_Y);
    panel.setSize({PANEL_W, PANEL_H});
    panel.setFillColor(COLOR_PANEL_BG);

    // Banda superior (enmascara textos que salen por arriba)
    topFade.setPosition(PANEL_X, PANEL_Y);
    topFade.setSize({PANEL_W, FADE_BAND_H});
    topFade.setFillColor(COLOR_FADE);

    // Banda inferior (enmascara textos que salen por abajo)
    bottomFade.setPosition(PANEL_X, PANEL_Y + PANEL_H - FADE_BAND_H);
    bottomFade.setSize({PANEL_W, FADE_BAND_H});
    bottomFade.setFillColor(COLOR_FADE);

    buildEntries();
    buildRendered();
    totalH = calcTotalHeight();
}

// ---------------------------------------------------------------------------
// buildEntries: contenido de los créditos. Sin nombres reales.
// ---------------------------------------------------------------------------
void CreditsTab::buildEntries() {
    entries.clear();

    auto heading   = [](const std::string& t) -> CreditEntry { return {t, true,  false}; };
    auto text      = [](const std::string& t) -> CreditEntry { return {t, false, false}; };
    auto separator = []()                     -> CreditEntry { return {"", false, true}; };

    entries.push_back(heading("THE LEGEND OF TILIN"));
    entries.push_back(separator());

    entries.push_back(heading("Desarrollo"));
    entries.push_back(text("Diseno y programacion - El Arquitecto"));
    entries.push_back(text("Arte y sprites - Recursos libres / itch.io"));
    entries.push_back(separator());

    entries.push_back(heading("Motor"));
    entries.push_back(text("C++17"));
    entries.push_back(text("SFML 2.6 - Simple and Fast Multimedia Library"));
    entries.push_back(text("CMake 3.16+"));
    entries.push_back(separator());

    entries.push_back(heading("Assets"));
    entries.push_back(text("Sprites de personajes - itch.io (dominio publico)"));
    entries.push_back(text("Fuente - Minecraft Font (libre para uso personal)"));
    entries.push_back(separator());

    entries.push_back(heading("Agradecimientos"));
    entries.push_back(text("A todos los que probaron el juego"));
    entries.push_back(text("A la comunidad de SFML y C++"));
    entries.push_back(separator());

    entries.push_back(heading("Apoyo"));
    entries.push_back(text("Reporta bugs y sugerencias en GitHub"));
    entries.push_back(text("!Gracias por jugar!"));
    entries.push_back(separator());
    entries.push_back(separator());
}

// ---------------------------------------------------------------------------
// buildRendered: construye el vector de sf::Text ya configurados.
// Cada entry usa la posición Y absoluta en el espacio de contenido.
// ---------------------------------------------------------------------------
void CreditsTab::buildRendered() {
    rendered.clear();

    // X central para centrar los textos
    const float centerX = PANEL_W / 2.f;

    // Acumulamos Y desde 0 (scrollY se aplica en draw mediante sf::View)
    float curY = 0.f;

    for (const auto& entry : entries) {
        if (entry.isSeparator) {
            curY += H_SEPARATOR;
            continue;
        }

        sf::Text t;
        t.setFont(font);

        if (entry.isHeading) {
            t.setCharacterSize(SIZE_HEADING);
            t.setFillColor(COLOR_HEADING);
        } else {
            t.setCharacterSize(SIZE_TEXT);
            t.setFillColor(COLOR_TEXT);
        }

        t.setString(entry.text);

        // Centrar horizontalmente
        sf::FloatRect bounds = t.getLocalBounds();
        t.setOrigin(bounds.left + bounds.width / 2.f, bounds.top);
        t.setPosition(centerX, curY);

        rendered.push_back(std::move(t));

        curY += entry.isHeading ? H_HEADING : H_TEXT;
    }
}

// ---------------------------------------------------------------------------
// calcTotalHeight: suma la altura de todos los entries
// ---------------------------------------------------------------------------
float CreditsTab::calcTotalHeight() const {
    float h = 0.f;
    for (const auto& entry : entries) {
        if (entry.isSeparator)        h += H_SEPARATOR;
        else if (entry.isHeading)     h += H_HEADING;
        else                          h += H_TEXT;
    }
    return h;
}

// ---------------------------------------------------------------------------
// onActivate / onDeactivate
// ---------------------------------------------------------------------------
void CreditsTab::onActivate() {
    // Reinicia el scroll al principio y reactiva el auto-scroll
    scrollY     = 0.f;
    autoScroll  = true;
}

void CreditsTab::onDeactivate() {
    autoScroll = false;
}

// ---------------------------------------------------------------------------
// handleEvent: rueda del ratón desactiva el auto-scroll y ajusta manualmente
// ---------------------------------------------------------------------------
void CreditsTab::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        autoScroll = false;
        scrollY -= event.mouseWheelScroll.delta * 20.f;
        // No limitamos el scroll hacia abajo deliberadamente — el ciclo lo maneja update()
        if (scrollY < 0.f) scrollY = 0.f;
    }
}

// ---------------------------------------------------------------------------
// update: avanza el scroll automático y cicla cuando termina
// ---------------------------------------------------------------------------
void CreditsTab::update(float dt) {
    if (!autoScroll) return;

    scrollY += scrollSpeed * dt;

    // Cuando se supera el final del contenido más un margen de pantalla vacía,
    // reiniciamos desde "antes del inicio" para un ciclo continuo.
    if (scrollY > totalH + 100.f) {
        scrollY = -PANEL_H;
    }
}

// ---------------------------------------------------------------------------
// draw: usa sf::View recortado al área del panel para aplicar scroll.
// Dibuja las bandas de enmascarado encima para ocultar los bordes.
// ---------------------------------------------------------------------------
void CreditsTab::draw(sf::RenderWindow& window) {
    // Fondo del panel (fuera de la vista modificada)
    window.draw(panel);

    const sf::View originalView = window.getView();
    const sf::Vector2u winSize  = window.getSize();

    // Vista recortada al área del panel (viewport en [0..1])
    sf::View contentView;
    contentView.setViewport({
        PANEL_X / static_cast<float>(winSize.x),
        PANEL_Y / static_cast<float>(winSize.y),
        PANEL_W / static_cast<float>(winSize.x),
        PANEL_H / static_cast<float>(winSize.y)
    });

    // El centro de la vista sigue el scroll. Los textos están en y=[0..totalH],
    // así que el top visible = scrollY, el centro visible = scrollY + PANEL_H/2.
    contentView.setSize(PANEL_W, PANEL_H);
    contentView.setCenter(PANEL_W / 2.f, scrollY + PANEL_H / 2.f);

    window.setView(contentView);

    for (const auto& t : rendered) {
        window.draw(t);
    }

    // Restaura la vista antes de dibujar las bandas de enmascarado
    window.setView(originalView);

    // Las bandas usan el mismo color de fondo — ilusión de fade sin alpha real
    window.draw(topFade);
    window.draw(bottomFade);
}
