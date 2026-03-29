#ifndef SKELETON_H
#define SKELETON_H
#include "normales.h"
class Skeleton:public Normales{
    private:
        vector<string> skinSkeleton;
    public: 
        Skeleton();
        ~Skeleton();
        Skeleton operator -=( Skeleton &obj, Protagonista &protagonista);
};
#endif