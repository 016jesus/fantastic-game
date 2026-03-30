#pragma once
#ifndef COFRE_H
#define COFRE_H
#include "items/Objetos.h"
#include <vector>
#include <memory>
#include <string>
using namespace std;

class Cofre {
public:
    // Constructor simple: cofre con monedas y sin ítems
    explicit Cofre(int monedas);
    // Constructor completo: con monedas e ítems
    Cofre(int monedas, std::vector<std::unique_ptr<Objetos>> items);

    void setMonedas(int monedas);
    int getMonedas() const;

    // Añade un ítem al cofre
    void addObjeto(std::unique_ptr<Objetos> obj);

    // Retorna los ítems (ownership transferida — el cofre queda vacío)
    std::vector<std::unique_ptr<Objetos>> takeObjetos();

    // Si ya fue abierto
    bool isOpened() const;
    void setOpened(bool opened);

    // Descripción del contenido para mostrar en UI
    std::string describe() const;

private:
    int monedas;
    std::vector<std::unique_ptr<Objetos>> objetos;
    bool opened = false;
};
#endif
