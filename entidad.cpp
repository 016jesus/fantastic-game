#ifndef ENTIDADES_CPP
#define ENTIDADES_CPP
#include "entidad.h"
#include<iostream>
Entidad::Entidad(){

}
Entidad::Entidad(int vida, int velocidad):vida(vida), velocidad(velocidad) {} 


int Entidad::getVelocidad() {
	return this->velocidad; 
}
int Entidad::getVida() {
	return this->vida; 
}
void Entidad::setVelocidad(int velocidad) {
	this->velocidad = velocidad; 
}
void Entidad::setVida(int vida) {
	this->vida = vida; 
}
Skins* Entidad::getSkin()
{
	return this->skin;
}
float Entidad::distanciaRelativa(float x2, float y2)
{
	float x1 = skin->getSprite()->getPosition().x;
	float y1 = skin->getSprite()->getPosition().y;

		return sqrt( pow(x1-x2, 2) + pow(y1-y2,2) );
}

bool Entidad::validarColision(float distancia) {
	//20 es un valor arbitrario, debe ser tomado a consideracion
	return distancia < 20.0f;
}
#endif 
