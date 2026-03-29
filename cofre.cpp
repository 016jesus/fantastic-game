#include "cofre.h"

Cofre::Cofre(int monedas)
    : monedas(monedas), opened(false)
{
}

Cofre::Cofre(int monedas, std::vector<std::unique_ptr<Objetos>> items)
    : monedas(monedas), objetos(std::move(items)), opened(false)
{
}

void Cofre::setMonedas(int monedas)
{
    this->monedas = monedas;
}

int Cofre::getMonedas() const
{
    return this->monedas;
}

void Cofre::addObjeto(std::unique_ptr<Objetos> obj)
{
    objetos.push_back(std::move(obj));
}

std::vector<std::unique_ptr<Objetos>> Cofre::takeObjetos()
{
    return std::move(objetos);
}

bool Cofre::isOpened() const
{
    return opened;
}

void Cofre::setOpened(bool opened)
{
    this->opened = opened;
}

std::string Cofre::describe() const
{
    if (objetos.empty()) {
        return "Contiene: " + std::to_string(monedas) + " monedas";
    }
    return "Contiene: " + std::to_string(monedas) + " monedas y "
           + std::to_string(objetos.size()) + " ítem(s)";
}
