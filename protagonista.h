#pragma once
#include "dociles.h"
#include "Objetos.h"
#include "inventario.h"
#include "combate.h"
#include <memory>
#include <unordered_map>
#ifndef PROTAGONISTA_H
#define PROTAGONISTA_H

class Protagonista : public Combate, public Dociles, public Inventario {
    private:
        int dinero;
        std::vector<std::unique_ptr<Objetos>> inventario;
        sf::Sprite healthBarSprite;
        sf::Texture healthBarTexture;

        // One Skins object per animation (separate sprite-sheet files).
        std::unordered_map<std::string, std::unique_ptr<Skins>> skinMap;
        // Name of the currently active animation.
        std::string activeAnim;

    public:
        //metodos propios de la clase
        Protagonista(int vida, int velocidad, std::string nombre, int dinero);
        int getDinero();
        void setDinero(int dinero);

        // Acceso de solo lectura al inventario (para overlays de UI)
        const std::vector<std::unique_ptr<Objetos>>& getInventario() const { return inventario; }
        // Elimina un ítem del inventario por índice (0-based)
        void removeFromInventario(int index);

        // Loads all protagonist sprite sheets and registers their animations.
        void loadSprites();

        // Advances the active animation by deltaTime seconds.
        void updateAnimation(float deltaTime);

        //metodos clase inventario
        std::string actualizarInventario(bool validez, Objetos* objeto) override;
        Skins* showInventario() override;

        //metodos clase animaciones
        void movimientos(sf::Keyboard* key) override;

        //metodos clase combate
        sf::Sprite* barraDeVida() override;
        int ataque(Armas* arma) override;
};

#endif
