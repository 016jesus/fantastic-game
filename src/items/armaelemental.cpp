#include "items/armaelemental.h"
ArmaElemental::ArmaElemental(string bonus): bonus(bonus){}
string ArmaElemental::getBonus() {
	return "BONUS";
}
void ArmaElemental::setBonus(string bonus) {
	this->bonus = bonus;
}
