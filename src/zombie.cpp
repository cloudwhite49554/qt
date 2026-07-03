#include "zombie.h"

Zombie::Zombie(QWidget *parent)
    : GameObject(parent)
{
    this->resize(50, 80);
    type="zombie";
    this->setImage(":/images/zombie.png");
}