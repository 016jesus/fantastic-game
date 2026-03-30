#pragma once
#ifndef HELP_TAB_H
#define HELP_TAB_H
#include "ITab.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class HelpTab : public ITab {
public:
    explicit HelpTab(const sf::Font& font);
    std::string getTitle() const override { return "Ayuda"; }
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

private:
    struct HelpSection {
        std::string              heading;
        std::vector<std::string> lines;
    };
    std::vector<HelpSection> sections;

    std::vector<sf::Text> renderedTexts;
    sf::RectangleShape panel;
    sf::RectangleShape leftColumn;
    sf::RectangleShape rightColumn;
    const sf::Font& font;
    float scrollOffset = 0.f;

    void buildContent();
    void buildRenderTexts();
};
#endif
