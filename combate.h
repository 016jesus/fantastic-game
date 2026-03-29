#pragma once
//clase interfaz
#ifndef COMBATE_H
#define COMBATE_H
#include "Armas.h"

class Combate{
	public:
		virtual Sprite* barraDeVida() = 0;
		virtual int ataque(Armas* arma) = 0;
};



#endif 
