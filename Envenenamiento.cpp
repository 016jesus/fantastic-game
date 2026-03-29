#ifndef ENVENENAMIENTO_CPP
#define ENVENENAMIENTO_CPP
#include "Envenenamiento.h"
Envenenamiento::Envenenamiento(){}
Envenenamiento::Envenenamiento(string descripcion, int efecto): Bonus(descripcion), efecto(efecto){}
int Envenenamiento::getEfecto() {
	return 8;
}
int Envenenamiento::getTipo() {
	return 2;
}
string Envenenamiento::atributo(){
	return "Envenena";
}
//virtual int tipoRandom();


#endif