#ifndef COFRE_H
#define COFRE_H
#include <iostream>
#include "cofre.h"

Cofre::Cofre(int monedas, vector<Objetos*> objetos): monedas(monedas), objetos(objetos){}

void Cofre::setMonedas(int moneda) {
	this->monedas = monedas; 
}
int Cofre::getMonedas() {
	return this->monedas; 
}

#endif

