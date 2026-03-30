#ifndef GOBLIN_H
#define GOBLIN_H
#include "entities/base/normales.h"
class Goblin: public Normales{
    private:
        vector<string> skinGoblin;
    public:
        Goblin();
        ~Goblin();
};

Goblin& operator-=(Goblin& obj, Protagonista& protagonista);

#endif
