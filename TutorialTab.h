#pragma once
#ifndef TUTORIAL_TAB_H
#define TUTORIAL_TAB_H
#include "ITab.h"
#include "Button.h"
#include "Label.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class TutorialTab : public ITab {
public:
    explicit TutorialTab(const sf::Font& font);
    std::string getTitle() const override { return "Tutorial"; }
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void onActivate() override;

private:
    struct TutorialPage {
        std::string title;
        std::string body;          // texto multilínea con \n
        sf::Color   accentColor;
    };
    std::vector<TutorialPage> pages;
    int currentPage = 0;

    sf::RectangleShape panel;
    sf::RectangleShape accentBar;  // barra de color arriba del título
    sf::Text           titleText;
    sf::Text           bodyText;
    sf::Text           pageIndicator;   // "1 / 5"
    Button             btnPrev;
    Button             btnNext;
    const sf::Font&    font;

    void buildPages();
    void showPage(int index);
};
#endif
