#pragma once
#ifndef NORMALES_H
#define NORMALES_H
#include "enemigos.h"
#include <iostream>
using namespace std;

// ---------------------------------------------------------------------------
// Normales — subclase concreta base para enemigos terrestres normales.
// Constructor ahora es público para que Skeleton (y futuras subclases) puedan
// llamarlo directamente desde su lista de inicialización.
//
// movimientos() se implementa como no-op: los enemigos Normales dirigen su
// propio movimiento mediante IA (ver Skeleton::update()), no por teclado.
// Esto satisface el virtual puro de Animaciones y permite instanciar
// subclases concretas (p. ej. en std::vector<Skeleton>).
//
// ataque() también se provee como default de emergencia para cumplir con
// la interfaz Combate; las subclases deben sobreescribirlo.
// ---------------------------------------------------------------------------
class Normales : public Enemigos {
public:
    Normales();
    Normales(int vida, int velocidad);

    // Retorna 0 como tipo genérico; las subclases deben sobreescribirlo.
    int getTipo() override;

    // Animaciones interface — no-op para enemigos controlados por IA.
    void movimientos(Keyboard* key) override { (void)key; }

    // Combate interface — default de emergencia (retorna 0).
    int ataque(Armas* arma) override { (void)arma; return 0; }
};

#endif
