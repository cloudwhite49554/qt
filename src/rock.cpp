#include "rock.h"
#include <QTimer>

Rock::Rock(bool isShadow, QWidget *parent)
    : GameObject(parent), isShadow(isShadow)
{
    setFixedSize(40, 40);
    type="rock";
    dropLetters << "C" << "K";

    if (isShadow) {
        this->setStyleSheet("background-color: rgba(0,150,255,0.4); border-radius: 20px;");
    } else {
        this->setImage(":/images/rock.png");
    }
}

void Rock::land()
{
    isLanded = true;
}