#include "entities/base/dociles.h"

Dociles::Dociles(int vida, int velocidad, std::string nombre): Entidad(vida, velocidad){
	this->nombre = nombre;
}

std::string Dociles::getNombre()
{
	return this->nombre;
}
void Dociles::setNombre(std::string nombre) {
	this->nombre = nombre;
}
