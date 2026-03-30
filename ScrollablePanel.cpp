#include "ScrollablePanel.h"
#include <algorithm>

ScrollablePanel::ScrollablePanel() {
    background.setFillColor(sf::Color(30, 30, 45));

    scrollbarTrack.setFillColor(sf::Color(50, 50, 65));
    scrollbarTrack.setSize({SCROLLBAR_W, panH});
    scrollbarTrack.setPosition(panX + panW - SCROLLBAR_W, panY);

    scrollThumb.setFillColor(sf::Color(100, 100, 140));
}

void ScrollablePanel::setBackground(sf::Color color) {
    background.setFillColor(color);
}

void ScrollablePanel::setBorder(sf::Color color, float thickness) {
    background.setOutlineColor(color);
    background.setOutlineThickness(thickness);
}

void ScrollablePanel::addWidget(std::unique_ptr<UIWidget> widget,
                                float relX, float relY) {
    // Actualiza la altura del contenido
    sf::FloatRect wb = widget->getBounds();
    float bottom = relY + wb.height;
    if (bottom + 8.f > contentH) contentH = bottom + 8.f;

    widgets.push_back({std::move(widget), relX, relY});
    updateWidgetPositions();
    updateScrollbar();
}

void ScrollablePanel::scrollBy(float delta) {
    scrollTo(scrollY + delta);
}

void ScrollablePanel::scrollTo(float targetY) {
    scrollY = std::clamp(targetY, 0.f, maxScroll());
    updateWidgetPositions();
    updateScrollbar();
}

float ScrollablePanel::getScrollY() const {
    return scrollY;
}

float ScrollablePanel::getContentHeight() const {
    return contentH;
}

void ScrollablePanel::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mouse(static_cast<float>(event.mouseMove.x),
                           static_cast<float>(event.mouseMove.y));
        hovered = getBounds().contains(mouse);
    }

    if (event.type == sf::Event::MouseWheelScrolled && hovered) {
        scrollBy(-event.mouseWheelScroll.delta * SCROLL_SPEED);
    }

    // Propaga eventos a los hijos (respetando el area visible)
    for (auto& entry : widgets) {
        entry.widget->handleEvent(event);
    }
}

void ScrollablePanel::update(float dt) {
    for (auto& entry : widgets)
        entry.widget->update(dt);
}

void ScrollablePanel::draw(sf::RenderWindow& window) {
    // Fondo del panel
    window.draw(background);

    // Guarda la view original
    sf::View originalView = window.getView();

    // Crea un viewport recortado al area del panel (en coordenadas normalizadas)
    sf::Vector2u winSize = window.getSize();
    float vx = panX / static_cast<float>(winSize.x);
    float vy = panY / static_cast<float>(winSize.y);
    float vw = (panW - SCROLLBAR_W) / static_cast<float>(winSize.x);
    float vh = panH / static_cast<float>(winSize.y);

    sf::View panelView(sf::FloatRect(panX, panY + scrollY,
                                    panW - SCROLLBAR_W, panH));
    panelView.setViewport(sf::FloatRect(vx, vy, vw, vh));
    window.setView(panelView);

    for (auto& entry : widgets)
        entry.widget->draw(window);

    // Restaura la view original y dibuja el scrollbar encima
    window.setView(originalView);
    window.draw(scrollbarTrack);
    window.draw(scrollThumb);
}

void ScrollablePanel::setPosition(float x, float y) {
    panX = x;
    panY = y;
    background.setPosition(x, y);
    scrollbarTrack.setPosition(x + panW - SCROLLBAR_W, y);
    updateWidgetPositions();
    updateScrollbar();
}

void ScrollablePanel::setSize(float w, float h) {
    panW = w;
    panH = h;
    background.setSize({w, h});
    scrollbarTrack.setSize({SCROLLBAR_W, h});
    scrollbarTrack.setPosition(panX + w - SCROLLBAR_W, panY);
    updateWidgetPositions();
    updateScrollbar();
}

sf::FloatRect ScrollablePanel::getBounds() const {
    return {panX, panY, panW, panH};
}

bool ScrollablePanel::isHovered() const {
    return hovered;
}

void ScrollablePanel::updateScrollbar() {
    if (contentH <= panH) {
        scrollThumb.setSize({SCROLLBAR_W, panH});
        scrollThumb.setPosition(panX + panW - SCROLLBAR_W, panY);
        return;
    }
    float ratio     = panH / contentH;
    float thumbH    = std::max(20.f, panH * ratio);
    float thumbY    = panY + (scrollY / maxScroll()) * (panH - thumbH);
    scrollThumb.setSize({SCROLLBAR_W, thumbH});
    scrollThumb.setPosition(panX + panW - SCROLLBAR_W, thumbY);
}

void ScrollablePanel::updateWidgetPositions() {
    for (auto& entry : widgets) {
        entry.widget->setPosition(panX + entry.relX,
                                  panY + entry.relY);
    }
}

float ScrollablePanel::maxScroll() const {
    return std::max(0.f, contentH - panH);
}
