#ifndef GOBLIN_H
#define GOBLIN_H
#include "normales.h"
class Goblin: public Normales{
    private:
        vector<string> skinGoblin;
    public:
        Goblin();
        ~Goblin();
};

Goblin& operator-=(Goblin& obj, Protagonista& protagonista);

#endif
