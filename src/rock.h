#ifndef ROCK_H
#define ROCK_H

#include "gameobject.h"

class Rock : public GameObject
{
public:
    explicit Rock(bool isShadow = false,QWidget *parent = nullptr);

    bool isShadow;
    bool isLanded = false;

    void land();
};

#endif // ROCK_H
