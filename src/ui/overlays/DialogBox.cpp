#include "ui/overlays/DialogBox.h"

// ---------------------------------------------------------------------------
// Constructor: configura las formas del panel de diálogo.
// El box ocupa el ancho completo en y=190, con altura de 60px.
// ---------------------------------------------------------------------------
DialogBox::DialogBox() {
    // Panel principal oscuro semitransparente
    box.setSize(sf::Vector2f(512.f, 60.f));
    box.setFillColor(sf::Color(0, 0, 0, 200));
    box.setPosition(0.f, 190.f);

    // Borde blanco de 1px sobre el panel
    border.setSize(sf::Vector2f(512.f, 60.f));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::White);
    border.setOutlineThickness(1.f);
    border.setPosition(0.f, 190.f);

    // Texto del hablante (amarillo, encima del box)
    speakerText.setCharacterSize(10u);
    speakerText.setFillColor(sf::Color::Yellow);

    // Texto del contenido (blanco, dentro del box)
    contentText.setCharacterSize(9u);
    contentText.setFillColor(sf::Color::White);

    // Indicador de avance "▶" en esquina inferior derecha
    arrowText.setCharacterSize(9u);
    arrowText.setFillColor(sf::Color(200, 200, 200));
    arrowText.setString(">");   // carácter ASCII compatible con la fuente Minecraft
}

// ---------------------------------------------------------------------------
// loadFont: asigna la fuente a todos los textos del diálogo
// ---------------------------------------------------------------------------
void DialogBox::loadFont(const sf::Font& f) {
    font = &f;
    speakerText.setFont(f);
    contentText.setFont(f);
    arrowText.setFont(f);
}

// ---------------------------------------------------------------------------
// open: inicializa el diálogo con hablante y lista de líneas
// ---------------------------------------------------------------------------
void DialogBox::open(const std::string& spk, const std::vector<std::string>& ls) {
    speaker     = spk;
    lines       = ls;
    currentLine = 0;
    open_       = true;
    layout();
}

// ---------------------------------------------------------------------------
// advance: avanza a la siguiente línea.
// Retorna false cuando se agotan las líneas (señal para cerrar el diálogo).
// ---------------------------------------------------------------------------
bool DialogBox::advance() {
    currentLine++;
    if (currentLine >= lines.size()) {
        open_ = false;
        return false;
    }
    layout();
    return true;
}

// ---------------------------------------------------------------------------
// isOpen: consulta si el diálogo está activo
// ---------------------------------------------------------------------------
bool DialogBox::isOpen() const {
    return open_;
}

// ---------------------------------------------------------------------------
// setWindowSize: actualiza dimensiones de la ventana y reposiciona el panel
// ---------------------------------------------------------------------------
void DialogBox::setWindowSize(float w, float h) {
    winW = w;
    winH = h;

    // Reposiciona el panel en la parte inferior de la ventana
    float boxY = h - 66.f;   // 6px de margen inferior
    box.setSize(sf::Vector2f(w, 60.f));
    box.setPosition(0.f, boxY);
    border.setSize(sf::Vector2f(w, 60.f));
    border.setPosition(0.f, boxY);

    if (open_) layout();
}

// ---------------------------------------------------------------------------
// layout: recalcula posiciones y cadenas de los textos según la línea actual
// ---------------------------------------------------------------------------
void DialogBox::layout() {
    if (!font) return;

    float boxY = box.getPosition().y;

    // Nombre del hablante en amarillo, 2px encima del borde superior del box
    speakerText.setString(speaker + ":");
    speakerText.setPosition(6.f, boxY - 14.f);

    // Contenido de la línea actual con padding de 6px
    if (currentLine < lines.size()) {
        contentText.setString(lines[currentLine]);
    }
    contentText.setPosition(6.f, boxY + 6.f);

    // Indicador de avance en esquina inferior derecha del panel
    arrowText.setPosition(winW - 14.f, boxY + 60.f - 14.f);
}

// ---------------------------------------------------------------------------
// draw: dibuja el panel de diálogo si está abierto
// ---------------------------------------------------------------------------
void DialogBox::draw(sf::RenderWindow& window) {
    if (!open_) return;

    window.draw(box);
    window.draw(border);
    window.draw(speakerText);
    window.draw(contentText);
    window.draw(arrowText);
}
