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
		inventario.push_back(std::unique_ptr<Objetos>(objeto));
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

	//no esta lista la funcionalidad a�n
	if (key->isKeyPressed(Keyboard::A)){

		getSkin()->getSprite()->move(-5, 0);

	}

}

Sprite* Protagonista::barraDeVida()
{
	IntRect cuadro;
	healthBarTexture.loadFromFile("Sprite/MONSTER/ICONS/helth_bar.png");
	healthBarSprite.setTexture(healthBarTexture);

	if (this->getVida() == 5) {
		cuadro = IntRect(0, 0, 32, 32);
	}
	else if (this->getVida() == 4) {
		cuadro = IntRect(32, 0, 32, 32);
	}
	else if (this->getVida() == 3) {
		cuadro = IntRect(64, 0, 32, 32);
	}
	else if (this->getVida() == 2) {
		cuadro = IntRect(0, -32, 32, 32);
	}
	else if (this->getVida() == 1) {
		cuadro = IntRect(32, -32, 32, 32);
	}
	else if (this->getVida() < 0) {
		cuadro = IntRect(32, -32, 32, 32);
	}

	healthBarSprite.setTextureRect(cuadro);
	return &healthBarSprite;
}

int Protagonista::ataque(Armas* arma)
{
	return arma->getDanio();
}
