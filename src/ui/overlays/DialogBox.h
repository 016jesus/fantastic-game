#pragma once
#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>

// ---------------------------------------------------------------------------
// DialogBox: componente de UI para mostrar diálogos de NPC avanzables con
// ENTER o E. Muestra un panel semitransparente en la parte inferior de la
// pantalla con nombre del hablante y texto actual.
// ---------------------------------------------------------------------------
class DialogBox {
public:
    DialogBox();
    void loadFont(const sf::Font& font);

    // Abre el diálogo con una lista de líneas de texto
    void open(const std::string& speaker, const std::vector<std::string>& lines);

    // Avanza a la siguiente línea.
    // Retorna false cuando se acaban las líneas (señal para cerrar).
    bool advance();

    bool isOpen() const;
    void draw(sf::RenderWindow& window);
    void setWindowSize(float w, float h);

private:
    sf::RectangleShape box;
    sf::RectangleShape border;
    sf::Text speakerText;
    sf::Text contentText;
    sf::Text arrowText;        // indicador "▶" de avance
    std::vector<std::string> lines;
    std::string speaker;
    std::size_t currentLine = 0;
    bool open_ = false;
    float winW = 512.f, winH = 256.f;
    const sf::Font* font = nullptr;

    void layout();
};
#endif
