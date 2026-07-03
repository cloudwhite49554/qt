#ifndef PLAYER_H
#define PLAYER_H

#include "gameobject.h"

class Player : public GameObject
{
    Q_OBJECT

public:
    // 构造函数
    explicit Player(QWidget *parent = nullptr);

    // 设置玩家朝向（用于手电筒方向）
    void setFacing(int direction);

    // 获取玩家朝向
    int getFacing() const { return facing; }

    // 玩家属性
    int health = 100;
    int maxHealth = 100;
    bool isInvincible = false;

    // 玩家方法
    void takeDamage(int damage);  // 受到伤害
    void heal(int amount);        // 恢复生命

signals:
    void gameOver();  // 死亡信号

private:
    int facing = 0;   // 0:上, 1:下, 2:左, 3:右
    QTimer *invincibleTimer;  // 无敌计时器

private slots:
    void onInvincibleEnd();  // 无敌结束
protected:
    QLabel *imageLabel;  // ← 确保这一行存在
};

#endif // PLAYER_H