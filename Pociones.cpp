#ifndef POCIONES_CPP
#define POCIONES_CPP
#include "Pociones.h"
Pociones::Pociones(){}
Pociones::Pociones(bool valido, int precio) :valido(valido), precio(precio){}
bool Pociones::getValido() {
	return this->valido;
}
int Pociones::getPrecio() {
	return this->precio;
}
void Pociones::setValido(bool valido) {
	this->valido = valido;
}
void Pociones::setPrecio(int precio) {
	this->precio = precio;
}
int Pociones::getTipo() {
	return 1;
}

#endif