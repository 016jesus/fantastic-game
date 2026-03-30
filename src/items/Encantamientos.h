#pragma once
#ifndef ENCANTAMIENTOS_H
#define ENCANTAMIENTOS_H
using namespace std;
#include <iostream>
class Encantamientos{
	public:
		virtual string atributo() = 0;
		virtual int getTipo() = 0; 
};
#endif