#pragma once
#ifndef TAB_BAR_H
#define TAB_BAR_H
#include "UIWidget.h"
#include <vector>
#include <string>
#include <functional>

// ---------------------------------------------------------------------------
// TabBar: barra horizontal de pestañas. Pestaña activa en azul-blanco,
// inactivas en gris, hover en color intermedio.
// Cada tab ocupa anchura igual = w / tabs.size().
// ---------------------------------------------------------------------------
class TabBar : public UIWidget {
public:
    using TabCallback = std::function<void(int index)>;

    TabBar();
    void setFont(const sf::Font& font, unsigned size = 12);
    void addTab(const std::string& title);
    void setActiveTab(int index);
    int  getActiveTab() const;
    void setOnTabChanged(TabCallback cb);

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void setPosition(float x, float y) override;
    void setSize(float w, float h) override;
    sf::FloatRect getBounds() const override;
    bool isHovered() const override;

private:
    struct Tab {
        std::string        title;
        sf::RectangleShape bg;
        sf::Text           text;
        bool               hovered = false;
    };
    std::vector<Tab> tabs;

    const sf::Font* font     = nullptr;
    unsigned        fontSize = 12;
    int             activeTab = 0;
    TabCallback     callback;

    float x = 0.f, y = 0.f, w = 512.f, h = 24.f;

    void layoutTabs();
};

#endif
