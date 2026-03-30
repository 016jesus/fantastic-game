#pragma once
#ifndef ENTIDAD_H
#define ENTIDAD_H
#include "entities/base/colisiones.h"
#include "entities/player/skins.h"
#include "entities/base/animaciones.h"
#include "entities/base/Position.h"
#include <memory>

//clase abstracta Entidad

class Entidad: public Animaciones, public Colisiones{
private:
	int vida;
	int velocidad;
	std::unique_ptr<Skins> skin;
	Position position;
public:
	Entidad();
	Entidad(int vida, int velocidad);
	int getVelocidad();
	int getVida();
	void setVelocidad(int velocidad);
	void setVida(int nuevaVida);
	Skins* getSkin();
	// Transfers ownership of a new Skins object into this entity.
	void setSkin(std::unique_ptr<Skins> newSkin);
	Position getPosition() const;
	void setPosition(const Position& pos);
	float distanciaRelativa(float x, float y) override;
	bool validarColision(float distancia) override;
};

#endif
