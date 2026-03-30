#pragma once
#ifndef CREDITS_TAB_H
#define CREDITS_TAB_H
#include "ITab.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class CreditsTab : public ITab {
public:
    explicit CreditsTab(const sf::Font& font);
    std::string getTitle() const override { return "Creditos"; }
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void onActivate() override;
    void onDeactivate() override;

private:
    struct CreditEntry {
        std::string text;
        bool        isHeading;   // cabeceras en amarillo y mayor tamaño
        bool        isSeparator; // línea vacía
    };
    std::vector<CreditEntry> entries;
    std::vector<sf::Text>    rendered;

    sf::RectangleShape panel;
    sf::RectangleShape topFade;     // banda superior para enmascarar borde
    sf::RectangleShape bottomFade;  // banda inferior para enmascarar borde

    float scrollY     = 0.f;
    float totalH      = 0.f;
    float scrollSpeed = 28.f;  // px/s
    bool  autoScroll  = true;
    const sf::Font& font;

    void buildEntries();
    void buildRendered();
    float calcTotalHeight() const;
};
#endif
