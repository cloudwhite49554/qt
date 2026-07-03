#include "gold.h"

Gold::Gold(QWidget *parent)
    : GameObject(parent)
{
    this->resize(40, 40);
    type="gold";
    dropLetters << "G" << "D";
    this->setImage(":/images/gold.png");
}