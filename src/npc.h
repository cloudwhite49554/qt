#ifndef NPC_H
#define NPC_H

#include "gameobject.h"

class NPC : public GameObject
{
public:
    explicit NPC(QWidget *parent = nullptr);
    int num=0;
    void setNPCImage(int npcNum);  // 根据编号设置不同图片
};

#endif // NPC_H