#pragma once
#ifndef INVENTARIO_H
#define INVENTARIO_H
#include <string>

class Skins;
class Objetos;

class Inventario {
public:
	virtual Skins* showInventario() = 0;
	virtual std::string actualizarInventario(bool validez, Objetos* objeto) =  0;

};




#endif