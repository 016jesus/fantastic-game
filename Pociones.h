#pragma once
#ifndef POCIONES_H
#define POCIONES_H
#include "Objetos.h"
class Pociones :public Objetos {
	private:
		bool valido;
		int precio;
	public:
		Pociones();
		Pociones(bool valido, int precio);
		bool getValido();
		int getPrecio();
		void setValido(bool valido);
		void setPrecio(int precio);
		virtual int efecto()=0;
		int getTipo()override;
};

#endif