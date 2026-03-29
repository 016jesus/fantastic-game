#pragma once
#ifndef ENVENENAMIENTO_H
#define ENVENENAMIENTO_H
#include "Bonus.h"
class Envenenamiento : public Bonus {
	private:
		int efecto;
	public:
		Envenenamiento();
		Envenenamiento(string descripcion, int efecto);
		int getEfecto();
		int getTipo()override;
		string atributo()override;
		//virtual int tipoRandom();

};


#endif