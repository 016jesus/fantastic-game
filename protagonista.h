#pragma once
#include "dociles.h"
#include "Objetos.h"
#include "inventario.h"
#include "combate.h"
#include <memory>
#ifndef PROTAGONISTA_H
#define PROTAGONISTA_H

class Protagonista : public Combate, public Dociles, public Inventario {
	private:
		int dinero;
		std::vector<std::unique_ptr<Objetos>> inventario;
		sf::Sprite healthBarSprite;
		sf::Texture healthBarTexture;

	public:
		//metodos propios de la clase
		Protagonista(int vida, int velocidad, string nombre, int dinero);
		int getDinero();
		void setDinero(int dinero);

		//metodos clase inventario
		string actualizarInventario(bool validez, Objetos* objeto) override;
		Skins* showInventario() override;

		//metodos clase animaciones

		void movimientos(Keyboard* key)override;

		//metodos clase combate
		Sprite* barraDeVida() override;
		int ataque(Armas* arma) override;

};

#endif
