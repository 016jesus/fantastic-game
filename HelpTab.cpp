#include "HelpTab.h"
#include <algorithm>

// ---------------------------------------------------------------------------
// Paleta de colores usada en esta pestaña
// ---------------------------------------------------------------------------
static const sf::Color COLOR_HEADING   {218, 180,  40};   // amarillo/dorado
static const sf::Color COLOR_ITEM      {220, 220, 220};   // blanco suave
static const sf::Color COLOR_PANEL_BG  { 20,  20,  30};   // fondo oscuro
static const sf::Color COLOR_COL_BG   { 28,  28,  40};   // columnas ligeramente más claras

// Tamaño de fuente para cabeceras e ítems
static constexpr unsigned SIZE_HEADING = 11u;
static constexpr unsigned SIZE_ITEM    = 10u;

// Espaciado vertical entre líneas
static constexpr float LINE_HEADING    = 16.f;
static constexpr float LINE_ITEM       = 14.f;
static constexpr float SECTION_GAP     = 10.f;   // espacio extra antes de cada sección

HelpTab::HelpTab(const sf::Font& f)
    : font(f)
{
    // Fondo general del panel
    panel.setPosition(0.f, 24.f);
    panel.setSize({512.f, 246.f});
    panel.setFillColor(COLOR_PANEL_BG);

    // Columna izquierda (x=8, ancho=246)
    leftColumn.setPosition(8.f, 28.f);
    leftColumn.setSize({246.f, 236.f});
    leftColumn.setFillColor(COLOR_COL_BG);

    // Columna derecha (x=260, ancho=246)
    rightColumn.setPosition(260.f, 28.f);
    rightColumn.setSize({246.f, 236.f});
    rightColumn.setFillColor(COLOR_COL_BG);

    buildContent();
    buildRenderTexts();
}

// ---------------------------------------------------------------------------
// buildContent: define todas las secciones de ayuda con sus ítems de texto.
// ---------------------------------------------------------------------------
void HelpTab::buildContent() {
    sections.clear();

    // Sección 0: Controles — columna izquierda
    sections.push_back({
        "CONTROLES",
        {
            "A / <- - Mover izquierda",
            "D / -> - Mover derecha",
            "ESPACIO - Saltar",
            "J - Atacar",
            "E - Interactuar (NPCs, cofres, tienda)",
            "I - Abrir / cerrar inventario",
            "ESC - Pausar / reanudar",
            "F3 - Overlay de debug"
        }
    });

    // Sección 1: Mecánicas — columna izquierda
    sections.push_back({
        "MECANICAS",
        {
            "* Mata enemigos para ganar experiencia",
            "* Compra pociones en la tienda del Vendedor",
            "* Abre cofres para conseguir monedas",
            "* Usa pociones desde el inventario (I -> ENTER)",
            "* El juego se guarda automaticamente al morir"
        }
    });

    // Sección 2: Progresión — columna derecha
    sections.push_back({
        "PROGRESION",
        {
            "* Seccion 1: Aldea - enemigos basicos",
            "* Seccion 2: Catacumbas - esqueletos mas duros",
            "* Seccion 3: Sala del Jefe - derrota al Jefe",
            "* Al llegar al borde derecho avanzas de seccion"
        }
    });

    // Sección 3: Combate — columna derecha
    sections.push_back({
        "COMBATE",
        {
            "* Skeletons te detectan a 120px",
            "* Patrullan en rango cuando no te ven",
            "* El Jefe tiene ataque especial cada 5 segundos",
            "* Tu rango de ataque: 40px"
        }
    });
}

// ---------------------------------------------------------------------------
// buildRenderTexts: convierte las secciones en objetos sf::Text listos para
// dibujar. Secciones 0-1 en columna izquierda; secciones 2-3 en columna derecha.
// ---------------------------------------------------------------------------
void HelpTab::buildRenderTexts() {
    renderedTexts.clear();

    // Describe cada grupo de secciones y su columna de origen (x, yStart)
    struct ColumnGroup {
        std::size_t sectionFrom;
        std::size_t sectionTo;     // exclusivo
        float       colX;
        float       yStart;
    };

    const std::vector<ColumnGroup> groups = {
        {0, 2, 12.f,  34.f},   // columna izquierda: secciones 0 y 1
        {2, 4, 264.f, 34.f}    // columna derecha:   secciones 2 y 3
    };

    for (const auto& group : groups) {
        float curY = group.yStart;

        for (std::size_t si = group.sectionFrom; si < group.sectionTo; ++si) {
            const auto& sec = sections[si];

            // Cabecera de sección
            sf::Text heading;
            heading.setFont(font);
            heading.setCharacterSize(SIZE_HEADING);
            heading.setFillColor(COLOR_HEADING);
            heading.setString(sec.heading);
            heading.setPosition(group.colX, curY);
            renderedTexts.push_back(std::move(heading));
            curY += LINE_HEADING;

            // Ítems de la sección
            for (const auto& line : sec.lines) {
                sf::Text item;
                item.setFont(font);
                item.setCharacterSize(SIZE_ITEM);
                item.setFillColor(COLOR_ITEM);
                item.setString(line);
                item.setPosition(group.colX + 4.f, curY);
                renderedTexts.push_back(std::move(item));
                curY += LINE_ITEM;
            }

            curY += SECTION_GAP;
        }
    }
}

// ---------------------------------------------------------------------------
// handleEvent: scroll con rueda del ratón y con teclas Up/Down.
// El scroll se limita a un rango calculado a partir del contenido total.
// ---------------------------------------------------------------------------
void HelpTab::handleEvent(const sf::Event& event) {
    // Altura total aproximada del contenido (calculada desde el último texto)
    float contentBottom = 0.f;
    for (const auto& t : renderedTexts) {
        float bottom = t.getPosition().y + static_cast<float>(t.getCharacterSize()) * 1.5f;
        if (bottom > contentBottom) contentBottom = bottom;
    }
    const float visibleHeight = 220.f;
    const float maxScroll = std::max(0.f, contentBottom - visibleHeight);

    if (event.type == sf::Event::MouseWheelScrolled) {
        scrollOffset -= event.mouseWheelScroll.delta * 20.f;
        scrollOffset  = std::max(0.f, std::min(scrollOffset, maxScroll));
    }

    if (event.type == sf::Event::KeyPressed) {
        constexpr float SPEED = 20.f;
        if (event.key.code == sf::Keyboard::Up) {
            scrollOffset = std::max(0.f, scrollOffset - SPEED);
        } else if (event.key.code == sf::Keyboard::Down) {
            scrollOffset = std::min(maxScroll, scrollOffset + SPEED);
        }
    }
}

void HelpTab::update(float /*dt*/) {
    // No hay animaciones que actualizar en esta pestaña
}

// ---------------------------------------------------------------------------
// draw: aplica la vista recortada al panel y dibuja columnas y textos.
// ---------------------------------------------------------------------------
void HelpTab::draw(sf::RenderWindow& window) {
    window.draw(panel);
    window.draw(leftColumn);
    window.draw(rightColumn);

    // Vista recortada para el contenido con scroll
    const sf::View originalView = window.getView();
    const sf::Vector2u winSize  = window.getSize();

    const float areaTop    = 28.f;
    const float areaHeight = 236.f;
    const float areaLeft   = 0.f;
    const float areaWidth  = 512.f;

    sf::View contentView;
    contentView.setViewport({
        areaLeft   / static_cast<float>(winSize.x),
        areaTop    / static_cast<float>(winSize.y),
        areaWidth  / static_cast<float>(winSize.x),
        areaHeight / static_cast<float>(winSize.y)
    });
    contentView.setCenter(areaWidth / 2.f, areaTop + areaHeight / 2.f + scrollOffset);
    contentView.setSize(areaWidth, areaHeight);

    window.setView(contentView);

    for (const auto& t : renderedTexts) {
        window.draw(t);
    }

    window.setView(originalView);
}
