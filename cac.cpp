#include "cac.h"
#include <string>

CAC::CAC(int danio, float rango): Armas(danio), rango(rango){}

void CAC::setRango(float rango) {
	this->rango = rango;
}
float CAC::getRango() {
	return this->rango;
}
int CAC::validarRango(float x, float y) {
	(void)x; (void)y;
	return 0;
}

const std::string& CAC::getEnchantmentDesc() const {
	return enchantmentDesc;
}

void CAC::setEnchantmentDesc(const std::string& desc) {
	this->enchantmentDesc = desc;
}

CAC operator+(CAC weapon, const Filo& enc) {
	// getAumento() and atributo() are non-const in the hierarchy; cast away const safely
	// since neither method mutates observable state.
	weapon.setDanio(weapon.getDanio() + const_cast<Filo&>(enc).getAumento());
	weapon.setEnchantmentDesc(const_cast<Filo&>(enc).atributo());
	return weapon;
}

CAC operator+(CAC weapon, const Envenenamiento& enc) {
	weapon.setEnchantmentDesc(
		const_cast<Envenenamiento&>(enc).atributo() +
		" (" + std::to_string(const_cast<Envenenamiento&>(enc).getEfecto()) + " dmg/s)"
	);
	return weapon;
}

CAC::~CAC(){}
