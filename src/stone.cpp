#include "stone.h"

Stone::Stone(QWidget *parent)
    : GameObject(parent)
{
    this->resize(40, 40);
    type="stone";
    this->setImage(":/images/stone.png");
}