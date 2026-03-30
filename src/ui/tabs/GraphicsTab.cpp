#include "ui/tabs/GraphicsTab.h"
#include "systems/GameSettings.h"

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
GraphicsTab::GraphicsTab(const sf::Font& font)
    : font(font)
{
    buildLayout();
}

// ---------------------------------------------------------------------------
// buildLayout: crea y posiciona todos los widgets según el grid de filas.
//
// Coordenadas de referencia:
//   - El panel ocupa (0,24)→(512,246) — debajo del TabBar.
//   - Labels en x=10, widgets en x=190, ancho de widget ~140px.
//   - Filas en y = 40, 72, 104, 136, 168 (paso de 32px).
// ---------------------------------------------------------------------------
void GraphicsTab::buildLayout() {
    // Panel de fondo de la pestaña
    panel.setSize(sf::Vector2f(512.f, 222.f));
    panel.setFillColor(sf::Color(20, 20, 30));
    panel.setPosition(0.f, 24.f);

    constexpr float LABEL_X   = 10.f;
    constexpr float WIDGET_X  = 190.f;
    constexpr float WIDGET_W  = 140.f;
    constexpr float WIDGET_H  = 20.f;
    constexpr unsigned LABEL_SIZE = 11u;

    // -----------------------------------------------------------------------
    // Fila 1 — Resolución (y=40)
    // -----------------------------------------------------------------------
    lblResolution = Label("Resolucion", font, LABEL_SIZE, sf::Color::White);
    lblResolution.setPosition(LABEL_X, 40.f);
    lblResolution.setSize(170.f, WIDGET_H);

    dropResolution.setFont(font, 11u);
    dropResolution.setOptions({"512x256", "800x450", "1280x720", "1920x1080"});
    dropResolution.setPosition(WIDGET_X, 40.f);
    dropResolution.setSize(WIDGET_W, WIDGET_H);
    // Resolución sólo se aplica con el botón Aplicar (requiere recrear ventana)

    // -----------------------------------------------------------------------
    // Fila 2 — Pantalla completa (y=72)
    // -----------------------------------------------------------------------
    lblFullscreen = Label("Modo", font, LABEL_SIZE, sf::Color::White);
    lblFullscreen.setPosition(LABEL_X, 72.f);
    lblFullscreen.setSize(170.f, WIDGET_H);

    toggleFullscreen.setLabel("Pantalla completa", font, LABEL_SIZE);
    toggleFullscreen.setPosition(WIDGET_X, 72.f);
    toggleFullscreen.setSize(WIDGET_W, WIDGET_H);
    // Fullscreen también sólo se aplica con el botón Aplicar

    // -----------------------------------------------------------------------
    // Fila 3 — VSync (y=104)
    // -----------------------------------------------------------------------
    lblVSync = Label("VSync", font, LABEL_SIZE, sf::Color::White);
    lblVSync.setPosition(LABEL_X, 104.f);
    lblVSync.setSize(170.f, WIDGET_H);

    toggleVSync.setLabel("VSync", font, LABEL_SIZE);
    toggleVSync.setPosition(WIDGET_X, 104.f);
    toggleVSync.setSize(WIDGET_W, WIDGET_H);
    toggleVSync.setOnChanged([](bool value) {
        GameSettings::getInstance().setBool("vsync", value);
    });

    // -----------------------------------------------------------------------
    // Fila 4 — Límite FPS (y=136)
    // -----------------------------------------------------------------------
    lblFPS = Label("Limite FPS", font, LABEL_SIZE, sf::Color::White);
    lblFPS.setPosition(LABEL_X, 136.f);
    lblFPS.setSize(170.f, WIDGET_H);

    dropFPS.setFont(font, 11u);
    dropFPS.setOptions({"30", "60", "120", "Sin limite"});
    dropFPS.setPosition(WIDGET_X, 136.f);
    dropFPS.setSize(WIDGET_W, WIDGET_H);
    dropFPS.setOnSelected([](int /*index*/, const std::string& item) {
        float fps = 60.f;
        if      (item == "30")        fps = 30.f;
        else if (item == "60")        fps = 60.f;
        else if (item == "120")       fps = 120.f;
        else if (item == "Sin limite") fps = 0.f;
        GameSettings::getInstance().setFloat("fpsLimit", fps);
    });

    // -----------------------------------------------------------------------
    // Fila 5 — Brillo (y=168)
    // -----------------------------------------------------------------------
    lblBrightness = Label("Brillo", font, LABEL_SIZE, sf::Color::White);
    lblBrightness.setPosition(LABEL_X, 168.f);
    lblBrightness.setSize(170.f, WIDGET_H);

    sliderBrightness.setRange(0.f, 1.f);
    sliderBrightness.setValue(1.f);
    sliderBrightness.setPosition(WIDGET_X, 168.f);
    sliderBrightness.setSize(WIDGET_W, WIDGET_H);
    sliderBrightness.setOnValueChanged([](float value) {
        GameSettings::getInstance().setFloat("brightness", value);
    });

    // -----------------------------------------------------------------------
    // Botón Aplicar (x=380, y=210, w=80, h=20)
    // Aplica resolución y fullscreen que no se pueden cambiar en caliente
    // -----------------------------------------------------------------------
    btnApply = Button("Aplicar", font, 11u);
    btnApply.setPosition(380.f, 210.f);
    btnApply.setSize(80.f, 20.f);
    btnApply.setCallback([this]() {
        applySettings();
    });
}

// ---------------------------------------------------------------------------
// onActivate: sincroniza todos los widgets con los valores actuales de
// GameSettings, para reflejar correctamente el estado guardado.
// ---------------------------------------------------------------------------
void GraphicsTab::onActivate() {
    GameSettings& cfg = GameSettings::getInstance();

    // Resolución
    const std::string res = cfg.getString("resolution", "512x256");
    const std::vector<std::string> resOptions = {"512x256", "800x450", "1280x720", "1920x1080"};
    for (int i = 0; i < static_cast<int>(resOptions.size()); ++i) {
        if (resOptions[i] == res) {
            dropResolution.setSelected(i);
            break;
        }
    }

    // Fullscreen
    toggleFullscreen.setValue(cfg.getBool("fullscreen", false));

    // VSync
    toggleVSync.setValue(cfg.getBool("vsync", false));

    // Límite FPS
    float fps = cfg.getFloat("fpsLimit", 60.f);
    int fpsIdx = 1; // default "60"
    if      (fps <= 0.f)   fpsIdx = 3; // Sin limite
    else if (fps <= 30.f)  fpsIdx = 0;
    else if (fps <= 60.f)  fpsIdx = 1;
    else                   fpsIdx = 2;
    dropFPS.setSelected(fpsIdx);

    // Brillo
    sliderBrightness.setValue(cfg.getFloat("brightness", 1.f));
}

// ---------------------------------------------------------------------------
// applySettings: guarda en GameSettings las opciones que requieren recrear
// la ventana. En este proyecto no se recrea la ventana directamente ya que
// no hay acceso a ella desde aquí; se persistirán al salir del juego.
// ---------------------------------------------------------------------------
void GraphicsTab::applySettings() {
    GameSettings& cfg = GameSettings::getInstance();

    // Resolución seleccionada
    cfg.setString("resolution", dropResolution.getSelectedOption());

    // Fullscreen
    cfg.setBool("fullscreen", toggleFullscreen.getValue());

    // Persiste todos los cambios al disco
    cfg.save();
}

// ---------------------------------------------------------------------------
// handleEvent: delega a cada widget
// ---------------------------------------------------------------------------
void GraphicsTab::handleEvent(const sf::Event& event) {
    dropResolution.handleEvent(event);
    toggleFullscreen.handleEvent(event);
    toggleVSync.handleEvent(event);
    dropFPS.handleEvent(event);
    sliderBrightness.handleEvent(event);
    btnApply.handleEvent(event);
}

// ---------------------------------------------------------------------------
// update
// ---------------------------------------------------------------------------
void GraphicsTab::update(float dt) {
    dropResolution.update(dt);
    toggleFullscreen.update(dt);
    toggleVSync.update(dt);
    dropFPS.update(dt);
    sliderBrightness.update(dt);
    btnApply.update(dt);
}

// ---------------------------------------------------------------------------
// draw: panel primero, luego labels, luego widgets
// ---------------------------------------------------------------------------
void GraphicsTab::draw(sf::RenderWindow& window) {
    window.draw(panel);

    lblResolution.draw(window);
    lblFullscreen.draw(window);
    lblVSync.draw(window);
    lblFPS.draw(window);
    lblBrightness.draw(window);

    dropResolution.draw(window);
    toggleFullscreen.draw(window);
    toggleVSync.draw(window);
    dropFPS.draw(window);
    sliderBrightness.draw(window);
    btnApply.draw(window);
}
