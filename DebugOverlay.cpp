#include "DebugOverlay.h"

// ---------------------------------------------------------------------------
// getInstance — singleton de Meyer.
// ---------------------------------------------------------------------------
DebugOverlay& DebugOverlay::getInstance() {
    static DebugOverlay instance;
    return instance;
}

// ---------------------------------------------------------------------------
// setFont — inyecta la fuente compartida del proyecto.
// ---------------------------------------------------------------------------
void DebugOverlay::setFont(const sf::Font& f) {
    font = &f;
}

// ---------------------------------------------------------------------------
// toggle — invierte la visibilidad del panel.
// ---------------------------------------------------------------------------
void DebugOverlay::toggle() {
    visible = !visible;
}

bool DebugOverlay::isVisible() const {
    return visible;
}

// ---------------------------------------------------------------------------
// registerLine — añade una línea de debug con su proveedor de valor.
// OCP: el caller extiende la información mostrada sin tocar esta clase.
// ---------------------------------------------------------------------------
void DebugOverlay::registerLine(const std::string& label,
                                 std::function<std::string()> provider)
{
    DebugLine dl;
    dl.label         = label;
    dl.valueProvider = std::move(provider);
    lines.push_back(std::move(dl));
}

// ---------------------------------------------------------------------------
// handleEvent — detecta F3 y llama toggle().
// ---------------------------------------------------------------------------
void DebugOverlay::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::F3)
    {
        toggle();
    }
}

// ---------------------------------------------------------------------------
// update — calcula FPS y reconstruye los textos si el panel está visible.
// ---------------------------------------------------------------------------
void DebugOverlay::update(float deltaTime) {
    ++frameCount;
    fpsTimer += deltaTime;

    if (fpsTimer >= 0.5f) {
        fps        = static_cast<float>(frameCount) / fpsTimer;
        frameCount = 0;
        fpsTimer   = 0.f;
    }

    if (visible) {
        rebuildTexts();
    }
}

// ---------------------------------------------------------------------------
// rebuildTexts — reconstruye el vector de sf::Text desde las líneas
// registradas más la línea de FPS que siempre va primera.
// ---------------------------------------------------------------------------
void DebugOverlay::rebuildTexts() {
    if (!font) return;

    // +1 para la línea de FPS que siempre encabeza el panel.
    const std::size_t totalLines = lines.size() + 1;
    renderedTexts.resize(totalLines);

    auto configureText = [this](sf::Text& t, const std::string& str, float y) {
        t.setFont(*font);
        t.setCharacterSize(10u);
        t.setFillColor(sf::Color(180, 255, 180));
        t.setString(str);
        t.setPosition(4.f, y);
    };

    // Primera línea: FPS.
    configureText(renderedTexts[0],
                  "FPS: " + std::to_string(static_cast<int>(fps)),
                  10.f);

    // Líneas registradas.
    for (std::size_t i = 0; i < lines.size(); ++i) {
        const std::string value = lines[i].valueProvider
                                  ? lines[i].valueProvider()
                                  : "?";
        const std::string str   = lines[i].label + ": " + value;
        const float y           = 10.f + static_cast<float>(i + 1) * 14.f;
        configureText(renderedTexts[i + 1], str, y);
    }
}

// ---------------------------------------------------------------------------
// draw — dibuja el panel de fondo y todos los textos si está visible.
// ---------------------------------------------------------------------------
void DebugOverlay::draw(sf::RenderWindow& window) {
    if (!visible) return;

    // +1 por la línea de FPS.
    const float panelH = 14.f * static_cast<float>(lines.size() + 1) + 20.f;

    background.setSize(sf::Vector2f(200.f, panelH));
    background.setPosition(0.f, 0.f);
    background.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(background);

    for (auto& t : renderedTexts) {
        window.draw(t);
    }
}
