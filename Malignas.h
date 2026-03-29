#pragma once
#ifndef MALIGNAS_H
#define MALIGNAS_H
#include "Pociones.h"
#include "Armas.h"

class Malignas :public Pociones, virtual public Armas {
	 public:
		 Malignas();
		 Malignas(bool valido, int precio, int danio);
		 ~Malignas();
		 int efecto()override;
		 int getTamanio()override;
};


#endif