#ifndef ARMAELEMENTAL_CPP
#define ARMAELEMENTAL_CPP
#include "armaelemental.h"
ArmaElemental::ArmaElemental(string bonus): bonus(bonus){}
string ArmaElemental::getBonus() {
	return "BONUS";
}
void ArmaElemental::setBonus(string bonus) {
	this->bonus = bonus;
}


#endif
