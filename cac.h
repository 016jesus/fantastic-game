#pragma once
#ifndef CAC_H
#define CAC_H
#include "Armas.h"
#include <SFML/Graphics.hpp>
class CAC : public Armas {
	private:
		float rango;
	public: 
		CAC(int danio, float rango);
		//CAC operator +(CAC& obj, Encantamientos& encantamiento);//
		void setRango(float rango);
		float getRango();
		int validarRango(float x, float y);
		~CAC();
};

#endif