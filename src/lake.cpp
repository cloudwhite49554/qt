#include "lake.h"

Lake::Lake(QWidget *parent)
    : GameObject(parent)
{
    this->resize(400, 400);
    type="lake";
    this->setStyleSheet("background-color: blue; border-radius: 5px;");
    this->setImage(":/images/lake.png");
}