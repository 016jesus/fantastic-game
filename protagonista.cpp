#include "protagonista.h"
#include <cmath>
#include <fstream>
Protagonista::Protagonista(int vida, int velocidad, string nombre, int dinero):
	Dociles(vida, velocidad, nombre), dinero(dinero){}

int Protagonista::getDinero()
{
	return this->dinero;
}

void Protagonista::setDinero(int dinero)
{
	this->dinero = dinero;
}





string Protagonista::actualizarInventario(bool validez, Objetos* objeto)
{
	//por implementar
	if (validez) {
		inventario.push_back(objeto);
	}
	else {
		
	}
	return "";
}

Skins* Protagonista::showInventario()
{
	//por implementar
	return nullptr;
}



void Protagonista::movimientos(Keyboard* key)
{

	string archivo = "rutas_prota.txt";
	

	//cambiar el sprite segun la tecla que se presione
	
	//no esta lista la funcionalidad aún
	if (key->isKeyPressed(Keyboard::A)){

		getSkin()->getSprite()->move(-5, 0);

	}
	
}

Sprite* Protagonista::barraDeVida()
{
	Sprite sprite;
	Texture t;
	IntRect cuadro;
	t.loadFromFile("Sprite/MONSTER/ICONS/helth_bar.png");
	if (this->getVida() == 5) {
		IntRect cuadro(0, 0, 32, 32);
		sprite.setTextureRect(cuadro);

	}
	else if (this->getVida() == 4) {
		IntRect cuadro(32, 0, 32, 32);
		sprite.setTextureRect(cuadro);
	}
	else if (this->getVida() == 3) {
		IntRect cuadro(64, 0, 32, 32);
		sprite.setTextureRect(cuadro);
	}
	else if (this->getVida() == 2) {
		IntRect cuadro(0, -32, 32, 32);
		sprite.setTextureRect(cuadro);
	}
	else if (this->getVida() == 1) {
		IntRect cuadro(32, -32, 32, 32);
		sprite.setTextureRect(cuadro);
	}
	else if (this->getVida() < 0 ) {
		IntRect cuadro(32, -32, 32, 32);
		sprite.setTextureRect(cuadro);
	}
	
	return &sprite;

}

int Protagonista::ataque(Armas* arma)
{
	return arma->getDanio();
}


