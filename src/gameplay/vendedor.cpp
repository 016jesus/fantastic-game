#include "gameplay/vendedor.h"

Vendedor::Vendedor()
    : Dociles(10, 0, "Mercader")
{
    initStock();
}

void Vendedor::initStock()
{
    stock.push_back(std::make_unique<Sanadoras>(true, 10, 20));  // poción pequeña
    stock.push_back(std::make_unique<Sanadoras>(true, 20, 50));  // poción grande
    stock.push_back(std::make_unique<Sanadoras>(true, 5, 10));   // poción mínima
}

std::vector<Pociones*> Vendedor::mostrarTienda() const
{
    std::vector<Pociones*> catalogo;
    catalogo.reserve(stock.size());
    for (const auto& item : stock) {
        catalogo.push_back(item.get());
    }
    return catalogo;
}

bool Vendedor::validarPrecio(int precio, int dineroComprador) const
{
    return dineroComprador >= precio;
}

std::string Vendedor::easterEgg() const
{
    if (comprasRealizadas >= 3) {
        return "¡Sabía que volverías! Toma esto... pero no se lo digas a nadie.";
    }
    return "";
}

void Vendedor::registrarCompra()
{
    comprasRealizadas++;
}

int Vendedor::getComprasRealizadas() const
{
    return comprasRealizadas;
}
