#include "PlayingState.h"
#include "GameStateManager.h"
#include <algorithm>
#include <cmath>

// ---------------------------------------------------------------------------
// Constructor: inicializa jugador, fondo, HUD y overlays de UI
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
    hpBarBg.setSize(sf::Vector2f(150.f, 15.f));
    hpBarBg.setFillColor(sf::Color(139, 0, 0));
    hpBarBg.setPosition(10.f, 10.f);

    hpBarFill.setSize(sf::Vector2f(150.f, 15.f));
    hpBarFill.setFillColor(sf::Color(220, 20, 20));
    hpBarFill.setPosition(10.f, 10.f);

    // Carga de fuente
    bool fontLoaded = font.loadFromFile("minecraft.otf");
    if (!fontLoaded) {
        font.loadFromFile("Minecraft.ttf");
    }

    hpText.setFont(font);
    hpText.setCharacterSize(10u);
    hpText.setFillColor(sf::Color::White);
    hpText.setPosition(12.f, 11.f);

    // Hint de controles actualizado con todas las acciones disponibles
    pauseHint.setFont(font);
    pauseHint.setCharacterSize(10u);
    pauseHint.setFillColor(sf::Color(200, 200, 200));
    pauseHint.setString("ESC:pausa I:inv J:atacar E:interactuar");
    pauseHint.setPosition(10.f, 30.f);

    // Conectar la fuente a los overlays de UI
    dialogBox.loadFont(font);
    inventoryOverlay.loadFont(font);
    shopOverlay.loadFont(font);
}

// ---------------------------------------------------------------------------
// onEnter: posiciona al jugador, genera enemigos e inicializa NPCs y cofres
// ---------------------------------------------------------------------------
void PlayingState::onEnter() {
    paused    = false;
    velocityY = 0.f;
    onGround  = true;

    // Posición inicial: izquierda del mapa, sobre el suelo
    if (player.getSkin() != nullptr) {
        player.getSkin()->getSprite()->setPosition(64.f, groundY - 64.f);
    }

    spawnEnemies();
    setupNpcsAndChests();
    updateHUD();
}

// ---------------------------------------------------------------------------
// setupNpcsAndChests: define las posiciones de NPCs, vendedor y cofres
// ---------------------------------------------------------------------------
void PlayingState::setupNpcsAndChests() {
    // NPC Aldeano en la zona izquierda del mapa
    npcSpots = {
        {100.f, groundY - 24.f, "Aldeano",
         {"Bienvenido a estas tierras,", "viajero. Ten cuidado.", "Los no-muertos acechan."}}
    };

    // Vendedor en la zona derecha
    vendorSpot = {380.f, groundY - 24.f};

    // Cofre en el centro del mapa con 30 monedas
    chestSpots = {
        {250.f, groundY - 24.f, false, 30}
    };
}

// ---------------------------------------------------------------------------
// handleEvent: procesa eventos de teclado enrutando primero a los overlays
// activos antes de procesar acciones globales (pausa, salto).
// ---------------------------------------------------------------------------
void PlayingState::handleEvent(const sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    // --- Prioridad 1: diálogo activo — ENTER o E avanza la línea ---
    if (dialogBox.isOpen()) {
        if (event.key.code == sf::Keyboard::Return ||
            event.key.code == sf::Keyboard::E)
        {
            dialogBox.advance();
        }
        return;
    }

    // --- Prioridad 2: overlay de inventario activo ---
    if (inventoryOverlay.isOpen()) {
        if (inventoryOverlay.handleEvent(event, player)) return;
    }

    // --- Prioridad 3: overlay de tienda activo ---
    if (shopOverlay.isOpen()) {
        if (shopOverlay.handleEvent(event, player)) return;
    }

    // --- Acciones globales ---
    switch (event.key.code) {
        case sf::Keyboard::Escape:
            paused = !paused;
            break;

        case sf::Keyboard::Space:
            // Saltar sólo si está en el suelo y no hay overlay abierto
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
// checkInteractions: detecta si el jugador está cerca de NPCs, vendedor o
// cofres y activa las interacciones con la tecla E o I.
// Se llama al inicio de update() antes de cualquier verificación de pausa.
// ---------------------------------------------------------------------------
void PlayingState::checkInteractions() {
    // No procesar interacciones si algún overlay ya está abierto
    if (dialogBox.isOpen() || inventoryOverlay.isOpen() || shopOverlay.isOpen()) return;

    // Tecla I: toggle del inventario (no requiere proximidad)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
        // Usamos un flag para evitar activar múltiples veces por frame
        // La detección real de "pulsación única" se hace en handleEvent;
        // este bloque es un complemento para la tecla I cuando no hay overlay.
        // Nota: la tecla I se maneja también en handleEvent → InventoryOverlay.
    }

    // Tecla E: interacción con objetos del mundo
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::E)) return;

    // --- NPC ---
    for (const auto& npc : npcSpots) {
        if (distanceToPlayer(npc.x, npc.y) < INTERACT_RADIUS) {
            dialogBox.open(npc.name, npc.dialog);
            return;
        }
    }

    // --- Vendedor ---
    if (distanceToPlayer(vendorSpot.x, vendorSpot.y) < INTERACT_RADIUS) {
        shopOverlay.open();
        return;
    }

    // --- Cofres ---
    for (auto& chest : chestSpots) {
        if (!chest.opened && distanceToPlayer(chest.x, chest.y) < INTERACT_RADIUS) {
            chest.opened = true;
            player.setDinero(player.getDinero() + chest.money);
            dialogBox.open("Cofre",
                {"Cofre abierto: +" + std::to_string(chest.money) + " monedas"});
            return;
        }
    }
}

// ---------------------------------------------------------------------------
// distanceToPlayer: calcula la distancia euclidiana desde (ox, oy) hasta el
// centro del sprite del jugador.
// ---------------------------------------------------------------------------
float PlayingState::distanceToPlayer(float ox, float oy) {
    Skins* skin = player.getSkin();
    if (!skin || !skin->getSprite()) return 9999.f;

    sf::Vector2f pos = skin->getSprite()->getPosition();
    sf::FloatRect bounds = skin->getSprite()->getGlobalBounds();
    float px = pos.x + bounds.width  * 0.5f;
    float py = pos.y + bounds.height * 0.5f;

    float dx = ox - px;
    float dy = oy - py;
    return std::sqrt(dx * dx + dy * dy);
}

// ---------------------------------------------------------------------------
// update: lógica de juego principal.
// checkInteractions se llama siempre (incluso en pausa es ignorado).
// Si hay overlay abierto, no se ejecuta movimiento ni combate.
// ---------------------------------------------------------------------------
void PlayingState::update(float deltaTime) {
    // Las interacciones (E/I) se verifican antes del bloqueo por pausa
    // para que la detección ocurra; checkInteractions ignora overlays abiertos.
    checkInteractions();

    if (paused) return;

    // Si algún overlay de UI está activo, el jugador no puede moverse ni atacar
    bool anyOverlayOpen = dialogBox.isOpen() ||
                          inventoryOverlay.isOpen() ||
                          shopOverlay.isOpen();

    if (!anyOverlayOpen) {
        handleMovement(deltaTime);
        updateCombat(deltaTime);
    }

    applyGravity(deltaTime);
    updateHUD();
}

// ---------------------------------------------------------------------------
// handleMovement: mueve al jugador con A/D o flechas izquierda/derecha
// La velocidad del protagonista (getVelocidad()) se usa como píxeles/segundo.
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
        sprite->setOrigin(0.f, 0.f);
        sprite->setScale(1.f, 1.f);
        moved = true;
    }

    (void)moved;

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

    const float spriteH   = sprite->getGlobalBounds().height;
    const float floorLine = groundY - spriteH;

    if (pos.y >= floorLine) {
        pos.y     = floorLine;
        velocityY = 0.f;
        onGround  = true;
    }

    sprite->setPosition(pos);
}

// ---------------------------------------------------------------------------
// spawnEnemies: crea Skeletons en posiciones fijas sobre el suelo
// ---------------------------------------------------------------------------
void PlayingState::spawnEnemies() {
    enemies.clear();

    const float ey = groundY - 32.f;
    enemies.emplace_back(200.f, ey);
    enemies.emplace_back(320.f, ey);
    enemies.emplace_back(400.f, ey);
}

// ---------------------------------------------------------------------------
// updateCombat: actualiza IA de enemigos, resuelve daño bidireccional y
// elimina los muertos del vector.
// ---------------------------------------------------------------------------
void PlayingState::updateCombat(float deltaTime) {
    sf::Vector2f playerSpritePos(0.f, 0.f);
    float playerHalfW = 0.f;
    float playerHalfH = 0.f;
    Skins* playerSkin = player.getSkin();
    if (playerSkin && playerSkin->getSprite()) {
        playerSpritePos = playerSkin->getSprite()->getPosition();
        sf::FloatRect pb = playerSkin->getSprite()->getGlobalBounds();
        playerHalfW = pb.width  * 0.5f;
        playerHalfH = pb.height * 0.5f;
    }

    for (auto& enemy : enemies) {
        enemy.update(deltaTime, player);

        if (enemy.canAttackPlayer(player)) {
            int newVida = player.getVida() - static_cast<int>(enemy.getAttackDamage());
            if (newVida < 0) newVida = 0;
            player.setVida(newVida);
            enemy.resetAttackCooldown();
        }
    }

    // Jugador ataca (tecla J) — detección de borde de pulsación
    bool attackKeyDown = sf::Keyboard::isKeyPressed(sf::Keyboard::J);

    if (attackKeyDown && !attackPressed) {
        attackPressed = true;

        float px = playerSpritePos.x + playerHalfW;
        float py = playerSpritePos.y + playerHalfH;

        for (auto& enemy : enemies) {
            if (enemy.isDead()) continue;

            sf::FloatRect eb = enemy.getBounds();
            float ex = eb.left + eb.width  * 0.5f;
            float ey = eb.top  + eb.height * 0.5f;

            float dx = ex - px;
            float dy = ey - py;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist <= PLAYER_ATTACK_RANGE) {
                enemy.takeDamage(PLAYER_ATTACK_DAMAGE);
            }
        }
    }

    if (!attackKeyDown) {
        attackPressed = false;
    }

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
                       [](const Skeleton& e) { return e.isDead(); }),
        enemies.end()
    );
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
// drawHUD: dibuja la barra HP y el hint de controles
// ---------------------------------------------------------------------------
void PlayingState::drawHUD(sf::RenderWindow& window) {
    window.draw(hpBarBg);
    window.draw(hpBarFill);
    window.draw(hpText);
    window.draw(pauseHint);
}

// ---------------------------------------------------------------------------
// drawEnemies: itera el vector de Skeletons y dibuja cada uno
// ---------------------------------------------------------------------------
void PlayingState::drawEnemies(sf::RenderWindow& window) {
    for (auto& enemy : enemies) {
        enemy.draw(window);
    }
}

// ---------------------------------------------------------------------------
// drawNpcsAndChests: dibuja representaciones visuales simples de los objetos
// del mundo (NPCs, vendedor, cofres) con etiquetas y hint de interacción [E].
// ---------------------------------------------------------------------------
void PlayingState::drawNpcsAndChests(sf::RenderWindow& window) {
    // Fuente para etiquetas (usamos la misma del HUD)
    sf::Text label;
    label.setFont(font);
    label.setCharacterSize(7u);
    label.setFillColor(sf::Color::White);

    sf::Text hintE;
    hintE.setFont(font);
    hintE.setCharacterSize(7u);
    hintE.setFillColor(sf::Color::Yellow);
    hintE.setString("[E]");

    sf::RectangleShape shape;

    // --- NPCs (rectángulo azul 16x24) ---
    for (const auto& npc : npcSpots) {
        shape.setSize(sf::Vector2f(16.f, 24.f));
        shape.setFillColor(sf::Color(60, 60, 200));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1.f);
        // Centrado en (npc.x, npc.y)
        shape.setPosition(npc.x - 8.f, npc.y - 24.f);
        window.draw(shape);

        label.setString("NPC");
        label.setPosition(npc.x - 8.f, npc.y - 32.f);
        window.draw(label);

        if (distanceToPlayer(npc.x, npc.y) < INTERACT_RADIUS) {
            hintE.setPosition(npc.x - 6.f, npc.y - 42.f);
            window.draw(hintE);
        }
    }

    // --- Vendedor (rectángulo dorado 16x24) ---
    {
        shape.setSize(sf::Vector2f(16.f, 24.f));
        shape.setFillColor(sf::Color(218, 165, 32));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1.f);
        shape.setPosition(vendorSpot.x - 8.f, vendorSpot.y - 24.f);
        window.draw(shape);

        label.setString("Vendedor");
        label.setPosition(vendorSpot.x - 16.f, vendorSpot.y - 32.f);
        window.draw(label);

        if (distanceToPlayer(vendorSpot.x, vendorSpot.y) < INTERACT_RADIUS) {
            hintE.setPosition(vendorSpot.x - 6.f, vendorSpot.y - 42.f);
            window.draw(hintE);
        }
    }

    // --- Cofres (cuadrado 20x20, marrón si cerrado, gris si abierto) ---
    for (const auto& chest : chestSpots) {
        shape.setSize(sf::Vector2f(20.f, 20.f));
        if (chest.opened) {
            shape.setFillColor(sf::Color(120, 120, 120));
        } else {
            shape.setFillColor(sf::Color(139, 90, 43));
        }
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1.f);
        shape.setPosition(chest.x - 10.f, chest.y - 20.f);
        window.draw(shape);

        label.setString(chest.opened ? "(abierto)" : "Cofre");
        label.setPosition(chest.x - 12.f, chest.y - 30.f);
        window.draw(label);

        if (!chest.opened && distanceToPlayer(chest.x, chest.y) < INTERACT_RADIUS) {
            hintE.setPosition(chest.x - 6.f, chest.y - 40.f);
            window.draw(hintE);
        }
    }
}

// ---------------------------------------------------------------------------
// render: fondo → enemigos → NPCs/cofres → jugador → HUD → overlays de UI
// ---------------------------------------------------------------------------
void PlayingState::render(sf::RenderWindow& window) {
    background.draw(window);

    // Dibuja enemigos
    drawEnemies(window);

    // Dibuja NPCs, vendedor y cofres
    drawNpcsAndChests(window);

    // Dibuja el sprite del jugador si existe
    Skins* skin = player.getSkin();
    if (skin != nullptr && skin->getSprite() != nullptr) {
        window.draw(*skin->getSprite());
    }

    drawHUD(window);

    // --- Overlays de UI (por encima del HUD) ---
    if (dialogBox.isOpen()) {
        dialogBox.draw(window);
    }
    if (inventoryOverlay.isOpen()) {
        inventoryOverlay.draw(window, player);
    }
    if (shopOverlay.isOpen()) {
        shopOverlay.draw(window, player);
    }

    // Overlay de pausa (encima de todo)
    if (paused) {
        sf::RectangleShape overlay(sf::Vector2f(512.f, 256.f));
        overlay.setFillColor(sf::Color(0, 0, 0, 140));
        window.draw(overlay);

        sf::Text pauseText;
        pauseText.setFont(font);
        pauseText.setString("PAUSA");
        pauseText.setCharacterSize(36u);
        pauseText.setFillColor(sf::Color::White);

        sf::FloatRect tb = pauseText.getLocalBounds();
        pauseText.setOrigin(tb.left + tb.width  / 2.f,
                            tb.top  + tb.height / 2.f);
        pauseText.setPosition(256.f, 128.f);
        window.draw(pauseText);
    }
}
