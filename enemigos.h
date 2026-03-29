#pragma once
#ifndef ENEMIGOS_H
#define ENEMIGOS_H
#include "entidad.h"
#include "combate.h"
#include "protagonista.h"
using namespace std; 
class Enemigos : public Entidad, public Combate{
public: 
	Enemigos(); 
	Enemigos(int vida, int velocidad); 
	virtual int getTipo() = 0; 
	Sprite* barraDeVida() override;
}; 
#endif 
//clase abstracta enemigos 