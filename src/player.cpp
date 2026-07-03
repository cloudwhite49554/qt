#include "player.h"
#include <QTimer>

// 构造函数
Player::Player(QWidget *parent)
    : GameObject(parent)
{
    this->resize(40, 40);
    type = "player";
    this->setImage(":/images/player.png");

    // 无敌计时器
    invincibleTimer = new QTimer(this);
    invincibleTimer->setSingleShot(true);
    connect(invincibleTimer, &QTimer::timeout, this, &Player::onInvincibleEnd);
}

// 设置朝向
void Player::setFacing(int direction)
{
    facing = direction;
}

// 受到伤害
void Player::takeDamage(int damage)
{
    if (isInvincible) return;

    health -= damage;
    if (health <= 0) {
        health = 0;
        emit gameOver();
    }

    // 无敌1秒
    isInvincible = true;
    invincibleTimer->start(1000);

    // 闪烁效果
    this->setStyleSheet("background-color: rgba(255,0,0,100);");
    QTimer::singleShot(100, this, [this]() {
        if (!isInvincible) {
            this->setStyleSheet("");
        }
    });
}

// 恢复生命
void Player::heal(int amount)
{
    health = qMin(maxHealth, health + amount);
}

// 无敌结束
void Player::onInvincibleEnd()
{
    isInvincible = false;
    this->setStyleSheet("");
}