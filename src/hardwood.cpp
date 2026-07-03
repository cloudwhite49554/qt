#include "hardwood.h"

Hardwood::Hardwood(QWidget *parent)
    : GameObject(parent)
{
    this->resize(132, 40);
    type="hardwood";
    this->setImage(":/images/hardwood.png");
}