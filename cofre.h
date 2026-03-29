#pragma once
#ifndef COFRE_H
#define COFRE_H
#include <iostream>
#include <vector>
using namespace std; 

class Cofre {
private: 
	int monedas; 
	vector<Objetos*> objetos;
public: 
	Cofres(int monedas,vector<Objetos*> objetos){}
	void setMonedas(int monedas); 
	int getMonedas(); 
};
#endif // !COFRE_H
