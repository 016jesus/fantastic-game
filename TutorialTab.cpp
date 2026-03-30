#include "TutorialTab.h"
#include <algorithm>

// ---------------------------------------------------------------------------
// Colores de acento por página (definidos como constantes para legibilidad)
// ---------------------------------------------------------------------------
static const sf::Color ACCENT_VERDE   { 40, 180,  80};
static const sf::Color ACCENT_ROJO    {200,  50,  50};
static const sf::Color ACCENT_AZUL    { 50, 120, 210};
static const sf::Color ACCENT_DORADO  {218, 165,  32};
static const sf::Color ACCENT_MAGENTA {200,  50, 180};

TutorialTab::TutorialTab(const sf::Font& f)
    : font(f)
{
    // Fondo del panel
    panel.setPosition(0.f, 24.f);
    panel.setSize({512.f, 246.f});
    panel.setFillColor(sf::Color(20, 20, 30));

    // Barra de acento encima del título (se colorea en showPage)
    accentBar.setPosition(0.f, 24.f);
    accentBar.setSize({512.f, 5.f});

    // Título de la página
    titleText.setFont(font);
    titleText.setCharacterSize(16u);
    titleText.setFillColor(sf::Color::White);

    // Cuerpo del texto
    bodyText.setFont(font);
    bodyText.setCharacterSize(10u);
    bodyText.setFillColor(sf::Color::White);

    // Indicador de página "N / 5" — centrado en x=256
    pageIndicator.setFont(font);
    pageIndicator.setCharacterSize(10u);
    pageIndicator.setFillColor(sf::Color(140, 140, 160));

    // Botón "Anterior"
    btnPrev = Button("< Anterior", font, 9);
    btnPrev.setPosition(20.f, 220.f);
    btnPrev.setSize(90.f, 18.f);
    btnPrev.setCallback([this]() {
        showPage(std::max(0, currentPage - 1));
    });

    // Botón "Siguiente"
    btnNext = Button("Siguiente >", font, 9);
    btnNext.setPosition(380.f, 220.f);
    btnNext.setSize(110.f, 18.f);
    btnNext.setCallback([this]() {
        showPage(std::min(static_cast<int>(pages.size()) - 1, currentPage + 1));
    });

    buildPages();
    showPage(0);
}

// ---------------------------------------------------------------------------
// buildPages: define el contenido de las 5 páginas del tutorial.
// ---------------------------------------------------------------------------
void TutorialTab::buildPages() {
    pages.clear();
    pages.reserve(5);

    pages.push_back({
        "Bienvenido",
        "The Legend of Tilin es un juego de accion 2D.\n"
        "Muevete con A y D, salta con ESPACIO.\n"
        "Tu objetivo: llegar a la Sala del Jefe y derrotarlo.\n"
        "\n"
        "Usa ESC para pausar en cualquier momento.",
        ACCENT_VERDE
    });

    pages.push_back({
        "Combate basico",
        "Los enemigos te atacaran cuando te acerques.\n"
        "Usa J para atacar a un enemigo en rango.\n"
        "\n"
        "Cada enemigo tiene puntos de vida.\n"
        "Cuando llegan a 0, son eliminados.\n"
        "\n"
        "Tu rango de ataque es de 40 pixeles.",
        ACCENT_ROJO
    });

    pages.push_back({
        "Inventario y pociones",
        "Pulsa I para abrir tu inventario.\n"
        "Usa las flechas para navegar entre items.\n"
        "Pulsa ENTER para usar una pocion seleccionada.\n"
        "\n"
        "Las pociones restauran 20 de vida.\n"
        "No puedes superar 100 de vida maxima.",
        ACCENT_AZUL
    });

    pages.push_back({
        "Vendedor y economia",
        "Busca al Vendedor en el mapa (rectangulo dorado).\n"
        "Acercate y pulsa E para abrir la tienda.\n"
        "\n"
        "Necesitas monedas para comprar.\n"
        "Encontraras monedas en los cofres del mapa.\n"
        "\n"
        "Atencion: hay un secreto si compras mucho...",
        ACCENT_DORADO
    });

    pages.push_back({
        "El Jefe",
        "La Sala del Jefe esta en la tercera seccion.\n"
        "El Jefe tiene 200 puntos de vida.\n"
        "\n"
        "Tiene dos tipos de ataque:\n"
        "  Normal: 15 de danio\n"
        "  Especial: 35 de danio (cada 5 segundos)\n"
        "\n"
        "Buena suerte!",
        ACCENT_MAGENTA
    });
}

// ---------------------------------------------------------------------------
// showPage: actualiza todos los elementos visuales para mostrar la página
// indicada. Hace clamp para evitar índices fuera de rango.
// ---------------------------------------------------------------------------
void TutorialTab::showPage(int index) {
    if (pages.empty()) return;
    currentPage = std::clamp(index, 0, static_cast<int>(pages.size()) - 1);

    const auto& page = pages[static_cast<std::size_t>(currentPage)];

    // Barra de acento
    accentBar.setFillColor(page.accentColor);

    // Título en (20, 35)
    titleText.setString(page.title);
    {
        sf::FloatRect tb = titleText.getLocalBounds();
        titleText.setOrigin(tb.left, tb.top);
        titleText.setPosition(20.f, 35.f);
    }

    // Cuerpo en (20, 60) — los \n en el string manejan el salto de línea
    bodyText.setString(page.body);
    {
        sf::FloatRect tb = bodyText.getLocalBounds();
        bodyText.setOrigin(tb.left, tb.top);
        bodyText.setPosition(20.f, 60.f);
    }

    // Indicador "N / 5" centrado en x=256, y=225
    const std::string indicator =
        std::to_string(currentPage + 1) + " / " + std::to_string(pages.size());
    pageIndicator.setString(indicator);
    {
        sf::FloatRect tb = pageIndicator.getLocalBounds();
        pageIndicator.setOrigin(tb.left + tb.width / 2.f, tb.top);
        pageIndicator.setPosition(256.f, 225.f);
    }

    // Oscurece los botones según los límites de navegación
    const sf::Color disabledBg {40, 40, 50};
    const sf::Color disabledTxt{80, 80, 90};

    if (currentPage == 0) {
        btnPrev.setNormalColor(disabledBg, disabledTxt);
        btnPrev.setHoverColor(disabledBg,  disabledTxt);
    } else {
        btnPrev.setNormalColor({60, 60, 80},   sf::Color::White);
        btnPrev.setHoverColor({90, 90, 120},   sf::Color::White);
    }

    if (currentPage == static_cast<int>(pages.size()) - 1) {
        btnNext.setNormalColor(disabledBg, disabledTxt);
        btnNext.setHoverColor(disabledBg,  disabledTxt);
    } else {
        btnNext.setNormalColor({60, 60, 80},   sf::Color::White);
        btnNext.setHoverColor({90, 90, 120},   sf::Color::White);
    }
}

// ---------------------------------------------------------------------------
// onActivate: siempre arranca en la primera página al entrar a esta pestaña.
// ---------------------------------------------------------------------------
void TutorialTab::onActivate() {
    showPage(0);
}

// ---------------------------------------------------------------------------
// handleEvent: navegación con teclas de flecha izquierda/derecha y botones.
// Los botones deshabilitados en los extremos no tienen efecto visual negativo
// porque el callback ya aplica clamp en showPage.
// ---------------------------------------------------------------------------
void TutorialTab::handleEvent(const sf::Event& event) {
    btnPrev.handleEvent(event);
    btnNext.handleEvent(event);

    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Left:
                showPage(currentPage - 1);
                break;
            case sf::Keyboard::Right:
                showPage(currentPage + 1);
                break;
            default:
                break;
        }
    }
}

// ---------------------------------------------------------------------------
// update: propaga dt a los botones para sus animaciones de hover/activo.
// ---------------------------------------------------------------------------
void TutorialTab::update(float dt) {
    btnPrev.update(dt);
    btnNext.update(dt);
}

// ---------------------------------------------------------------------------
// draw: dibuja el panel de fondo, barra de acento, textos y botones.
// ---------------------------------------------------------------------------
void TutorialTab::draw(sf::RenderWindow& window) {
    window.draw(panel);
    window.draw(accentBar);
    window.draw(titleText);
    window.draw(bodyText);
    window.draw(pageIndicator);
    btnPrev.draw(window);
    btnNext.draw(window);
}
