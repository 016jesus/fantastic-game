#pragma once
#ifndef SCROLLABLE_PANEL_H
#define SCROLLABLE_PANEL_H
#include "UIWidget.h"
#include <vector>
#include <memory>

// ---------------------------------------------------------------------------
// ScrollablePanel: contenedor con scroll vertical. Renderiza sus hijos
// dentro de un sf::View recortado al bounds del panel y aplica offset
// vertical según scrollY. Muestra scrollbar visual en el borde derecho.
// ---------------------------------------------------------------------------
class ScrollablePanel : public UIWidget {
public:
    ScrollablePanel();

    void setBackground(sf::Color color);
    void setBorder(sf::Color color, float thickness = 1.f);

    // Añade un widget al panel. El panel toma ownership.
    // relX/relY son posiciones relativas al origen del panel (sin contar scroll).
    void addWidget(std::unique_ptr<UIWidget> widget, float relX, float relY);

    void scrollBy(float delta);
    void scrollTo(float targetY);
    float getScrollY() const;
    float getContentHeight() const;

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void setPosition(float x, float y) override;
    void setSize(float w, float h) override;
    sf::FloatRect getBounds() const override;
    bool isHovered() const override;

private:
    struct WidgetEntry {
        std::unique_ptr<UIWidget> widget;
        float relX = 0.f;
        float relY = 0.f;
    };
    std::vector<WidgetEntry> widgets;

    sf::RectangleShape background;
    sf::RectangleShape scrollbarTrack;
    sf::RectangleShape scrollThumb;

    float panX = 0.f, panY = 0.f, panW = 300.f, panH = 200.f;
    float scrollY  = 0.f;
    float contentH = 0.f;
    bool  hovered  = false;

    static constexpr float SCROLLBAR_W = 8.f;
    static constexpr float SCROLL_SPEED = 30.f;

    void updateScrollbar();
    void updateWidgetPositions();
    float maxScroll() const;
};

#endif
