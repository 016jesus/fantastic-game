#include "jefe.h"
#include "protagonista.h"
#include <cmath>
#include <iostream>

// ---------------------------------------------------------------------------
// Constructor
// Inicializa Enemigos(HP_MAX, velocidad=45), guarda posición, configura el
// rectángulo de debug magenta (48x48) y las dos barras de HP del boss.
// ---------------------------------------------------------------------------
Jefe::Jefe(float startX, float startY)
    : Enemigos(HP_MAX, static_cast<int>(SPEED))
    , x(startX)
    , y(startY)
{
    // Rectángulo magenta de fallback — distinto al rojo del Skeleton.
    debugRect.setSize(sf::Vector2f(static_cast<float>(WIDTH),
                                   static_cast<float>(HEIGHT)));
    debugRect.setFillColor(sf::Color(180, 20, 180, 200));
    debugRect.setOutlineColor(sf::Color::Magenta);
    debugRect.setOutlineThickness(1.f);
    debugRect.setPosition(x, y);

    // Barra de HP: fondo (rojo oscuro) + fill (rojo brillante), 60x6 px.
    // Se dibuja 10 px por encima del boss.
    hpBarBg.setSize(sf::Vector2f(60.f, 6.f));
    hpBarBg.setFillColor(sf::Color(100, 0, 0));
    hpBarBg.setPosition(x, y - 10.f);

    hpBarFill.setSize(sf::Vector2f(60.f, 6.f));
    hpBarFill.setFillColor(sf::Color(220, 20, 20));
    hpBarFill.setPosition(x, y - 10.f);

    // El boss no tiene sprites definidos todavía; skin queda nullptr y draw()
    // usará debugRect. Se puede extender con skin = make_unique<Skins>() y
    // carga de texturas igual que en Skeleton::loadSprites().
}

// ---------------------------------------------------------------------------
// getTipo — identifica al jefe como tipo 2
// ---------------------------------------------------------------------------
int Jefe::getTipo() {
    return 2;
}

// ---------------------------------------------------------------------------
// transitionTo — cambia estado y resetea el timer de estado
// ---------------------------------------------------------------------------
void Jefe::transitionTo(EnemyState newState) {
    if (state == newState) return;
    state      = newState;
    stateTimer = 0.f;
}

// ---------------------------------------------------------------------------
// update — dispatcher principal de la máquina de estados del boss
// ---------------------------------------------------------------------------
void Jefe::update(float deltaTime, const Protagonista& player) {
    if (state == EnemyState::Dead) {
        updateDead(deltaTime);
        return;
    }

    // Decrementar cooldowns independientemente del estado activo
    if (attackCooldown > 0.f) {
        attackCooldown -= deltaTime;
        if (attackCooldown < 0.f) attackCooldown = 0.f;
    }
    if (specialAttackCooldown > 0.f) {
        specialAttackCooldown -= deltaTime;
        if (specialAttackCooldown < 0.f) specialAttackCooldown = 0.f;
    }

    switch (state) {
        case EnemyState::Idle:          updateIdle(deltaTime, player);          break;
        case EnemyState::Chase:         updateChase(deltaTime, player);         break;
        case EnemyState::Attack:        updateAttack(deltaTime, player);        break;
        case EnemyState::SpecialAttack: updateSpecialAttack(deltaTime, player); break;
        case EnemyState::Hit:           updateHit(deltaTime);                   break;
        default: break;
    }

    // Avanzar animación si hay skin
    if (skin) {
        skin->update(deltaTime);
    }

    syncSpritePosition();
    updateHealthBar();
}

// ---------------------------------------------------------------------------
// updateIdle — espera quieto; detecta al jugador en DETECTION_RANGE
// ---------------------------------------------------------------------------
void Jefe::updateIdle(float dt, const Protagonista& player) {
    (void)dt;
    if (distanceTo(player) < DETECTION_RANGE) {
        transitionTo(EnemyState::Chase);
    }
}

// ---------------------------------------------------------------------------
// updateChase — persigue al jugador.
//
// Prioridades al estar en rango:
//   1. SpecialAttack: si dist < SPECIAL_ATTACK_RANGE, specialCooldown<=0 y
//      attackCooldown > 0 (el ataque normal aún no está listo).
//   2. Attack normal: si dist < ATTACK_RANGE y attackCooldown<=0.
//   3. Volver a Idle si el jugador se alejó demasiado.
// ---------------------------------------------------------------------------
void Jefe::updateChase(float dt, const Protagonista& player) {
    const float dist = distanceTo(player);

    // ¿Se fue demasiado lejos? → Idle
    if (dist > DETECTION_RANGE * 1.3f) {
        transitionTo(EnemyState::Idle);
        return;
    }

    // Prioridad al ataque especial cuando el normal está en cooldown
    if (dist < SPECIAL_ATTACK_RANGE && specialAttackCooldown <= 0.f
        && attackCooldown > 0.f)
    {
        transitionTo(EnemyState::SpecialAttack);
        return;
    }

    // Ataque normal
    if (dist < ATTACK_RANGE && attackCooldown <= 0.f) {
        transitionTo(EnemyState::Attack);
        return;
    }

    // Moverse hacia el jugador en el eje X (enemigo terrestre)
    Position playerPos = player.getPosition();

    // Intentar usar la posición del sprite del jugador si está disponible
    Protagonista& mutablePlayer = const_cast<Protagonista&>(player);
    Skins* pSkin = mutablePlayer.getSkin();
    float targetX = playerPos.x;
    float targetY = playerPos.y;
    if (pSkin && pSkin->getSprite()) {
        sf::Vector2f spritePos = pSkin->getSprite()->getPosition();
        targetX = spritePos.x;
        targetY = spritePos.y;
    }

    float dx  = targetX - x;
    float dy  = targetY - y;
    float len = std::sqrt(dx * dx + dy * dy);

    if (len > 0.001f) {
        float nx = dx / len;
        x += nx * SPEED * dt;
        // Enemigo terrestre — solo movimiento en X
    }
}

// ---------------------------------------------------------------------------
// updateAttack — ataque normal en dos fases:
//   0.0s – 0.4s : swing (hit-frame a los 0.4s)
//   0.4s        : activa attackHit
//   0.8s        : fin del ciclo → Chase, cooldown = ATTACK_COOLDOWN
// ---------------------------------------------------------------------------
void Jefe::updateAttack(float dt, const Protagonista& player) {
    (void)player;
    stateTimer += dt;

    if (stateTimer >= 0.4f && !attackHit) {
        attackHit = true;
    }

    if (stateTimer >= 0.8f) {
        stateTimer     = 0.f;
        attackCooldown = ATTACK_COOLDOWN;
        transitionTo(EnemyState::Chase);
    }
}

// ---------------------------------------------------------------------------
// updateSpecialAttack — ataque especial de área:
//   0.0s – 0.5s : carga
//   0.5s        : activa specialHit + mensaje en stderr
//   1.2s        : fin del ciclo → Chase, cooldown = SPECIAL_COOLDOWN
// ---------------------------------------------------------------------------
void Jefe::updateSpecialAttack(float dt, const Protagonista& player) {
    (void)player;
    stateTimer += dt;

    if (stateTimer >= 0.5f && !specialHit) {
        specialHit = true;
        std::cerr << "¡ATAQUE ESPECIAL DEL JEFE!\n";
    }

    if (stateTimer >= 1.2f) {
        stateTimer            = 0.f;
        specialAttackCooldown = SPECIAL_COOLDOWN;
        transitionTo(EnemyState::Chase);
    }
}

// ---------------------------------------------------------------------------
// updateHit — stun breve de 0.4s tras recibir daño
// ---------------------------------------------------------------------------
void Jefe::updateHit(float dt) {
    stateTimer += dt;

    if (stateTimer >= 0.4f) {
        stateTimer = 0.f;
        if (getVida() > 0) {
            transitionTo(EnemyState::Chase);
        } else {
            transitionTo(EnemyState::Dead);
        }
    }
}

// ---------------------------------------------------------------------------
// updateDead — sin lógica; permanece muerto
// ---------------------------------------------------------------------------
void Jefe::updateDead(float dt) {
    (void)dt;
}

// ---------------------------------------------------------------------------
// takeDamage — reduce vida y transiciona al estado correcto
// ---------------------------------------------------------------------------
void Jefe::takeDamage(int amount) {
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
bool Jefe::isDead() const {
    return state == EnemyState::Dead;
}

// ---------------------------------------------------------------------------
// getAttackDamage
// ---------------------------------------------------------------------------
float Jefe::getAttackDamage() const {
    return ATTACK_DAMAGE;
}

// ---------------------------------------------------------------------------
// getSpecialAttackDamage
// ---------------------------------------------------------------------------
float Jefe::getSpecialAttackDamage() const {
    return SPECIAL_DAMAGE;
}

// ---------------------------------------------------------------------------
// canAttackPlayer — retorna true si el hit-frame del ataque normal está activo.
// Se limpia con resetAttackCooldown() desde PlayingState.
// ---------------------------------------------------------------------------
bool Jefe::canAttackPlayer(const Protagonista& /*player*/) const {
    return attackHit;
}

// ---------------------------------------------------------------------------
// canSpecialAttackPlayer — retorna true si el hit-frame del especial está activo.
// Se limpia con resetSpecialAttackCooldown() desde PlayingState.
// ---------------------------------------------------------------------------
bool Jefe::canSpecialAttackPlayer(const Protagonista& /*player*/) const {
    return specialHit;
}

// ---------------------------------------------------------------------------
// resetAttackCooldown — limpia el flag de ataque normal tras aplicar daño
// ---------------------------------------------------------------------------
void Jefe::resetAttackCooldown() {
    attackHit = false;
}

// ---------------------------------------------------------------------------
// resetSpecialAttackCooldown — limpia el flag del ataque especial
// ---------------------------------------------------------------------------
void Jefe::resetSpecialAttackCooldown() {
    specialHit = false;
}

// ---------------------------------------------------------------------------
// distanceTo — distancia euclidiana al sprite del jugador (o posición lógica)
// ---------------------------------------------------------------------------
float Jefe::distanceTo(const Protagonista& player) const {
    // getSkin() no es const en Entidad; cast away const — solo lectura
    Protagonista& mutablePlayer = const_cast<Protagonista&>(player);
    Skins* pSkin = mutablePlayer.getSkin();
    if (pSkin && pSkin->getSprite()) {
        sf::Vector2f spritePos = pSkin->getSprite()->getPosition();
        float dx = spritePos.x - x;
        float dy = spritePos.y - y;
        return std::sqrt(dx * dx + dy * dy);
    }

    Position p = player.getPosition();
    float dx = p.x - x;
    float dy = p.y - y;
    return std::sqrt(dx * dx + dy * dy);
}

// ---------------------------------------------------------------------------
// syncSpritePosition — aplica x,y al sprite y al debugRect
// ---------------------------------------------------------------------------
void Jefe::syncSpritePosition() {
    if (skin && skin->getSprite()) {
        skin->getSprite()->setPosition(x, y);
    }
    debugRect.setPosition(x, y);
}

// ---------------------------------------------------------------------------
// updateHealthBar — actualiza el ancho del fill de la barra HP
// ---------------------------------------------------------------------------
void Jefe::updateHealthBar() {
    const float ratio    = static_cast<float>(getVida()) / static_cast<float>(HP_MAX);
    const float clampedR = ratio < 0.f ? 0.f : (ratio > 1.f ? 1.f : ratio);
    hpBarFill.setSize(sf::Vector2f(60.f * clampedR, 6.f));

    // Reposicionar sobre el boss
    hpBarBg.setPosition(x, y - 10.f);
    hpBarFill.setPosition(x, y - 10.f);
}

// ---------------------------------------------------------------------------
// draw — dibuja sprite (o debugRect magenta) y la barra de HP siempre
// ---------------------------------------------------------------------------
void Jefe::draw(sf::RenderWindow& window) {
    if (skin && skin->getSprite()) {
        window.draw(*skin->getSprite());
    } else {
        window.draw(debugRect);
    }

    // La barra de HP se dibuja siempre encima del boss
    window.draw(hpBarBg);
    window.draw(hpBarFill);
}

// ---------------------------------------------------------------------------
// getBounds — AABB de WIDTH x HEIGHT anclado en (x, y)
// ---------------------------------------------------------------------------
sf::FloatRect Jefe::getBounds() const {
    return sf::FloatRect(x, y, static_cast<float>(WIDTH), static_cast<float>(HEIGHT));
}

float Jefe::getX() const { return x; }
float Jefe::getY() const { return y; }

// ---------------------------------------------------------------------------
// Combate interface — el ataque del boss se maneja con attackHit/specialHit;
// este método satisface el virtual puro heredado.
// ---------------------------------------------------------------------------
int Jefe::ataque(Armas* /*arma*/) {
    return static_cast<int>(ATTACK_DAMAGE);
}

// ---------------------------------------------------------------------------
// barraDeVida — stub para satisfacer la interfaz; la barra real es hpBarBg/Fill.
// ---------------------------------------------------------------------------
Sprite* Jefe::barraDeVida() {
    return &debugRect;  // cumple la firma; la barra visual se dibuja en draw()
}
