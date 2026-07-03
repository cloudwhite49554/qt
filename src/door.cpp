#include "door.h"

Door::Door(QWidget *parent)
    : GameObject(parent)
{
    this->resize(100, 50);
    type="door";
    this->setStyleSheet("background-color: brown; border-radius: 5px;");

}