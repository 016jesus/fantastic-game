#include "PlayingState.h"
#include "GameStateManager.h"

// ---------------------------------------------------------------------------
// Constructor: inicializa jugador, fondo y HUD
// ---------------------------------------------------------------------------
PlayingState::PlayingState(GameStateManager* gsm, const std::string& playerName)
    : player(MAX_HP, 5, playerName, 0)
{
    // Guarda el puntero al gestor (miembro protegido de IGameState)
    this->gsm = gsm;

    // Configura el fondo verde oscuro (sin textura por defecto)
    background.setSize(512.f, 256.f);
    background.setColor(sf::Color(34, 139, 34));

    // --- Barra de vida (HUD) ---
    // Fondo rojo oscuro, 150x15 en esquina superior izquierda
    hpBarBg.setSize(sf::Vector2f(150.f, 15.f));
    hpBarBg.setFillColor(sf::Color(139, 0, 0));
    hpBarBg.setPosition(10.f, 10.f);

    // Fill rojo brillante — se actualiza en updateHUD()
    hpBarFill.setSize(sf::Vector2f(150.f, 15.f));
    hpBarFill.setFillColor(sf::Color(220, 20, 20));
    hpBarFill.setPosition(10.f, 10.f);

    // Texto de vida
    bool fontLoaded = font.loadFromFile("minecraft.otf");
    if (!fontLoaded) {
        font.loadFromFile("Minecraft.ttf");
    }

    hpText.setFont(font);
    hpText.setCharacterSize(10u);
    hpText.setFillColor(sf::Color::White);
    hpText.setPosition(12.f, 11.f);

    // Hint de pausa
    pauseHint.setFont(font);
    pauseHint.setCharacterSize(10u);
    pauseHint.setFillColor(sf::Color(200, 200, 200));
    pauseHint.setString("ESC: pausa");
    pauseHint.setPosition(10.f, 30.f);
}

// ---------------------------------------------------------------------------
// onEnter: posiciona al jugador al iniciar el estado
// ---------------------------------------------------------------------------
void PlayingState::onEnter() {
    paused    = false;
    velocityY = 0.f;
    onGround  = true;

    // Posición inicial: izquierda del mapa, sobre el suelo
    if (player.getSkin() != nullptr) {
        player.getSkin()->getSprite()->setPosition(64.f, groundY - 64.f);
    }

    updateHUD();
}

// ---------------------------------------------------------------------------
// handleEvent: ESC pausa/reanuda; espacio salta
// ---------------------------------------------------------------------------
void PlayingState::handleEvent(const sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    switch (event.key.code) {
        case sf::Keyboard::Escape:
            paused = !paused;
            break;

        case sf::Keyboard::Space:
            // Saltar sólo si está en el suelo
            if (onGround) {
                velocityY = JUMP_FORCE;
                onGround  = false;
            }
            break;

        default:
            break;
    }
}

// ---------------------------------------------------------------------------
// update: lógica de juego principal (se detiene si está pausado)
// ---------------------------------------------------------------------------
void PlayingState::update(float deltaTime) {
    if (paused) return;

    handleMovement(deltaTime);
    applyGravity(deltaTime);
    updateHUD();
}

// ---------------------------------------------------------------------------
// handleMovement: mueve al jugador con A/D o flechas izquierda/derecha
// La velocidad del protagonista (getVelocidad()) se usa como píxeles/segundo.
// NO se llama a player.movimientos() porque esa firma requiere sf::Keyboard*
// y mezclaría lógica de eventos con real-time input de forma inconsistente.
// ---------------------------------------------------------------------------
void PlayingState::handleMovement(float deltaTime) {
    Skins* skin = player.getSkin();
    if (skin == nullptr) return;

    sf::Sprite* sprite = skin->getSprite();
    if (sprite == nullptr) return;

    const float speed = static_cast<float>(player.getVelocidad()) * 30.f;
    sf::Vector2f pos  = sprite->getPosition();
    bool moved        = false;

    // Movimiento a la izquierda
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        pos.x -= speed * deltaTime;
        // Voltear sprite horizontalmente
        sf::FloatRect bounds = sprite->getLocalBounds();
        sprite->setOrigin(bounds.width, 0.f);
        sprite->setScale(-1.f, 1.f);
        moved = true;
    }

    // Movimiento a la derecha
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        pos.x += speed * deltaTime;
        // Restablecer orientación normal
        sprite->setOrigin(0.f, 0.f);
        sprite->setScale(1.f, 1.f);
        moved = true;
    }

    (void)moved;   // reservado para activar animación "walk" vs "idle"

    // Limitar X al ancho de la ventana con algo de margen
    const float spriteW = sprite->getGlobalBounds().width;
    if (pos.x < 0.f) pos.x = 0.f;
    if (pos.x + spriteW > 480.f) pos.x = 480.f - spriteW;

    sprite->setPosition(pos);
}

// ---------------------------------------------------------------------------
// applyGravity: aplica gravedad constante y hace snap al suelo
// ---------------------------------------------------------------------------
void PlayingState::applyGravity(float deltaTime) {
    Skins* skin = player.getSkin();
    if (skin == nullptr) return;

    sf::Sprite* sprite = skin->getSprite();
    if (sprite == nullptr) return;

    velocityY += GRAVITY * deltaTime;

    sf::Vector2f pos = sprite->getPosition();
    pos.y += velocityY * deltaTime;

    // Snap al suelo
    const float spriteH   = sprite->getGlobalBounds().height;
    const float floorLine = groundY - spriteH;

    if (pos.y >= floorLine) {
        pos.y    = floorLine;
        velocityY = 0.f;
        onGround  = true;
    }

    sprite->setPosition(pos);
}

// ---------------------------------------------------------------------------
// updateHUD: ajusta el ancho del fill de la barra HP según la vida actual
// ---------------------------------------------------------------------------
void PlayingState::updateHUD() {
    const int   vida      = player.getVida();
    const float ratio     = static_cast<float>(vida) / static_cast<float>(MAX_HP);
    const float fillWidth = 150.f * (ratio < 0.f ? 0.f : (ratio > 1.f ? 1.f : ratio));

    hpBarFill.setSize(sf::Vector2f(fillWidth, 15.f));
    hpText.setString("HP: " + std::to_string(vida) + "/" + std::to_string(MAX_HP));
}

// ---------------------------------------------------------------------------
// drawHUD: dibuja la barra HP y el hint de pausa
// ---------------------------------------------------------------------------
void PlayingState::drawHUD(sf::RenderWindow& window) {
    window.draw(hpBarBg);
    window.draw(hpBarFill);
    window.draw(hpText);
    window.draw(pauseHint);
}

// ---------------------------------------------------------------------------
// render: fondo → jugador → HUD → overlay de pausa
// ---------------------------------------------------------------------------
void PlayingState::render(sf::RenderWindow& window) {
    background.draw(window);

    // Dibuja el sprite del jugador si existe
    Skins* skin = player.getSkin();
    if (skin != nullptr && skin->getSprite() != nullptr) {
        window.draw(*skin->getSprite());
    }

    drawHUD(window);

    // Overlay de pausa
    if (paused) {
        // Panel semitransparente
        sf::RectangleShape overlay(sf::Vector2f(512.f, 256.f));
        overlay.setFillColor(sf::Color(0, 0, 0, 140));
        window.draw(overlay);

        sf::Text pauseText;
        pauseText.setFont(font);
        pauseText.setString("PAUSA");
        pauseText.setCharacterSize(36u);
        pauseText.setFillColor(sf::Color::White);

        sf::FloatRect tb = pauseText.getLocalBounds();
        pauseText.setOrigin(tb.left + tb.width / 2.f,
                            tb.top  + tb.height / 2.f);
        pauseText.setPosition(256.f, 128.f);
        window.draw(pauseText);
    }
}
