#include "tpdoor.h"

Tpdoor::Tpdoor(QWidget *parent)
    : GameObject(parent) // 调用父类的构造函数
{
    // 设置石头的外观
    this->setStyleSheet("background-color: rgba(0,0,0,0.3); border-radius: 5px;");
    this->resize(50, 80); // 设置大小
    type="tpdoor";

}
