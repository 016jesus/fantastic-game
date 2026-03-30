#pragma once
#ifndef DROPDOWN_H
#define DROPDOWN_H
#include "ui/widgets/UIWidget.h"
#include <vector>
#include <string>
#include <functional>

// ---------------------------------------------------------------------------
// Dropdown: selector combo-box. Muestra un header con la opción activa;
// al hacer click despliega todas las opciones debajo. Escape o click fuera
// cierra sin cambiar la selección.
// ---------------------------------------------------------------------------
class Dropdown : public UIWidget {
public:
    using SelectCallback = std::function<void(int index, const std::string& item)>;

    Dropdown();
    void setFont(const sf::Font& font, unsigned size = 13);
    void setOptions(const std::vector<std::string>& options);
    void setSelected(int index);
    int  getSelectedIndex() const;
    const std::string& getSelectedOption() const;
    void setOnSelected(SelectCallback cb);

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void setPosition(float x, float y) override;
    void setSize(float w, float h) override;
    sf::FloatRect getBounds() const override;
    bool isHovered() const override;

private:
    sf::RectangleShape header;       // fila siempre visible
    sf::Text           headerText;
    sf::Text           arrowText;    // "v" o "^"

    std::vector<sf::RectangleShape> optionBgs;
    std::vector<sf::Text>           optionTexts;
    std::vector<std::string>        options;

    const sf::Font* font     = nullptr;
    unsigned        fontSize = 13;

    int  selectedIndex = 0;
    bool expanded      = false;
    bool hovered       = false;
    SelectCallback callback;

    float x = 0.f, y = 0.f, w = 160.f, h = 24.f;

    void buildOptionWidgets();
    int  getHoveredOption(float mouseY) const;
    void rebuildLayout();
};

#endif
