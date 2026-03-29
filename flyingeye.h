#ifndef FLYINGEYE_H
#define FLYINGEYE_H
#include "normales.h"
class flyingEye:public Normales{
    private:
        vector<string> skinFlyingEye;
    public:
        flyingEye();
        ~flyingEye();
        flyingEye operator -=( flyingEye &obj, Protagonista &protagonista);
};
#endif