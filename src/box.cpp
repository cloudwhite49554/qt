#include "box.h"

Box::Box(QWidget *parent)
    : GameObject(parent)
{
    this->resize(40, 40);
    type="box";
    dropLetters << "B" << "X";
    this->setImage(":/images/box.png");
}