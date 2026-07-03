#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>

class GameObject : public QWidget
{
    Q_OBJECT
public:
    explicit GameObject(QWidget *parent = nullptr);
    ~GameObject(); // 析构函数

    // 设置图片
    void setImage(const QString &imagePath);

    QString type="wall";
    int number;
    QStringList dropLetters;

    // 虚函数：获取碰撞矩形（以后树、石头可以重写这个）
    virtual QRect getRect();

signals:
protected:
    QLabel *imageLabel;  // 用于显示图片
};

#endif // GAMEOBJECT_H