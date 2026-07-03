#include "jar.h"

Jar::Jar(QWidget *parent)
    : GameObject(parent)
{
    this->resize(40, 40);
    type="jar";
    dropLetters << "J";
    this->setImage(":/images/jar.png");
}