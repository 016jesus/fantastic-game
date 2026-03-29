#pragma once
#ifndef INVENTORY_OVERLAY_H
#define INVENTORY_OVERLAY_H
#include <SFML/Graphics.hpp>
#include "protagonista.h"
#include <vector>

// ---------------------------------------------------------------------------
// InventoryOverlay: panel de inventario del jugador.
// Permite navegar con flechas y usar ítems con ENTER.
// ---------------------------------------------------------------------------
class InventoryOverlay {
public:
    InventoryOverlay();
    void loadFont(const sf::Font& font);
    void toggle();
    bool isOpen() const;

    // Procesa teclas dentro del overlay (flechas para navegar, ENTER para usar).
    // Retorna true si consumió el evento.
    bool handleEvent(const sf::Event& event, Protagonista& player);

    void draw(sf::RenderWindow& window, const Protagonista& player);

private:
    bool open_ = false;
    int selectedIndex = 0;
    sf::RectangleShape panel;
    sf::RectangleShape border;
    sf::RectangleShape highlight;
    sf::Text titleText;
    sf::Text emptyText;
    sf::Text hintText;
    std::vector<sf::Text> itemTexts;
    const sf::Font* font = nullptr;

    void buildItemList(const Protagonista& player);
    std::string getItemName(const Objetos* obj) const;
};
#endif
