#include "skeleton.h"
#include "protagonista.h"
#include <cmath>
#include <stdexcept>

// ---------------------------------------------------------------------------
// Constructor
// Inicializa la herencia con vida=40 y velocidad=60, guarda la posición
// inicial, configura el rectángulo de depuración y carga los sprites.
// ---------------------------------------------------------------------------
Skeleton::Skeleton(float startX, float startY)
    : Normales(40, 60)
    , x(startX)
    , y(startY)
    , patrolOriginX(startX)
{
    // Rectángulo rojo de fallback — se usa si los sprites no cargan.
    debugRect.setSize(sf::Vector2f(static_cast<float>(WIDTH),
                                   static_cast<float>(HEIGHT)));
    debugRect.setFillColor(sf::Color(180, 20, 20, 200));
    debugRect.setOutlineColor(sf::Color::Red);
    debugRect.setOutlineThickness(1.f);
    debugRect.setPosition(x, y);

    loadSprites();
}

// ---------------------------------------------------------------------------
// getTipo — identifica a este enemigo como tipo 1 (Skeleton)
// ---------------------------------------------------------------------------
int Skeleton::getTipo() {
    return 1;
}

// ---------------------------------------------------------------------------
// loadSprites
// Carga cada animación desde su propio sprite sheet.
// Rutas tomadas de rutas_skeleton.txt:
//   Sprites/Skeleton/Sprite_Sheets/_Idle.png    (264x32  — 8 frames de 33x32)
//   Sprites/Skeleton/Sprite_Sheets/_Walk.png    (286x33  — 11 frames de 26x33)
//   Sprites/Skeleton/Sprite_Sheets/_Attack.png  (774x37  — 9 frames de 86x37)
//   Sprites/Skeleton/Sprite_Sheets/_Hit.png     (240x32  — 10 frames de 24x32)
//   Sprites/Skeleton/Sprite_Sheets/_Dead.png    (495x32  — 9 frames de 55x32)
// Si cualquier textura falla, se deja skin=nullptr y el draw() usa debugRect.
// ---------------------------------------------------------------------------
void Skeleton::loadSprites() {
    static const char* BASE = "Sprites/Skeleton/Sprite_Sheets/";

    struct SheetDef {
        const char* name;
        const char* file;
        int fw, fh, frames;
        float duration;
        bool loop;
    };

    static const SheetDef defs[] = {
        { "idle",   "_Idle.png",   SK_FRAME_W_IDLE,   SK_FRAME_H_IDLE,   SK_FRAMES_IDLE,   0.12f, true  },
        { "walk",   "_Walk.png",   SK_FRAME_W_WALK,   SK_FRAME_H_WALK,   SK_FRAMES_WALK,   0.08f, true  },
        { "attack", "_Attack.png", SK_FRAME_W_ATTACK, SK_FRAME_H_ATTACK, SK_FRAMES_ATTACK, 0.07f, false },
        { "hit",    "_Hit.png",    SK_FRAME_W_HIT,    SK_FRAME_H_HIT,    SK_FRAMES_HIT,    0.05f, false },
        { "dead",   "_Dead.png",   SK_FRAME_W_DEAD,   SK_FRAME_H_DEAD,   SK_FRAMES_DEAD,   0.12f, false },
    };

    // Usamos una sola instancia de Skins con la animación de idle como base.
    // Cargamos todas las animaciones en el mismo objeto Skins si la primera
    // textura (idle) es asequible; si no, dejamos skin = nullptr.
    // Nota: Skins/AnimationSystem acepta una textura activa y múltiples
    // definiciones de animación. Dado que las animaciones son sprite sheets
    // separados, cada una necesita su propia textura — pero Skins sólo
    // mantiene UN sprite (y por tanto una textura). Para poder conmutar entre
    // animaciones que viven en archivos distintos usamos un mapa análogo al
    // de Protagonista: un Skins por animación, guardados en skinMap.

    struct SkinEntry {
        std::string name;
        std::unique_ptr<Skins> sk;
    };

    // Mapa local (skinMap como miembro causaría incompatibilidades con
    // Skeleton almacenado en std::vector — por eso usamos un único Skins
    // activo y reemplazamos la textura al transicionar).
    // Solución pragmática: creamos el objeto Skins con la animación idle
    // y simplemente recargamos la textura + animación al cambiar de estado.
    // Esto es sencillo porque Skins::loadTexture() rebindea el sprite.

    try {
        skin = std::make_unique<Skins>();

        // Cargar idle como animación inicial
        std::string idlePath = std::string(BASE) + defs[0].file;
        skin->loadTexture(idlePath);
        skin->addAnimation(defs[0].name,
                           defs[0].fw, defs[0].fh,
                           /*row=*/0, defs[0].frames,
                           defs[0].duration, defs[0].loop);

        // Registrar las demás animaciones en el mismo objeto.
        // Como cada una tiene su propio archivo de textura sólo podemos
        // tener UNA textura activa en el sprite a la vez. Por ello, cuando
        // transicionTo() cambia de estado, recargará la textura
        // correspondiente usando loadAnimation().
        // Por ahora guardamos sólo los metadatos en el AnimationSystem
        // (sin textura para los otros) — addAnimation sólo registra el
        // IntRect mapping, no necesita la textura en ese momento.
        for (int i = 1; i < 5; ++i) {
            skin->addAnimation(defs[i].name,
                               defs[i].fw, defs[i].fh,
                               0, defs[i].frames,
                               defs[i].duration, defs[i].loop);
        }

        skin->playAnimation("idle");
        syncSpritePosition();

    } catch (const std::runtime_error&) {
        // Sprites no encontrados — el draw() usará debugRect rojo.
        skin.reset();
    }
}

// ---------------------------------------------------------------------------
// syncSpritePosition — aplica x,y al sprite si existe.
// ---------------------------------------------------------------------------
void Skeleton::syncSpritePosition() {
    if (skin && skin->getSprite()) {
        skin->getSprite()->setPosition(x, y);
    }
    debugRect.setPosition(x, y);
}

// ---------------------------------------------------------------------------
// transitionTo — cambia de estado y arranca la animación correspondiente.
// También recarga la textura del sprite sheet correcto porque cada animación
// vive en un archivo PNG separado.
// ---------------------------------------------------------------------------
void Skeleton::transitionTo(EnemyState newState) {
    if (state == newState) return;
    state      = newState;
    stateTimer = 0.f;

    if (!skin) return;

    static const char* BASE = "Sprites/Skeleton/Sprite_Sheets/";

    struct AnimInfo {
        EnemyState  state;
        const char* file;
        const char* name;
        int fw, fh, frames;
        float duration;
        bool loop;
    };

    static const AnimInfo table[] = {
        { EnemyState::Idle,   "_Idle.png",   "idle",   Skeleton::SK_FRAME_W_IDLE,   Skeleton::SK_FRAME_H_IDLE,   Skeleton::SK_FRAMES_IDLE,   0.12f, true  },
        { EnemyState::Patrol, "_Walk.png",   "walk",   Skeleton::SK_FRAME_W_WALK,   Skeleton::SK_FRAME_H_WALK,   Skeleton::SK_FRAMES_WALK,   0.08f, true  },
        { EnemyState::Chase,  "_Walk.png",   "walk",   Skeleton::SK_FRAME_W_WALK,   Skeleton::SK_FRAME_H_WALK,   Skeleton::SK_FRAMES_WALK,   0.08f, true  },
        { EnemyState::Attack, "_Attack.png", "attack", Skeleton::SK_FRAME_W_ATTACK, Skeleton::SK_FRAME_H_ATTACK, Skeleton::SK_FRAMES_ATTACK, 0.07f, false },
        { EnemyState::Hit,    "_Hit.png",    "hit",    Skeleton::SK_FRAME_W_HIT,    Skeleton::SK_FRAME_H_HIT,    Skeleton::SK_FRAMES_HIT,    0.05f, false },
        { EnemyState::Dead,   "_Dead.png",   "dead",   Skeleton::SK_FRAME_W_DEAD,   Skeleton::SK_FRAME_H_DEAD,   Skeleton::SK_FRAMES_DEAD,   0.12f, false },
    };

    for (const auto& info : table) {
        if (info.state == newState) {
            try {
                skin->loadTexture(std::string(BASE) + info.file);
                // Re-register the animation with the new texture dimensions.
                skin->addAnimation(info.name, info.fw, info.fh,
                                   0, info.frames, info.duration, info.loop);
                skin->playAnimation(info.name);
            } catch (const std::runtime_error&) {
                // Textura no disponible — continúa con debugRect.
            }
            break;
        }
    }
}

// ---------------------------------------------------------------------------
// update — dispatcher principal de la máquina de estados
// ---------------------------------------------------------------------------
void Skeleton::update(float deltaTime, const Protagonista& player) {
    if (state == EnemyState::Dead) {
        updateDead(deltaTime);
        return;
    }

    // Decrementar cooldown de ataque independientemente del estado
    if (attackCooldown > 0.f) {
        attackCooldown -= deltaTime;
        if (attackCooldown < 0.f) attackCooldown = 0.f;
    }

    switch (state) {
        case EnemyState::Idle:   updateIdle(deltaTime, player);   break;
        case EnemyState::Patrol: updatePatrol(deltaTime, player); break;
        case EnemyState::Chase:  updateChase(deltaTime, player);  break;
        case EnemyState::Attack: updateAttack(deltaTime, player); break;
        case EnemyState::Hit:    updateHit(deltaTime);            break;
        default: break;
    }

    // Avanzar animación
    if (skin) {
        skin->update(deltaTime);
    }

    syncSpritePosition();
}

// ---------------------------------------------------------------------------
// updateIdle — espera quieto; transiciona a Patrol tras 1.5s o si detecta
//              al jugador en DETECTION_RANGE.
// ---------------------------------------------------------------------------
void Skeleton::updateIdle(float deltaTime, const Protagonista& player) {
    idleTimer += deltaTime;

    if (distanceTo(player) < DETECTION_RANGE) {
        idleTimer = 0.f;
        transitionTo(EnemyState::Chase);
        return;
    }

    if (idleTimer >= 1.5f) {
        idleTimer = 0.f;
        transitionTo(EnemyState::Patrol);
    }
}

// ---------------------------------------------------------------------------
// updatePatrol — patrulla de lado a lado dentro de patrolRange desde el
//               origen. Si el jugador entra en rango, cambia a Chase.
// ---------------------------------------------------------------------------
void Skeleton::updatePatrol(float deltaTime, const Protagonista& player) {
    if (distanceTo(player) < DETECTION_RANGE) {
        transitionTo(EnemyState::Chase);
        return;
    }

    x += patrolDir * SPEED * deltaTime;

    // Invertir dirección al salir del rango de patrulla
    const float patrolRange = 80.f;
    if (x > patrolOriginX + patrolRange) {
        x = patrolOriginX + patrolRange;
        patrolDir = -1.f;
    } else if (x < patrolOriginX - patrolRange) {
        x = patrolOriginX - patrolRange;
        patrolDir = 1.f;
    }

    // Voltear sprite según dirección
    if (skin && skin->getSprite()) {
        if (patrolDir < 0.f) {
            sf::FloatRect lb = skin->getSprite()->getLocalBounds();
            skin->getSprite()->setOrigin(lb.width, 0.f);
            skin->getSprite()->setScale(-1.f, 1.f);
        } else {
            skin->getSprite()->setOrigin(0.f, 0.f);
            skin->getSprite()->setScale(1.f, 1.f);
        }
    }
}

// ---------------------------------------------------------------------------
// updateChase — persigue al jugador. Ataca si está en rango y el cooldown
//               expiró. Vuelve a Idle si el jugador se aleja demasiado.
// ---------------------------------------------------------------------------
void Skeleton::updateChase(float deltaTime, const Protagonista& player) {
    const float dist = distanceTo(player);

    // ¿Se fue demasiado lejos?
    if (dist > DETECTION_RANGE * 1.5f) {
        transitionTo(EnemyState::Idle);
        return;
    }

    // ¿Puede atacar?
    if (dist < ATTACK_RANGE && attackCooldown <= 0.f) {
        transitionTo(EnemyState::Attack);
        return;
    }

    // Mover hacia el jugador
    Position playerPos = player.getPosition();
    float dx = playerPos.x - x;
    float dy = playerPos.y - y;
    float len = std::sqrt(dx * dx + dy * dy);

    if (len > 0.001f) {
        float nx = dx / len;
        x += nx * SPEED * deltaTime;
        // Solo movimiento en X para un enemigo terrestre simple
    }

    // Voltear sprite en la dirección de movimiento
    if (skin && skin->getSprite()) {
        if (dx < 0.f) {
            sf::FloatRect lb = skin->getSprite()->getLocalBounds();
            skin->getSprite()->setOrigin(lb.width, 0.f);
            skin->getSprite()->setScale(-1.f, 1.f);
        } else {
            skin->getSprite()->setOrigin(0.f, 0.f);
            skin->getSprite()->setScale(1.f, 1.f);
        }
    }
}

// ---------------------------------------------------------------------------
// updateAttack — ejecuta la secuencia de ataque en dos fases:
//   0.0s – 0.3s : animación de swing
//   0.3s        : activa attackHit (el daño se aplica en PlayingState)
//   0.3s – 0.6s : recuperación
//   0.6s        : resetea y vuelve a Chase
// ---------------------------------------------------------------------------
void Skeleton::updateAttack(float deltaTime, const Protagonista& player) {
    (void)player; // la posición ya fue usada para transicionar a este estado

    stateTimer += deltaTime;

    // Activar el flag de golpe a los 0.3s (hit-frame de la animación)
    if (stateTimer >= 0.3f && !attackHit) {
        attackHit = true;
    }

    // Fin del ciclo de ataque a los 0.6s
    if (stateTimer >= 0.6f) {
        stateTimer     = 0.f;
        attackCooldown = ATTACK_COOLDOWN;
        transitionTo(EnemyState::Chase);
    }
}

// ---------------------------------------------------------------------------
// updateHit — stun breve de 0.3s tras recibir daño.
// ---------------------------------------------------------------------------
void Skeleton::updateHit(float deltaTime) {
    stateTimer += deltaTime;

    if (stateTimer >= 0.3f) {
        stateTimer = 0.f;
        if (getVida() > 0) {
            transitionTo(EnemyState::Chase);
        } else {
            transitionTo(EnemyState::Dead);
        }
    }
}

// ---------------------------------------------------------------------------
// updateDead — sin lógica; la animación de muerte se reproduce y el vector
//              de PlayingState retirará este enemigo con isDead().
// ---------------------------------------------------------------------------
void Skeleton::updateDead(float deltaTime) {
    stateTimer += deltaTime;
    // No hay transición de salida — permanece Dead.
    (void)deltaTime;
}

// ---------------------------------------------------------------------------
// takeDamage — reduce vida y transiciona al estado correcto.
// ---------------------------------------------------------------------------
void Skeleton::takeDamage(int amount) {
    int newVida = getVida() - amount;
    if (newVida < 0) newVida = 0;
    setVida(newVida);

    if (newVida <= 0) {
        transitionTo(EnemyState::Dead);
    } else {
        transitionTo(EnemyState::Hit);
    }
}

// ---------------------------------------------------------------------------
// isDead
// ---------------------------------------------------------------------------
bool Skeleton::isDead() const {
    return state == EnemyState::Dead;
}

// ---------------------------------------------------------------------------
// getAttackDamage
// ---------------------------------------------------------------------------
float Skeleton::getAttackDamage() const {
    return ATTACK_DAMAGE;
}

// ---------------------------------------------------------------------------
// canAttackPlayer — retorna true si el hit-frame de ataque está activo.
// El flag se limpia al llamar resetAttackCooldown() desde PlayingState.
// ---------------------------------------------------------------------------
bool Skeleton::canAttackPlayer(const Protagonista& /*player*/) const {
    return attackHit;
}

// ---------------------------------------------------------------------------
// resetAttackCooldown — PlayingState lo llama tras aplicar el daño.
// ---------------------------------------------------------------------------
void Skeleton::resetAttackCooldown() {
    attackHit = false;
}

// ---------------------------------------------------------------------------
// draw — dibuja sprite o rectángulo de debug según disponibilidad.
// ---------------------------------------------------------------------------
void Skeleton::draw(sf::RenderWindow& window) {
    if (skin && skin->getSprite()) {
        window.draw(*skin->getSprite());
    } else {
        window.draw(debugRect);
    }
}

// ---------------------------------------------------------------------------
// getBounds — AABB de WIDTH x HEIGHT anclado en (x, y).
// ---------------------------------------------------------------------------
sf::FloatRect Skeleton::getBounds() const {
    return sf::FloatRect(x, y, static_cast<float>(WIDTH), static_cast<float>(HEIGHT));
}

// ---------------------------------------------------------------------------
// getX / getY
// ---------------------------------------------------------------------------
float Skeleton::getX() const { return x; }
float Skeleton::getY() const { return y; }

// ---------------------------------------------------------------------------
// distanceTo — distancia euclidiana al sprite/posición del jugador.
// Usa player.getPosition() que retorna el Position de Entidad.
// ---------------------------------------------------------------------------
float Skeleton::distanceTo(const Protagonista& player) const {
    Position p = player.getPosition();

    // getSkin() is not const in Entidad; cast away const safely — read-only use.
    Protagonista& mutablePlayer = const_cast<Protagonista&>(player);
    Skins* pSkin = mutablePlayer.getSkin();
    if (pSkin && pSkin->getSprite()) {
        sf::Vector2f spritePos = pSkin->getSprite()->getPosition();
        float dx = spritePos.x - x;
        float dy = spritePos.y - y;
        return std::sqrt(dx * dx + dy * dy);
    }

    float dx = p.x - x;
    float dy = p.y - y;
    return std::sqrt(dx * dx + dy * dy);
}

// ---------------------------------------------------------------------------
// Combate interface (heredada de Enemigos -> Combate)
// Skeleton no tiene barra de vida visual propia (la hereda del HUD del player)
// pero debe implementar los virtuales puros.
// ---------------------------------------------------------------------------
// barraDeVida() ya está implementado en Enemigos::barraDeVida() -> nullptr.
// ataque() no es invocado directamente desde AI (se maneja con attackHit),
// pero se necesita para satisfacer la interfaz Combate.
int Skeleton::ataque(Armas* /*arma*/) {
    return static_cast<int>(ATTACK_DAMAGE);
}
