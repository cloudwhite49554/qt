#include "gameobject.h"

GameObject::GameObject(QWidget *parent)
    : QWidget{parent}
{
    this->resize(50, 50);
    setAttribute(Qt::WA_StyledBackground, true);

    // 创建图片标签
    imageLabel = new QLabel(this);
    imageLabel->setGeometry(0, 0, width(), height());
    imageLabel->setScaledContents(true);
    imageLabel->setAlignment(Qt::AlignCenter);
}

GameObject::~GameObject()
{
}

void GameObject::setImage(const QString &imagePath)
{
    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        // 强制拉伸填满整个物体大小
        QPixmap scaled = pixmap.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(scaled);
        imageLabel->setGeometry(0, 0, this->width(), this->height());
        imageLabel->setScaledContents(false);
        this->setStyleSheet("");
    }
}

QRect GameObject::getRect()
{
    return QRect(this->x(), this->y(), this->width(), this->height());
}