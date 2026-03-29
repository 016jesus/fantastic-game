#pragma once
#ifndef JEFE_H
#define JEFE_H
#include"enemigos.h"
#include<iostream>
using namespace std; 
class Jefe : public Enemigos {
public: 
	Jefe();
	int ataqueEspecial();
	~Jefe();
	};
#endif //falta la sobrecarga del operador -=
