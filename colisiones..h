#pragma once
#ifndef COLISIONES_H
#define COLISIONES_H
class Colisiones {
	public:
		virtual float distanciaRelativa(float x, float y) = 0;
		virtual bool validarColision(float distancia) = 0;
	};
#endif
//interface Colisiones(el nombre es colisiones..h con dos puntos pq se me fue xd)