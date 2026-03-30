#pragma once
#ifndef DEBUG_OVERLAY_H
#define DEBUG_OVERLAY_H
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <functional>

// ---------------------------------------------------------------------------
// DebugLine — una línea de información que se refresca cada frame.
// El valueProvider es una lambda que retorna el valor actual del dato.
// ---------------------------------------------------------------------------
struct DebugLine {
    std::string                  label;
    std::function<std::string()> valueProvider;
};

// ---------------------------------------------------------------------------
// DebugOverlay — panel de debug en esquina superior izquierda.
//
// Se activa/desactiva con F3.
// Principio Open/Closed: se añaden nuevas líneas con registerLine() sin
// modificar esta clase. El sistema de renderizado es cerrado a cambio.
// Singleton de Meyer (thread-safe desde C++11).
// ---------------------------------------------------------------------------
class DebugOverlay {
public:
    static DebugOverlay& getInstance();

    // Inyecta la fuente compartida; debe llamarse antes del primer draw.
    void setFont(const sf::Font& font);

    // Invierte la visibilidad del panel.
    void toggle();
    bool isVisible() const;

    // Registra una línea de debug. OCP: se añaden líneas desde fuera sin
    // tocar DebugOverlay.
    void registerLine(const std::string& label,
                      std::function<std::string()> provider);

    void handleEvent(const sf::Event& event);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    DebugOverlay() = default;

    // No copiable ni movible — es un singleton.
    DebugOverlay(const DebugOverlay&)            = delete;
    DebugOverlay& operator=(const DebugOverlay&) = delete;

    bool visible = false;

    std::vector<DebugLine>    lines;
    std::vector<sf::Text>     renderedTexts;
    sf::RectangleShape        background;
    const sf::Font*           font       = nullptr;

    float fps        = 0.f;
    float fpsTimer   = 0.f;
    int   frameCount = 0;

    // Reconstruye el vector de sf::Text desde las líneas registradas.
    void rebuildTexts();
};
#endif
