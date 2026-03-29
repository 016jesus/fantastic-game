#pragma once
#ifndef DOCILES_H
#define DOCILES_H
#include "entidad.h"
class Dociles : public Entidad {
	private:
		string nombre;
	public:
		Dociles(int vida, int velocidad, string nombre);
		string getNombre();
		void setNombre(string nombre);
};


#endif