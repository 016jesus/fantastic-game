#include "entities/base/normales.h"

Normales::Normales() : Enemigos() {}

Normales::Normales(int vida, int velocidad) : Enemigos(vida, velocidad) {}

// Tipo 0 = enemigo normal genérico.
// Las subclases (Skeleton = 1, etc.) deben sobreescribir este método.
int Normales::getTipo() {
    return 0;
}
