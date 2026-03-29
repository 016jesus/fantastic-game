#pragma once
//clase interfaz
#ifndef ANIMACIONES_H
#define ANIMACIONES_H
#include <SFML/Graphics.hpp>
class Animaciones {
	public:
		virtual void movimientos(Keyboard* key) = 0;
};	

#endif 
