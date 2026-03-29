#include "skins.h"
#include <fstream>

Skins::Skins(IntRect* dimension, Texture *textura):
    dimension(dimension), sprite(new Sprite(*textura)){}


void Skins::setDimension(IntRect* dimension)
{
    this->dimension = dimension;
}

Sprite* Skins::getSprite()
{
    return this->sprite;
}

vector<string>* Skins::getSkinFiles(){
    return &this->skinFiles;
}

void Skins::setSkinFiles(string archivo)
{
	ifstream archivoRutas(archivo);
	if (archivoRutas.is_open()) {
		string ruta;
		while (getline(archivoRutas, ruta)) {
			this->getSkinFiles()->push_back(ruta);
		}
		archivoRutas.close();

	}
}
