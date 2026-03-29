#ifndef SANADORAS_CPP
#define SANADORAS_CPP
#include "Sanadoras.h"
Sanadoras::Sanadoras(){}
Sanadoras::Sanadoras(bool valido, int precio, int aumento):Pociones(valido, precio), aumento(aumento){}
Sanadoras::~Sanadoras(){}
int Sanadoras::getAumento() {
	return this->aumento;
}
void Sanadoras::setAumento(int aumento) {
	this->aumento = aumento;
}
int Sanadoras::efecto() {
	return aumento;
}

#endif