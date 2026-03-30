#pragma once
#ifndef ARMAS_H
#define ARMAS_H
#include "items/Objetos.h"
#include "items/Encantamientos.h"
class Armas : virtual public Objetos {
	private:
		int danio;
	public:
		Armas();
		Armas(int danio);
		int getDanio();
		void setDanio(int danio);
		int getTipo()override;
		virtual int getTamanio() = 0;
};

#endif