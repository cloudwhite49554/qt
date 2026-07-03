#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "gameobject.h"

class Zombie : public GameObject
{
public:
    explicit Zombie(QWidget *parent = nullptr);
    QPoint start,end;
    QPoint val;
    bool light=0;
};

#endif // ZOMBIE_H
