#pragma once
#ifndef DOCILES_H
#define DOCILES_H
#include "entidad.h"
#include <string>
class Dociles : public Entidad {
	private:
		std::string nombre;
	public:
		Dociles(int vida, int velocidad, std::string nombre);
		std::string getNombre();
		void setNombre(std::string nombre);
};


#endif