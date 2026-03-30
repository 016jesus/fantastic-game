#include "enemigos.h"
#include<iostream>
Enemigos::Enemigos(int vida, int velocidad): Entidad(vida,velocidad){}
sf::Sprite* Enemigos::barraDeVida()
{
    return nullptr;
}
Enemigos::Enemigos(){}
