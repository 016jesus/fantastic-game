#include "MenuState.h"
#include "GameStateManager.h"
// Forward declaration de PlayingState para evitar include circular en el header.
// La inclusión completa sólo se necesita en startGame().
#include "PlayingState.h"
#include <memory>

MenuState::MenuState(GameStateManager* gsm) {
    // El puntero al gestor se almacena en el miembro protegido de IGameState
    this->gsm = gsm;
}

// ---------------------------------------------------------------------------
// onEnter: carga recursos y configura la UI del menú
// ---------------------------------------------------------------------------
void MenuState::onEnter() {
    playerName.clear();

    // Intenta cargar la fuente primero en minúsculas, luego en mayúsculas
    fontLoaded = font.loadFromFile("minecraft.otf");
    if (!fontLoaded) {
        fontLoaded = font.loadFromFile("Minecraft.ttf");
    }

    // Tamaño de ventana conocido (512x256); setupUI usa ese valor
    setupUI(sf::Vector2u(512u, 256u));
}

// ---------------------------------------------------------------------------
// setupUI: configura posiciones y estilos de todos los elementos visuales
// ---------------------------------------------------------------------------
void MenuState::setupUI(const sf::Vector2u& windowSize) {
    const float cx = windowSize.x / 2.f;   // centro horizontal
    const float cy = windowSize.y / 2.f;   // centro vertical

    // --- Título ---
    titleText.setFont(font);
    titleText.setString("The Legend of Tilin");
    titleText.setCharacterSize(40u);
    titleText.setFillColor(sf::Color::White);
    // Centrar por el bounds del texto
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.left + titleBounds.width / 2.f,
                        titleBounds.top  + titleBounds.height / 2.f);
    titleText.setPosition(cx, cy - 80.f);

    // --- Prompt "Ingresa tu nombre:" ---
    promptText.setFont(font);
    promptText.setString("Ingresa tu nombre:");
    promptText.setCharacterSize(20u);
    promptText.setFillColor(sf::Color::White);
    sf::FloatRect promptBounds = promptText.getLocalBounds();
    promptText.setOrigin(promptBounds.left + promptBounds.width / 2.f,
                         promptBounds.top  + promptBounds.height / 2.f);
    promptText.setPosition(cx, cy - 25.f);

    // --- Caja de input ---
    inputBox.setSize(sf::Vector2f(300.f, 40.f));
    inputBox.setFillColor(sf::Color::Black);
    inputBox.setOutlineColor(sf::Color::White);
    inputBox.setOutlineThickness(2.f);
    inputBox.setOrigin(150.f, 20.f);   // centrar sobre su eje
    inputBox.setPosition(cx, cy + 10.f);

    // --- Texto del nombre dentro de la caja ---
    nameText.setFont(font);
    nameText.setString("");
    nameText.setCharacterSize(18u);
    nameText.setFillColor(sf::Color::White);
    // Se reposiciona en render() para seguir al texto escrito
    nameText.setPosition(cx - 145.f, cy - 5.f);

    // --- Instrucción inferior ---
    instructionText.setFont(font);
    instructionText.setString("Presiona ENTER para comenzar");
    instructionText.setCharacterSize(16u);
    instructionText.setFillColor(sf::Color(180, 180, 180));
    sf::FloatRect instrBounds = instructionText.getLocalBounds();
    instructionText.setOrigin(instrBounds.left + instrBounds.width / 2.f,
                              instrBounds.top  + instrBounds.height / 2.f);
    instructionText.setPosition(cx, cy + 55.f);
}

// ---------------------------------------------------------------------------
// handleEvent: captura texto y teclas de control
// ---------------------------------------------------------------------------
void MenuState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        // Sólo caracteres ASCII imprimibles (>= 32) y backspace (8)
        if (event.text.unicode == 8u) {
            // Backspace: elimina el último carácter
            if (!playerName.empty()) {
                playerName.pop_back();
            }
        } else if (event.text.unicode >= 32u && event.text.unicode < 128u) {
            // Añade carácter hasta el límite de 15
            if (playerName.size() < 15u) {
                playerName += static_cast<char>(event.text.unicode);
            }
        }
        // Actualiza el texto visible
        nameText.setString(playerName);
    }

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Return) {
            if (!playerName.empty()) {
                startGame();
            }
        }
    }
}

// ---------------------------------------------------------------------------
// update: el menú es estático, no necesita lógica de tiempo
// ---------------------------------------------------------------------------
void MenuState::update(float /*deltaTime*/) {
    // Sin lógica de actualización por ahora
}

// ---------------------------------------------------------------------------
// render: dibuja todos los elementos del menú sobre fondo negro
// ---------------------------------------------------------------------------
void MenuState::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);

    if (fontLoaded) {
        window.draw(titleText);
        window.draw(promptText);
        window.draw(inputBox);
        window.draw(nameText);
        window.draw(instructionText);
    } else {
        // Fallback sin fuente: al menos muestra el título como texto básico
        // (SFML no puede dibujar Text sin fuente válida, no hacemos nada más)
    }
}

// ---------------------------------------------------------------------------
// startGame: transiciona al estado de juego reemplazando el menú
// ---------------------------------------------------------------------------
void MenuState::startGame() {
    gsm->replace(std::make_unique<PlayingState>(gsm, playerName));
}
