#pragma once
#ifndef ARMAELEMENTAL_H
#define ARMAELEMENTAL_H
#include "Especial.h"
#include <iostream>
using namespace std;
class ArmaElemental: public Especial{
	private:
		string bonus;
	public:
		ArmaElemental(string bonus);
		string getBonus();
		void setBonus(string bonus);
};



#endif