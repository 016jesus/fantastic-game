#pragma once
#ifndef ENEMIGOS_H
#define ENEMIGOS_H
#include "entities/base/entidad.h"
#include "entities/base/combate.h"
#include "entities/player/protagonista.h"
using namespace std; 
class Enemigos : public Entidad, public Combate{
public: 
	Enemigos(); 
	Enemigos(int vida, int velocidad); 
	virtual int getTipo() = 0; 
	sf::Sprite* barraDeVida() override;
}; 
#endif 
//clase abstracta enemigos 