#include "npc.h"

NPC::NPC(QWidget *parent)
    : GameObject(parent)
{
    this->resize(50, 80);
    type="NPC";
    this->setStyleSheet("background-color: green; border-radius: 5px;");
}

void NPC::setNPCImage(int npcNum)
{
    switch(npcNum) {
    case 1:
        this->setImage(":/images/npc_1.png");  // 渔夫
        break;
    case 2:
        this->setImage(":/images/npc_2.png");  // 伐木工
        break;
    case 3:
        this->setImage(":/images/npc_3.png");  // 养蜂人
        break;
    case 4:
        this->setImage(":/images/npc_4.png");  // 女王
        break;
    default:
        this->setImage(":/images/npc.png");    // 默认NPC图片
        break;
    }
}