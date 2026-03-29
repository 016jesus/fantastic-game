#ifndef ARMAS_CPP
#define ARMAS_CPP
#include "Armas.h"
Armas::Armas(){}
Armas::Armas(int danio): danio(danio){}
int Armas::getDanio() {
	return this->danio;
}
void Armas::setDanio(int danio) {
	this->danio = danio;
}
int Armas::getTipo() {
	return 2;
}


#endif