#pragma once
#ifndef FILO_H
#define FILO_H
#include "Bonus.h"
class Filo : public Bonus {
	private:
		int aumento;
	public:
		Filo();
		Filo(string descripcion, int aumento);
		void setAumento(int aumento);
		int getAumento();
		int getTipo()override;
		string atributo()override;
};



#endif