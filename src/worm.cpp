#include "worm.h"

Worm::Worm(QWidget *parent)
    : GameObject(parent)
{
    this->resize(40, 40);
    type="worm";
    dropLetters << "W" << "M";
    this->setImage(":/images/worm.png");
}