#pragma once
//clase abstracta
#ifndef SKINS_H
#define SKINS_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;
using namespace sf;
class Skins {
	private:
		IntRect* dimension;
		Sprite *sprite;
		vector <string> skinFiles;
	public:
		Skins(IntRect* dimension, Texture* textura);
		void setDimension(IntRect* dimension);
		Sprite* getSprite();
		void setSkinFiles(string archivo);
		vector <string>* getSkinFiles();
};


#endif 
