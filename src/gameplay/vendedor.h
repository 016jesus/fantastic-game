#pragma once
#ifndef VENDEDOR_H
#define VENDEDOR_H
#include "entities/base/dociles.h"
#include "items/Pociones.h"
#include "items/Sanadoras.h"
#include "items/cac.h"
#include <vector>
#include <string>
#include <memory>

class Vendedor : public Dociles {
public:
    Vendedor();
    ~Vendedor() = default;

    // Retorna el catálogo disponible (punteros no-owning al stock interno)
    std::vector<Pociones*> mostrarTienda() const;

    // Valida si el precio está dentro del presupuesto del comprador
    bool validarPrecio(int precio, int dineroComprador) const;

    // Easter egg — se activa si el jugador compra exactamente 3 ítems
    std::string easterEgg() const;

    // Incrementa el contador de compras
    void registrarCompra();
    int getComprasRealizadas() const;

private:
    // Stock del vendedor — se restock en el constructor
    std::vector<std::unique_ptr<Pociones>> stock;
    int comprasRealizadas = 0;

    void initStock();
};
#endif
