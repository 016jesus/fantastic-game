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

    // Verificar si existe un save para mostrar la opción "Continuar"
    hasSaveGame = SaveSystem::getInstance().hasSave();
    if (hasSaveGame && fontLoaded) {
        continueText.setFont(font);
        continueText.setString("C: Continuar partida");
        continueText.setCharacterSize(16u);
        continueText.setFillColor(sf::Color::Green);

        // Posicionar debajo del texto de instrucciones (instructionText está en cy+80)
        const float cx = 256.f;   // mitad de 512
        const float cy = 128.f;   // mitad de 256
        sf::FloatRect cb = continueText.getLocalBounds();
        continueText.setOrigin(cb.left + cb.width / 2.f,
                               cb.top  + cb.height / 2.f);
        continueText.setPosition(cx, cy + 100.f);
    }
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
    nameText.setPosition(cx - 145.f, cy - 5.f);

    // -----------------------------------------------------------------------
    // Fila de selección de género (~25 px debajo del borde inferior del
    // inputBox, que está en cy+10+20 = cy+30 → fila en cy+55)
    // -----------------------------------------------------------------------

    // Highlight: rectángulo semitransparente amarillo detrás del botón activo
    genderHighlight.setSize(sf::Vector2f(110.f, 20.f));
    genderHighlight.setFillColor(sf::Color(255, 220, 0, 90));
    genderHighlight.setOutlineColor(sf::Color(255, 220, 0, 180));
    genderHighlight.setOutlineThickness(1.f);

    // Etiqueta "Genero:"
    genderLabel.setFont(font);
    genderLabel.setString("Genero:");
    genderLabel.setCharacterSize(14u);
    genderLabel.setFillColor(sf::Color::White);
    genderLabel.setPosition(cx - 150.f, cy + 53.f);

    // Botón "[M] Masculino"
    genderMaleBtn.setFont(font);
    genderMaleBtn.setString("[M] Masculino");
    genderMaleBtn.setCharacterSize(14u);
    genderMaleBtn.setFillColor(sf::Color::White);
    genderMaleBtn.setPosition(cx - 50.f, cy + 53.f);

    // Botón "[F] Femenino"
    genderFemaleBtn.setFont(font);
    genderFemaleBtn.setString("[F] Femenino");
    genderFemaleBtn.setCharacterSize(14u);
    genderFemaleBtn.setFillColor(sf::Color::White);
    genderFemaleBtn.setPosition(cx + 65.f, cy + 53.f);

    // Posiciona el highlight según la selección inicial (Male)
    updateGenderHighlight();

    // --- Instrucción inferior — desplazada hacia abajo para no solaparse ---
    instructionText.setFont(font);
    instructionText.setString("Presiona ENTER para comenzar");
    instructionText.setCharacterSize(16u);
    instructionText.setFillColor(sf::Color(180, 180, 180));
    sf::FloatRect instrBounds = instructionText.getLocalBounds();
    instructionText.setOrigin(instrBounds.left + instrBounds.width / 2.f,
                              instrBounds.top  + instrBounds.height / 2.f);
    instructionText.setPosition(cx, cy + 80.f);
}

// ---------------------------------------------------------------------------
// handleEvent: captura texto, controla selección de género y arranca el juego
// ---------------------------------------------------------------------------
void MenuState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        // Sólo caracteres ASCII imprimibles (>= 32) y backspace (8)
        if (event.text.unicode == 8u) {
            if (!playerName.empty()) {
                playerName.pop_back();
            }
        } else if (event.text.unicode >= 32u && event.text.unicode < 128u) {
            if (playerName.size() < 15u) {
                playerName += static_cast<char>(event.text.unicode);
            }
        }
        nameText.setString(playerName);
    }

    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            // Confirmar e iniciar juego
            case sf::Keyboard::Return:
                if (!playerName.empty()) {
                    startGame();
                }
                break;

            // Tecla C → continuar partida guardada
            case sf::Keyboard::C:
                if (hasSaveGame) {
                    SaveData sd;
                    if (SaveSystem::getInstance().load(sd)) {
                        gsm->replace(std::make_unique<PlayingState>(gsm, sd.playerName, sd.gender));
                    }
                }
                break;

            // Tecla M → seleccionar Masculino directamente
            case sf::Keyboard::M:
                selectedGender = Gender::Male;
                updateGenderHighlight();
                break;

            // Tecla F → seleccionar Femenino directamente
            case sf::Keyboard::F:
                selectedGender = Gender::Female;
                updateGenderHighlight();
                break;

            // Flecha izquierda → Masculino
            case sf::Keyboard::Left:
                selectedGender = Gender::Male;
                updateGenderHighlight();
                break;

            // Flecha derecha → Femenino
            case sf::Keyboard::Right:
                selectedGender = Gender::Female;
                updateGenderHighlight();
                break;

            default:
                break;
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

        // --- Fila de selección de género ---
        window.draw(genderHighlight);
        window.draw(genderLabel);
        window.draw(genderMaleBtn);
        window.draw(genderFemaleBtn);

        window.draw(instructionText);

        if (hasSaveGame) {
            window.draw(continueText);
        }
    }
    // Fallback sin fuente: SFML no puede dibujar Text sin fuente válida
}

// ---------------------------------------------------------------------------
// updateGenderHighlight: reposiciona el rectángulo amarillo detrás del botón
// actualmente seleccionado.
// ---------------------------------------------------------------------------
void MenuState::updateGenderHighlight() {
    if (selectedGender == Gender::Male) {
        // Alinea el highlight con genderMaleBtn
        sf::Vector2f pos = genderMaleBtn.getPosition();
        genderHighlight.setPosition(pos.x - 3.f, pos.y - 1.f);
    } else {
        // Alinea el highlight con genderFemaleBtn
        sf::Vector2f pos = genderFemaleBtn.getPosition();
        genderHighlight.setPosition(pos.x - 3.f, pos.y - 1.f);
    }
}

// ---------------------------------------------------------------------------
// startGame: transiciona al estado de juego pasando nombre y género
// ---------------------------------------------------------------------------
void MenuState::startGame() {
    const std::string genderStr =
        (selectedGender == Gender::Male) ? "male" : "female";
    gsm->replace(std::make_unique<PlayingState>(gsm, playerName, genderStr));
}
