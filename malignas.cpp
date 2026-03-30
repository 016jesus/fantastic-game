#include "Malignas.h"
Malignas::Malignas(){}
Malignas::Malignas(bool valido, int precio, int danio): Pociones(valido, precio), Armas(danio){}
Malignas::~Malignas(){}
int Malignas::efecto() {
	return 2;
}
int Malignas::getTamanio() {
	return 1;
}
int Malignas::getTipo() {
	return 3;
}
