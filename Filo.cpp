#ifndef FILO_CPP
#define FILO_CPP
#include "Filo.h"
Filo::Filo(){}
Filo::Filo(string descripcion, int aumento): Bonus(descripcion), aumento(aumento){}
int Filo::getAumento() {
	return this->aumento;
}
void Filo::setAumento(int aumento) {
	this->aumento = aumento;
}
int Filo::getTipo() {
	return 1;
}
string Filo::atributo(){
	return "Afila armas";
}

#endif
