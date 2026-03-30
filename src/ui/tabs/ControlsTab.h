#pragma once
#ifndef CONTROLS_TAB_H
#define CONTROLS_TAB_H
#include "ui/tabs/ITab.h"
#include "ui/widgets/KeyBindWidget.h"
#include "ui/widgets/Button.h"
#include "ui/widgets/Label.h"
#include <vector>
#include <memory>

class ControlsTab : public ITab {
public:
    explicit ControlsTab(const sf::Font& font);
    std::string getTitle() const override { return "Controles"; }
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void onActivate() override;

private:
    std::vector<KeyBindWidget> bindWidgets;
    Button   btnReset;
    Label    lblTitle;
    Label    lblHint;
    sf::RectangleShape panel;
    const sf::Font& font;

    // Desplazamiento vertical para scroll manual con flechas
    float scrollOffset = 0.f;
    static constexpr float ROW_HEIGHT   = 26.f;
    static constexpr float SCROLL_SPEED = 30.f;

    void buildLayout();
    void saveBindings();
};
#endif
