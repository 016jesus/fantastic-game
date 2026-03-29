#ifndef CAC_CPP
#define CAC_CPP
#include "cac.h"

CAC::CAC(int danio, float rango): Armas(danio), rango(rango){}
//CAC operator +(CAC& obj, Encantamientos& encantamiento);//
void CAC::setRango(float rango) {
	this->rango = rango;
}
float CAC::getRango() {
	return this->rango;
}
int CAC::validarRango(float x, float y) {

}
CAC::~CAC(){}

#endif