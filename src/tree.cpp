#include "tree.h"

Tree::Tree(QWidget *parent)
    : GameObject(parent)
{
    this->resize(40, 40);
    type="tree";
    dropLetters << "T" << "R" << "E";

    // 加载图片
    this->setImage(":/images/tree.png");
}