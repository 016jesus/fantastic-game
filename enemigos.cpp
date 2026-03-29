#ifndef ENEMIGOS_CPP
#define ENEMIGOS_CPP
#include "enemigos.h"
#include<iostream>
Enemigos::Enemigos(int vida, int velocidad): Entidad(vida,velocidad){}
Sprite* Enemigos::barraDeVida()
{
    return nullptr;
}
Enemigos::Enemigos(){}
#endif