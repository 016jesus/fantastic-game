#pragma once
#ifndef CAC_H
#define CAC_H
#include "items/Armas.h"
#include "items/Filo.h"
#include "items/Envenenamiento.h"
#include <SFML/Graphics.hpp>
#include <string>

class CAC : public Armas {
	private:
		float rango;
		std::string enchantmentDesc;  // descripción del encantamiento activo
	public:
		CAC(int danio, float rango);
		void setRango(float rango);
		float getRango();
		int validarRango(float x, float y);
		int getTamanio() override { return 1; }  // implementa Armas::getTamanio puro

		const std::string& getEnchantmentDesc() const;
		void setEnchantmentDesc(const std::string& desc);

		~CAC();
};

// Aplica un encantamiento a un arma CAC — retorna nueva arma con daño ajustado
CAC operator+(CAC weapon, const Filo& enc);
CAC operator+(CAC weapon, const Envenenamiento& enc);

#endif