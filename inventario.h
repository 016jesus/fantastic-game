#pragma once
#ifndef INVENTARIO_H
#define INVENTARIO_H

class Inventario {

	virtual Skins* showInventario() = 0;
	virtual string actualizarInventario(bool validez, Objetos* objeto) =  0;

};




#endif