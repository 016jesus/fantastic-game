#pragma once
#ifndef SHOP_OVERLAY_H
#define SHOP_OVERLAY_H
#include <SFML/Graphics.hpp>
#include "protagonista.h"
#include <vector>
#include <string>

// ---------------------------------------------------------------------------
// ShopItem: descriptor de un producto en el catálogo de la tienda
// ---------------------------------------------------------------------------
struct ShopItem {
    std::string name;
    int price;
    int type;   // 1=Sanadoras, 2=Arma basica
};

// ---------------------------------------------------------------------------
// ShopOverlay: panel de tienda del vendedor.
// Permite navegar con flechas, comprar con ENTER y cerrar con ESC.
// ---------------------------------------------------------------------------
class ShopOverlay {
public:
    ShopOverlay();
    void loadFont(const sf::Font& font);
    void open();
    void close();
    bool isOpen() const;

    // Procesa eventos de teclado dentro del overlay.
    // Retorna true si consumió el evento.
    bool handleEvent(const sf::Event& event, Protagonista& player);

    // Avanza el temporizador del easter egg; llamar con deltaTime cada frame
    void update(float deltaTime);

    void draw(sf::RenderWindow& window, Protagonista& player);

private:
    bool open_ = false;
    int selectedIndex = 0;
    sf::RectangleShape panel;
    sf::RectangleShape border;
    sf::RectangleShape highlight;
    sf::Text titleText;
    sf::Text moneyText;
    sf::Text hintText;
    sf::Text feedbackText;      // mensaje transitorio "!Comprado!" / "Sin fondos"
    bool showFeedback = false;
    std::vector<sf::Text> itemTexts;
    std::vector<ShopItem> catalog;
    const sf::Font* font = nullptr;

    void initCatalog();
    void buildUI();
    void buySelected(Protagonista& player);

    // Easter egg: se activa tras 3 compras exitosas
    int purchaseCount = 0;
    bool easterEggShown = false;
    sf::Text easterEggText;
    float easterEggTimer = 0.f;
};
#endif
