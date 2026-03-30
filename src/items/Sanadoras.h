#pragma once
#ifndef SANADORAS_H
#define SANADORAS_H
#include "items/Pociones.h"

class Sanadoras :public Pociones {
	private:
		int aumento;
	public:
		Sanadoras();
		Sanadoras(bool valido, int precio, int aumento);
		~Sanadoras();
		int getAumento();
		void setAumento(int aumento);
		int efecto()override;		
};

#endif