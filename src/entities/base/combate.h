#pragma once
//clase interfaz
#ifndef COMBATE_H
#define COMBATE_H
#include "items/Armas.h"
#include <SFML/Graphics.hpp>

class Combate{
	public:
		virtual sf::Sprite* barraDeVida() = 0;
		virtual int ataque(Armas* arma) = 0;
};



#endif 
