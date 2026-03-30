#include "protagonista.h"
#include <cmath>
#include <fstream>

// Animation frame counts measured from the actual sprite sheets under
// Sprites/PROTAGREY/Outline/120x80_PNGSheets/.
// All sheets use 120x80 px frames on a single row.
static constexpr int   FRAME_W = 120;
static constexpr int   FRAME_H = 80;
static const char* SPRITE_BASE = "Sprites/PROTAGREY/Outline/120x80_PNGSheets/";

Protagonista::Protagonista(int vida, int velocidad, string nombre, int dinero):
    Dociles(vida, velocidad, nombre), dinero(dinero){}

int Protagonista::getDinero()
{
    return this->dinero;
}

void Protagonista::setDinero(int dinero)
{
    this->dinero = dinero;
}

// ---------------------------------------------------------------------------
// loadSprites
// Each animation has its own sprite sheet file, so we create one Skins
// instance per animation and store them in skinMap.
// The "idle" skin is also installed as the Entidad base skin so that
// getSkin() always returns a valid pointer for collision / position queries.
// ---------------------------------------------------------------------------
void Protagonista::loadSprites()
{
    struct AnimDef {
        const char* name;
        const char* file;
        int frameCount;
        float frameDuration;
        bool loop;
    };

    static const AnimDef defs[] = {
        { "idle",    "_Idle.png",   10, 0.10f, true  },
        { "run",     "_Run.png",     8, 0.08f, true  },
        { "jump",    "_Jump.png",    3, 0.12f, false },
        { "attack",  "_Attack.png",  4, 0.10f, false },
        { "hit",     "_Hit.png",     2, 0.15f, false },
        { "death",   "_Death.png",   6, 0.15f, false },
    };

    for (const auto& def : defs) {
        auto sk = std::make_unique<Skins>();
        std::string path = std::string(SPRITE_BASE) + def.file;
        try {
            sk->loadTexture(path);
        } catch (const std::runtime_error&) {
            // Texture file not found; skip this animation gracefully.
            continue;
        }
        // Single-row sheets: row index is always 0.
        sk->addAnimation(def.name, FRAME_W, FRAME_H,
                         /*row=*/0, def.frameCount,
                         def.frameDuration, def.loop);
        sk->playAnimation(def.name);
        skinMap[def.name] = std::move(sk);
    }

    // Set the idle skin as the active entity skin (used by Entidad helpers).
    activeAnim = "idle";
    if (skinMap.count("idle")) {
        // Transfer a fresh idle skin reference to the Entidad base.
        // We keep the owning copy in skinMap; setSkin gets a non-owning view
        // wrapped in a unique_ptr that we DON'T want to own — but setSkin
        // takes ownership. Instead we seed Entidad::skin once with a separate
        // skin built from the same texture.
        auto baseSkin = std::make_unique<Skins>();
        baseSkin->loadTexture(std::string(SPRITE_BASE) + "_Idle.png");
        baseSkin->addAnimation("idle", FRAME_W, FRAME_H, 0, 10, 0.10f, true);
        baseSkin->playAnimation("idle");
        setSkin(std::move(baseSkin));
    }
}

// ---------------------------------------------------------------------------
// updateAnimation
// Advances the currently active skin's animation.
// ---------------------------------------------------------------------------
void Protagonista::updateAnimation(float deltaTime)
{
    auto it = skinMap.find(activeAnim);
    if (it != skinMap.end()) {
        it->second->update(deltaTime);
    }
    // Keep the Entidad base skin in sync as well.
    if (getSkin()) {
        getSkin()->update(deltaTime);
    }
}

// ---------------------------------------------------------------------------

string Protagonista::actualizarInventario(bool validez, Objetos* objeto)
{
    if (validez) {
        inventario.push_back(std::unique_ptr<Objetos>(objeto));
    }
    return "";
}

Skins* Protagonista::showInventario()
{
    return nullptr;
}

// ---------------------------------------------------------------------------
// removeFromInventario: elimina el ítem en la posición index del inventario
// ---------------------------------------------------------------------------
void Protagonista::removeFromInventario(int index)
{
    if (index >= 0 && index < static_cast<int>(inventario.size()))
        inventario.erase(inventario.begin() + index);
}

void Protagonista::movimientos(sf::Keyboard* key)
{
    bool moving = false;

    if (key->isKeyPressed(sf::Keyboard::A)) {
        if (getSkin()) {
            getSkin()->getSprite()->move(-static_cast<float>(getVelocidad()), 0.f);
        }
        moving = true;
        if (activeAnim != "run") {
            activeAnim = "run";
            if (skinMap.count("run")) skinMap["run"]->playAnimation("run");
        }
    }

    if (key->isKeyPressed(sf::Keyboard::D)) {
        if (getSkin()) {
            getSkin()->getSprite()->move(static_cast<float>(getVelocidad()), 0.f);
        }
        moving = true;
        if (activeAnim != "run") {
            activeAnim = "run";
            if (skinMap.count("run")) skinMap["run"]->playAnimation("run");
        }
    }

    if (!moving && activeAnim == "run") {
        activeAnim = "idle";
        if (skinMap.count("idle")) skinMap["idle"]->playAnimation("idle");
    }

    if (key->isKeyPressed(sf::Keyboard::Space)) {
        if (activeAnim != "jump") {
            activeAnim = "jump";
            if (skinMap.count("jump")) skinMap["jump"]->playAnimation("jump");
        }
    }

    if (key->isKeyPressed(sf::Keyboard::J)) {
        if (activeAnim != "attack") {
            activeAnim = "attack";
            if (skinMap.count("attack")) skinMap["attack"]->playAnimation("attack");
        }
    }

    // Once a non-looping animation finishes, return to idle.
    auto it = skinMap.find(activeAnim);
    if (it != skinMap.end()) {
        if (it->second->getAnimSystem().isFinished() && activeAnim != "idle") {
            activeAnim = "idle";
            if (skinMap.count("idle")) skinMap["idle"]->playAnimation("idle");
        }
    }
}

sf::Sprite* Protagonista::barraDeVida()
{
    sf::IntRect cuadro;
    healthBarTexture.loadFromFile("Sprite/MONSTER/ICONS/helth_bar.png");
    healthBarSprite.setTexture(healthBarTexture);

    if (this->getVida() == 5) {
        cuadro = sf::IntRect(0, 0, 32, 32);
    }
    else if (this->getVida() == 4) {
        cuadro = sf::IntRect(32, 0, 32, 32);
    }
    else if (this->getVida() == 3) {
        cuadro = sf::IntRect(64, 0, 32, 32);
    }
    else if (this->getVida() == 2) {
        cuadro = sf::IntRect(0, -32, 32, 32);
    }
    else if (this->getVida() <= 1) {
        cuadro = sf::IntRect(32, -32, 32, 32);
    }

    healthBarSprite.setTextureRect(cuadro);
    return &healthBarSprite;
}

int Protagonista::ataque(Armas* arma)
{
    // Guard: si no hay arma equipada (llamada directa desde el combat loop
    // sin pasar un objeto Armas), retornamos el daño base de puño.
    if (!arma) return 20;

    // Ejemplo: si el arma es CAC, se puede encantar con operator+:
    // CAC espada(15, 30.f); Filo filo("Afilado", 5);
    // CAC espada_afilada = espada + filo;  // daño = 20
    // La espada encantada tiene espada_afilada.getEnchantmentDesc() == "Afila armas"

    return arma->getDanio();
}
