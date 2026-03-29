#pragma once
#ifndef COFRE_H
#define COFRE_H
#include <iostream>
#include <vector>
#include "Objetos.h"
using namespace std;

class Cofre {
private:
	int monedas;
	vector<Objetos*> objetos;
public:
	Cofre(int monedas, vector<Objetos*> objetos);
	void setMonedas(int monedas);
	int getMonedas();
};
#endif // !COFRE_H
