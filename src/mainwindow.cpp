#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameobject.h"
#include "tree.h"
#include "door.h"
#include "box.h"
#include "rock.h"
#include "stone.h"
#include "lake.h"
#include "jar.h"
#include "zombie.h"
#include "book.h"
#include "gold.h"
#include "tpdoor.h"
#include"hardwood.h"
#include "worm.h"
#include "npc.h"
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include<QDialog>
#include <QKeyEvent>
#include<qDebug>
#include<QPainter>
#include<QFile>
#include <QTextStream>
#include <cstdlib>
#include "player.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setAttribute(Qt::WA_TranslucentBackground);
    ui->setupUi(this);
    fishingWidget = new QWidget(this);
    fishingWidget->setGeometry(0, 0, 800, 600);
    fishingWidget->raise();
    fishingWidget->installEventFilter(this);
    fishingWidget->show();

    uiLayer = new QWidget(this);
    uiLayer->setGeometry(0, 0, width(), height());
    uiLayer->show();
    startWidget = new QWidget(this);
    pauseMenu = new QWidget(this);
    gameOverPanel = new QWidget(this);
    cgWidget=new QWidget(this);
    cgWidget->setStyleSheet(
        "QWidget {"
        "   background-repeat: no-repeat;"
        "   background-position: center;"
        "   border-image: url(:/images/cg.png) 0 0 0 0 stretch stretch;"
        "}"
        );


    cgWidget->setGeometry(0, 0, 800, 600);


    cgWidget->hide();


    cgWidget->raise();

    initStartMenu();
    initDialog();
    initBagUI();
    initGameOverUI();

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (dialogBox->isVisible()||gameOverPanel->isVisible()||pauseMenu->isVisible())
        return; // 对话框开着不能操作
    switch(event->key()){
    case Qt::Key_Q:
    {        
        break;
    }
    case Qt::Key_W: keyW = true; break;
    case Qt::Key_A: keyA = true; break;
    case Qt::Key_S: keyS = true; break;
    case Qt::Key_D: keyD = true; break;
    case Qt::Key_E: {
        keyE = true;
        if(fishgame)
        {
            if(rodCasting==0)
            {rodCasting = 1;}
        }
        else
        {
            QRect playerRect(
                player->x(),
                player->y(),
                player->width(),
                player->height()
                );
            const int interactRange = 5;
            playerRect.adjust(-interactRange, -interactRange,
                              interactRange,  interactRange);
            for (int i = 0; i < objects.size(); i++) {
                GameObject *obj = objects[i];
                if(!obj->isVisible())continue;
                QRect Rect(
                    obj->x(),
                    obj->y(),
                    obj->width(),
                    obj->height()
                    );
                if (obj->type=="tree")
                {
                    if (playerRect.intersects(Rect))
                    {
                        if(heldItem=="ax")
                        {
                            for (const QString& letter : obj->dropLetters)
                            {
                                inventory.append(letter); // 加入背包
                            }
                            if (!encyclopedia.contains("tree"))
                            {
                                showDialog("解锁了新物品：tree","旁白");
                                encyclopedia["tree"] = encyclopedia.size();
                            }
                            objects.removeAt(i);
                            obj->hide();
                            if(currentLevel==2)
                            {
                                obj2.removeAll(obj);
                            }
                            break; // 砍完一棵就退出
                        }
                        else{showDialog("我没有合适的工具！");}
                    }
                }
                if (obj->type=="gold")
                {
                    if (playerRect.intersects(Rect))
                    {

                        for (const QString& letter : obj->dropLetters)
                        {
                            inventory.append(letter); // 加入背包
                        }
                        if (!encyclopedia.contains("gold"))
                        {
                            QVector<DialogLine> lines = {
                                {"旁白","你在矿坑的深处发现了一具高度腐败的骷髅\n他的脚边是一页纸"},
                                {"日记4","只恨我的枪无法杀灭人心中的贪念\n愿天佑女王！（God Save The Queen!）"},
                                {"旁白", "解锁了新物品：gold"},
                                {"维克托", "这也许是个提示，是时候和僵尸们一决胜负了"}
                            };
                            startDialog(lines);
                            encyclopedia["gold"]=encyclopedia.size();
                        }
                        objects.removeAt(i);
                        obj->hide();
                        if(currentLevel==4)
                        {
                            obj41.removeAll(obj);
                        }
                        tutorialStep=2;
                        QPoint center = Rect.center()+QPoint(-20,20);
                        currentSave.lastCheckpoint = center;
                        currentSave.level=currentLevel;
                        currentSave.encyclopedia=encyclopedia;
                        currentSave.inventory=inventory;
                        currentSave.tutorialStep=tutorialStep;
                        currentSave.number = 0;
                        break;
                    }
                }
                if (obj->type=="rock"||obj->type=="stone")
                {
                    if (playerRect.intersects(Rect))
                    {
                        if(heldItem=="bat")
                        {
                            for (const QString& letter : obj->dropLetters)
                            {
                                inventory.append(letter); // 加入背包
                            }
                            if (!encyclopedia.contains("rock"))
                            {
                                showDialog("解锁了新物品：rock","旁白");
                                encyclopedia["rock"] = encyclopedia.size();
                            }
                            objects.removeAt(i);
                            obj->hide();
                            if(currentLevel==2)
                            {
                                obj2.removeAll(obj);
                            }
                            break;
                        }
                        else{showDialog("我无法撼动它");}
                    }
                }
                if (obj->type=="box")
                {
                    if (playerRect.intersects(Rect))
                    {
                        for (const QString& letter : obj->dropLetters)
                        {
                            inventory.append(letter); // 加入背包
                        }
                        if (!encyclopedia.contains("box"))
                        {
                            encyclopedia["box"] = encyclopedia.size();
                        }
                        objects.removeAt(i);
                        obj->hide();
                        break; // 砍完一棵就退出
                    }
                }
                if (obj->type=="jar")
                {
                    if (playerRect.intersects(Rect))
                    {
                        for (const QString& letter : obj->dropLetters)
                        {
                            inventory.append(letter); // 加入背包
                        }
                        if (!encyclopedia.contains("jar"))
                        {
                            showDialog("解锁了新物品：jar","旁白");
                            encyclopedia["jar"] = encyclopedia.size();
                        }
                        objects.removeAt(i);
                        obj31.removeAll(obj);
                        obj->hide();
                        break; // 砍完一棵就退出
                    }
                }
                if (obj->type=="book")
                {
                    if (playerRect.intersects(Rect))
                    {
                        Book* book = dynamic_cast<Book*>(obj);
                        if(book->num==1)
                        {
                            QVector<DialogLine> lines = {
                                {"日记1", "近来王国北部发现了一座规模巨大的金矿\n盗贼，奸商蜂拥而至"},
                                {"日记1", "奇怪的是，进入金矿的人都似乎丧失了理智\n他们甚至遗忘了自己的名字，且具有极强的攻击性"},
                                {"日记1", "作为女王忠实的勇士，我决意调查这一切"},
                                {"旁白", "年代久远，日记的封面已经模糊不清\n隐约可看出V……R这两个字母"}
                            };
                            startDialog(lines);
                        }
                        if(book->num==2)
                        {
                            QVector<DialogLine> lines = {
                                {"日记2", "越来越多无辜的群众变成了疯狂的僵尸（zombie）\n女王当机立断，封锁了金矿"},
                                {"日记2", "但危机并未结束\n人们逐渐遗忘了字母王国引以为傲的字母合成方法\n甚至不记得常见物品或是自己名字的拼写"},
                                {"日记2", "女王称之为字母失踪事件\n说实话，我一介武夫真不知道如何是好\n我只能驻守于此，祈求灾祸就此平息……"},
                            };
                            startDialog(lines);
                        }
                        if(book->num==3)
                        {
                            QVector<DialogLine> lines = {
                                {"日记3", "僵尸作战能力强悍\n一旦被贴身极难存活"},
                                {"日记3", "但其亦有弱点\n此生物高度目盲，行动轨迹固定，且极其畏光"},
                                {"日记3", "意味着只要被强光照射\n僵尸就会停止行动\n可加以利用"}
                            };
                            startDialog(lines);
                        }
                    }
                }
                if (obj->type=="door")
                {
                    if (playerRect.intersects(Rect)&&currentLevel==1)
                    {
                        clearGame();
                        rockgame=false;
                        loadLevel(2);
                        break;
                    }
                    if (playerRect.intersects(Rect)&&currentLevel==2)
                    {
                        clearGame();
                        loadLevel(3);
                        break;
                    }
                    if (playerRect.intersects(Rect)&&currentLevel==3)
                    {
                        if(heldItem!="key")
                        {
                            showDialog("门上锁了\n看起来我需要一把钥匙（key）");
                        }
                        else
                        {
                            clearGame();
                            loadLevel(4);
                        }
                        break;
                    }
                }
                if (obj->type=="lake")
                {
                    if (playerRect.intersects(Rect))
                    {
                        if(tutorialStep<=0){
                            if(heldItem!="rock")
                            {
                                showDialog("这个湖面看起来不是很平静","旁白");
                                tutorialStep=0;
                                break;
                            }
                            else
                            {
                                showDialog("你把石头扔进了湖中","旁白");
                                tutorialStep=1;
                                heldItem="空手";
                                break;
                            }
                        }
                        if(tutorialStep==1)
                        {
                            if (!encyclopedia.contains("ripple"))
                            {
                                showDialog("你从湖中抓住了一抹涟漪，解锁了新物品：ripple","旁白");
                                encyclopedia["ripple"] = encyclopedia.size();
                                inventory.append("P");
                                inventory.append("L");
                                tutorialStep=2;
                                break;
                            }
                        }
                        if(tutorialStep==2)
                        {
                            if(heldItem!="trap")
                            showDialog("湖里似乎有一些鱼在游动","维克多");
                            else
                            {
                                if(!encyclopedia.contains("fish"))
                                {
                                    fish = new GameObject(this);
                                    //fish->setStyleSheet("background-color: green; border-radius: 5px;");
                                    fish->setImage(":/images/fish.png");
                                    fish->resize(40,50);
                                    fish->move(150, 150);
                                    fish->show();
                                    player->move(400,280);
                                    fishVel=QPoint(0,1);
                                    rodLength  = 40;
                                    rodCasting = 0;
                                    fishCaught = 0;
                                    fishgame=1;
                                    break;
                                }
                            }
                        }
                        if(tutorialStep>=3)
                        {
                            showDialog("终于捕到鱼了\n去找伐木工吧","维克多");
                        }
                    }
                }
                if (obj->type=="hardwood")
                {
                    if (playerRect.intersects(Rect)){
                        if (tutorialStep<=3&&heldItem=="ax")
                        {
                            QVector<DialogLine> lines = {
                                {"旁白","这棵树太硬了，你拼尽全力无法战胜"},
                                {"维克多","看来只能寻求别人的帮助了"},
                            };
                            startDialog(lines);
                        }
                        else if(tutorialStep>=4){
                            obj->hide();
                            QVector<DialogLine> lines = {
                                {"旁白","只见伐木工手起刀落\n一下便锯开了硬木"},
                                {"伐木工","简单。"},
                                {"维克多","强？！强！？"},
                            };
                            startDialog(lines);
                            tutorialStep=5;
                        }
                        else{
                            showDialog("你没有合适的工具","旁白");
                        }
                    }
                }
                if (obj->type=="NPC")
                {
                    NPC* npc = dynamic_cast<NPC*>(obj);
                    if (playerRect.intersects(Rect))
                    {
                        if(npc->num==1)
                        {
                           if(iftalkwithfishman==0&&tutorialStep>=0){
                                QVector<DialogLine> lines = {
                                    {"维克托","老大爷，您这卖鱼吗"},
                                    {"渔夫","不好意思啊，小伙子，我今天这里鱼卖完了"},
                                    {"维克托","啊，那我现在去哪里能弄到鱼呢"},
                                    {"渔夫","这样吧，小伙子，我教你怎么捕鱼，剩下的就要看你自己的喽"},
                                    {"维克托","好的"},
                                    {"渔夫","如果你有钓竿那自然是不用说，如果你嫌麻烦，用渔网捕鱼也不是不行"},
                                    {"维克托","非常感谢您"},
                                    {"旁白","现在，你可以合成一个渔网(trap)去捕鱼了"}
                                };

                                startDialog(lines);
                                iftalkwithfishman=1;
                            }
                           else{
                               showDialog("这是一个渔夫","旁白");
                           }
                        }
                        if(npc->num==2)
                        {
                            if(iftalkwithlumberjack==0){
                                QVector<DialogLine> lines = {
                                    {"维克托","先生，请问您能帮我砍倒这棵硬木吗"},
                                    {"伐木工","我可以帮忙，但是我现在很饿"},
                                    {"伐木工","我想吃鱼，你去帮我弄条鱼来我就帮你砍树"}
                                };

                                startDialog(lines);
                                iftalkwithlumberjack=1;
                            }
                            else if(tutorialStep<=4)
                            {
                                if(heldItem!="fish")
                                {
                                    showDialog("去帮我弄条鱼吧","伐木工");
                                }
                                else
                                {
                                    showDialog("这鱼味道真不错,好的，我们来砍树吧","伐木工");
                                    tutorialStep=4;
                                }
                            }
                            else if(tutorialStep==5)
                            {
                                showDialog("感谢你的鱼\n一路顺风！","伐木工");
                            }
                        }
                        if(npc->num==3)
                        {
                            if(tutorialStep==-1)
                            {
                                if(heldItem=="flower")
                                {
                                    QVector<DialogLine> lines = {
                                        {"维克多", "你好，请问这些花可以帮到你吗？"},
                                        {"养蜂人", "（难以置信地）\n你~你是从哪里搞到他们的，这可帮了大忙了"},
                                        {"旁白", "一阵等待后"},
                                        {"养蜂人", "太好了\n过了这么久，我终于见到蜂蜜（honey）了\n如果你需要，可以带个罐子（jar）来找我"}
                                    };
                                    startDialog(lines);
                                    tutorialStep++;
                                    break;
                                }
                                else
                                {

                                    QVector<DialogLine> lines = {
                                        {"养蜂人", "自从“字母失踪”以来\n花园里的花都枯萎了"},
                                        {"养蜂人", "没有了花（flower）\n就没办法酿出蜂蜜（honey）了啊"},
                                        {"维克多", "要合成flower，我似乎只差字母w了\n也许我该去左边的洞穴里碰碰运气"}
                                    };
                                    startDialog(lines);
                                    break;
                                }
                            }
                            else if(tutorialStep==0)
                            {
                                if(heldItem=="jar")
                                {
                                    QVector<DialogLine> lines = {
                                        {"养蜂人", "感谢你的蜂蜜，这是你应得的报酬"},
                                        {"旁白", "解锁了新物品：honey"}
                                    };
                                    startDialog(lines);
                                    tutorialStep++;
                                    encyclopedia["honey"] = encyclopedia.size();
                                    inventory.append("Y");
                                    inventory.append("N");
                                    break;
                                }
                                else
                                {
                                    QVector<DialogLine> lines = {
                                        {"养蜂人", "蜂蜜已经做好了，你需要的话可以拿着jar来找我"}
                                    };
                                    startDialog(lines);
                                    break;
                                }
                            }
                            else
                            {
                                QVector<DialogLine> lines = {{"旁白", "养蜂人正与他的蜜蜂们愉快的玩耍"}};
                                startDialog(lines);
                            }
                        }
                        if(npc->num==4)
                        {
                            if(tutorialStep==-1)
                            {   QVector<DialogLine> lines = {
                                    {"女王", "我的勇士，你终于来了，看起来你已经收集了不少字母了"},
                                    {"维克多", "那剩下的字母，你有什么头绪吗"},
                                    {"旁白", "女王陷入了沉思"},
                                    {"女王", "我现在能想到的，也只有前方的矿坑了"},
                                    {"女王", "那个废弃的矿坑里应该还有一些金子（gold）\n能给你缺少的G和D两个字母"},
                                    {"维克多","好，我这就去试试"},
                                    {"女王", "千万小心\n矿坑里不但极度漆黑，还有危险的生物僵尸（zombie）\n一定要和它保持距离！"}
                                };
                                startDialog(lines);
                                tutorialStep=0;
                                break;
                            }
                            else if(!inventory.contains('G'))
                            {
                                showDialog("去矿坑里找gold吧","旁白");
                            }
                            else if(!inventory.contains('Z'))
                            {
                                showDialog("看起来你还没有收集到字母Z\n你可以试着用武器击杀它们","女王");
                            }
                            else if(tutorialStep==2)
                            {
                                QVector<DialogLine> lines = {
                                    {"维克多", "还有Q和V两个字母没有找到"},
                                    {"女王", "这本来也不是常见的字母……\n找不到倒也是情理之中……"},
                                    {"旁白", "开动脑筋，你真的没有见过Q和V这两个字母吗"},
                                    {"旁白","没有头绪的话，你可以看看矿坑里某位前辈留下的日记\n或者在背包界面获取提示"},
                                    {"旁白","总之，你想到两个字母对应的单词以后\n记得再次和女王交流，告诉她你的答案哦"},
                                };
                                startDialog(lines);
                                tutorialStep=3;
                                break;
                            }
                            else if(tutorialStep==3)
                            {
                                QString ans1=getYourname(1).toUpper();
                                QString ans2=getYourname(2).toUpper();
                                if(ans1=="QUEEN"&&ans2=="VICTOR")
                                {
                                    inventory.append("Q");
                                    inventory.append("V");
                                    heldItemLabel->hide();
                                    cgWidget->show();
                                    QVector<DialogLine> lines = {
                                        {"旁白", "……"},
                                        {"旁白", "多年后，字母王国主城的某个街头"},
                                         {"说书人", "……只见得那维克多勇士微微一笑\n道出了女王和它的名字\n竟然刚好有Q和V两个字母！"},
                                         {"说书人", "……自此，26个字母悉数归位\n又在二人不懈的努力下\n字母王国终于回归了安宁"},
                                         {"小孩甲", "不就是名字里有这两个字母嘛\n维克多勇士具体又是怎么得到两个字母的呢\n你这说的不完整！"},
                                         {"说书人","呃呃……这个小孩子不能听啊，不能听啊"},
                                         {"小孩乙","而且那个死在矿坑里的勇士不是也叫Victor吗\n这也太巧合了吧"},
                                         {"男子","这个我知道\n因为victor是胜利者的意思\n大家都喜欢叫这个名字"},
                                         {"女子","也说不定这就是女王和victor的缘分吧"},
                                         {"小孩丙","我还有问题，我还有问题"},
                                         {"旁白", "转眼间，说书摊前便被小孩子围得水泄不通"},
                                         {"旁白", "男子和女子相视一笑\n夕阳下二人的影子越拉越长\n直至消失……"},
                                         {"旁白", "恭喜通关本作！感谢您的游玩！\n再次点击鼠标可以回到游戏开始界面"},

                                         };
                                    startDialog(lines);
                                    tutorialStep=4;
                                    break;
                                }
                                else
                                {
                                    showDialog("我不是很理解这两个单词的意思……","女王");
                                }
                            }
                        }
                    }
                }
            }
        }
        break;
        }
    case Qt::Key_B: {
        bagOpen = !bagOpen;
        if (bagOpen) {            
            uiLayer->show();
            uiLayer->raise();
            pauseGame();
            updateBagUI();
            break;
        }
        break;
    }
    case Qt::Key_Escape: {
        if (bagOpen) {           
            bagOpen = false;
            resumeGame();
            updateHeldItemUI();            
            uiLayer->hide();
            this->setFocus();
        }
        break;
    }
    }
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (dialogBox->isVisible()) {
        if (textTimer->isActive()) {
            textTimer->stop();
            dialogText->setText(fullText);

        } else {
            dialogDelay = 0;
            showNextDialog();

        }
        return;
    }
    else
    {
        if(currentLevel==4&&tutorialStep==4&&!uiLayer->isVisible()&&!pauseMenu->isVisible())
        {
            cgWidget->hide();
            pauseMenu->hide();
            menuBtn->hide();
            heldItemLabel->hide();
            clearGame();
            startWidget->show();
        }
    }
    QMainWindow::mousePressEvent(event);
}
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_W: keyW = false; break;
    case Qt::Key_A: keyA = false; break;
    case Qt::Key_S: keyS = false; break;
    case Qt::Key_D: keyD = false; break;
    case Qt::Key_E: keyE = false; break;
    }
}
void MainWindow::pauseGame()
{
    gamePaused = true;

    if (timer) timer->stop();
    if (rockSpawnTimer) rockSpawnTimer->stop();
    for(QTimer* i:warningTimer)
    {
        if(i)
        {
            i->stop();
        }
    }
}
void MainWindow::clearGame()
{
    for (GameObject *obj : objects)
        obj->deleteLater();
    objects.clear();
    checkpoints.clear();
    if(player)
    {player->deleteLater();}
    if(fish)
    {fish->deleteLater();}
    if(heldItemLabel)
    {heldItemLabel->deleteLater();}
    if(timer)
    timer->deleteLater();
    if(rockSpawnTimer)
    {
        rockSpawnTimer->stop();
        rockSpawnTimer->deleteLater();
    }
    for (QTimer* t : warningTimer)
    {
        if (t)
        {
            t->stop();
            t->disconnect();
            warningTimer.removeAll(t);
            t->deleteLater();
        }
    }
    obj2.clear();
    obj21.clear();
    obj22.clear();
    obj3.clear();
    obj31.clear();
    obj4.clear();
    obj41.clear();
    if(torchlight)
    {torchlight->hide();}
    rodCasting=0;
    fishCaught=0;
    fishgame=0;
    fishingWidget->update();
    wormgame=0;
    zombiegame=0;
    currentMap=0;
}
void MainWindow::resumeGame()
{
    gamePaused = false;
    if (timer) timer->start(16);
    if (rockSpawnTimer) rockSpawnTimer->start();
    for(QTimer* i:warningTimer)
    {
        if(i)
        {
            i->start();
        }
    }
}
void MainWindow::updateHeldItemUI()
{
    if (heldItem.isEmpty()) {
        heldItemLabel->setText("手持：空手");
    } else {
        heldItemLabel->setText("手持：" + heldItem);
    }
    heldItemLabel->raise();
    if(!(currentLevel==4&&tutorialStep==4))
    heldItemLabel->show();
}
void MainWindow::loadLevel(int level)
{
    currentLevel=level;
    if(level>currentSave.level)
    {
        currentSave.number=0;
        currentSave.tutorialStep=-1;
        tutorialStep=-1;
    }
    gamePaused = false;

    // 1.清场
    player=nullptr;
    heldItemLabel=nullptr;
    timer=nullptr;
    rockSpawnTimer=nullptr;
    fish=nullptr;
    clearGame();
    if(level==1)
    {
        inventory.clear();
        inventory.append("A");
        inventory.append("E");
        inventory.append("I");
        inventory.append("O");
        inventory.append("U");
        encyclopedia.clear();
    }
    heldItem = "";
    // 3. 手持显示
    heldItemLabel=new QLabel(this);


    heldItemLabel->setGeometry(10, 10, 200, 30); // 左上角
    heldItemLabel->setStyleSheet(
        "color: white;"
        "font-size: 18px;"
        "background-color: rgba(0, 0, 0, 150);"
        "padding: 5px;"
        );
    heldItemLabel->setText("手持：空手");

    // 4.加载地图（复用你原来的逻辑）
    QFile file(QString(":/level/level%1.txt").arg(level));
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.isEmpty()) {
                continue;
            }
            if (line.startsWith("#")) continue; // 跳过注释
            QStringList parts = line.split(" ");
            QString type = parts[0];
            int x = parts[1].toInt();
            int y = parts[2].toInt();
            int w = parts[3].toInt();
            int h = parts[4].toInt();
            int num=parts[5].toInt();
            if(type=="player")
            {
                player = new Player(centralWidget());
                player->setGeometry(x, y, w, h);
                player->show();
            }
            GameObject *obj = nullptr;
            if(num>currentSave.number)
            {
                if (type == "wall") {
                    obj = new GameObject(centralWidget());
                    obj->setStyleSheet("background-color: gray;");
                }
                else if (type == "tree") {
                    Tree *tree = new Tree(centralWidget()); // 使用 Tree 类
                    obj = tree;                   // 基类指针指向派生类
                }
                else if (type == "stone") {
                    Stone *stone = new Stone(centralWidget()); // 使用 Tree 类
                    obj = stone;                   // 基类指针指向派生类
                }
                else if (type == "door") {
                    Door *door = new Door(centralWidget()); // 使用 Door 类
                    obj = door;                   // 基类指针指向派生类

                }
                else if (type == "box") {
                    Box *box = new Box(centralWidget()); // 使用 Door 类
                    obj = box;                   // 基类指针指向派生类
                }
                else if (type == "tpdoor") {
                    Tpdoor *tpdoor = new Tpdoor(centralWidget()); // 使用 Tpdoor 类
                    obj = tpdoor;                   // 基类指针指向派生类
                }
                else if (type == "lake") {
                    Lake *lake = new Lake(centralWidget()); // 使用 Tpdoor 类
                    obj = lake;                   // 基类指针指向派生类
                }
                else if (type == "worm") {
                    Worm *worm = new Worm(centralWidget()); // 使用 Tpdoor 类
                    obj = worm;                   // 基类指针指向派生类
                }
                else if (type == "NPC") {
                    NPC *npc = new NPC(centralWidget());
                    npc->num = parts[6].toInt();
                    npc->setNPCImage(npc->num);  // ← 添加这一行
                    obj = npc;
                }
                else if (type == "jar") {
                    Jar *jar = new Jar(centralWidget());
                    obj = jar;                   // 基类指针指向派生类
                }
                else if (type == "gold"&&!encyclopedia.contains("gold")) {
                    Gold *gold = new Gold(centralWidget());
                    obj = gold;                   // 基类指针指向派生类
                }
                else if (type == "book") {
                    Book *book = new Book(centralWidget());
                    obj = book;
                    book->num=parts[6].toInt();
                }
                else if (type == "hardwood") {
                    Hardwood *hardwood = new Hardwood(centralWidget()); // 使用 Tpdoor 类
                    obj = hardwood;                   // 基类指针指向派生类
                }
                else if (type == "zombie") {
                    Zombie *zombie = new Zombie(centralWidget());
                    obj = zombie;
                    zombie->val.setX(parts[6].toInt());
                    zombie->val.setY(parts[7].toInt());
                    zombie->start.setX(parts[1].toInt());
                    zombie->start.setY(parts[2].toInt());
                    zombie->end.setX(parts[8].toInt());
                    zombie->end.setY(parts[9].toInt());
                }
            }
            if(type=="checkpoint")
            {
                obj = new GameObject(centralWidget());
                obj->setStyleSheet("background-color: green; border-radius: 20px;");
                obj->type="checkpoint";
                obj->stackUnder(player);
                checkpoints.append(obj);
            }

            if (obj) {
                obj->setGeometry(x, y, w, h);
                if(obj->type=="door")
                {
                    obj->setImage(":/images/door.png");
                }
                obj->number=num;
                obj->show();                
                objects.append(obj); // 加入列表
                if(level==2)
                {
                    if(num==1)
                    {
                        obj2.append(obj);

                    }
                    else if(num==2)
                    {
                        obj21.append(obj);
                        obj->hide();
                    }
                    else if(num==3)
                    {
                        obj22.append(obj);
                        obj->hide();
                    }
                }
                if(level==3)
                {
                    if(num==1)
                    {
                        obj3.append(obj);

                    }
                    else if(num==2)
                    {
                        obj31.append(obj);
                        obj->hide();
                    }
                }
                if(level==4)
                {

                    if(num==1)
                    {
                        obj4.append(obj);

                    }
                    else if(num==2)
                    {
                        obj41.append(obj);
                        obj->hide();

                    }
                }
            }

        }

        file.close();
    }

    initGame();


    updateBagUI();

    updateHeldItemUI();

    if(currentSave.level<level )
    {
        currentSave.level = level;
        currentSave.lastCheckpoint = player->pos();
        currentSave.inventory = inventory;
        currentSave.encyclopedia = encyclopedia;
    }
    /*if(level==3)
    {
        wormgame=1;
        torchlight = new GameObject(centralWidget());
        shootTorchlight();
    }*/
    /*if(level==4)
    {
        zombiegame=1;
        torchlight = new GameObject(centralWidget());
        shootTorchlight2();
    }*/

}
void MainWindow::initStartMenu()
{
    // ===== 开始界面 =====

    startWidget->setGeometry(0, 0, 800, 600);
    startWidget->setStyleSheet("background-color: #1e1e1e;");

    QVBoxLayout *layout = new QVBoxLayout(startWidget);
    layout->setSpacing(30);
    layout->setContentsMargins(0, 150, 0, 0);

    // 标题
    QLabel *title = new QLabel("LetterWorld");
    title->setStyleSheet(
        "color: white;"
        "font-size: 48px;"
        "font-weight: bold;"
        );
    title->setAlignment(Qt::AlignCenter);

    // 开始新游戏
    QPushButton *btnNewGame = new QPushButton("开始新游戏");
    btnNewGame->setFixedSize(260, 60);
    btnNewGame->setStyleSheet(
        "font-size: 24px;"
        "color: white;"
        "background-color: #444;"
        "border-radius: 8px;"
        );

    // 继续游戏
    QPushButton *btnContinue = new QPushButton("继续游戏");
    btnContinue->setFixedSize(260, 60);   
    btnContinue->setStyleSheet(
        "font-size: 24px;"
        "color: gray;"
        "background-color: #333;"
        "border-radius: 8px;"
        );

    // 退出游戏
    QPushButton *btnExit = new QPushButton("退出游戏");
    btnExit->setFixedSize(260, 60);
    btnExit->setStyleSheet(
        "font-size: 24px;"
        "color: white;"
        "background-color: #662222;"
        "border-radius: 8px;"
        );

    // 加入布局
    layout->addWidget(title);
    layout->addSpacing(40);
    layout->addWidget(btnNewGame, 0, Qt::AlignCenter);
    layout->addWidget(btnContinue, 0, Qt::AlignCenter);
    layout->addWidget(btnExit, 0, Qt::AlignCenter);
    layout->addStretch();
    connect(btnNewGame, &QPushButton::clicked, this, [=](){
        startWidget->hide();
        uiLayer->hide();         // 隐藏开始界面
        this->centralWidget()->show(); // 显示游戏      
        this->setFocus();
        currentSave=SaveData();
        tutorialStep=-1;
        iftalkwithfishman=0;
        iftalkwithlumberjack=0;
        loadLevel(1);             // ✅ 加载第一关
    });
    connect(btnContinue, &QPushButton::clicked, this, [=](){
        startWidget->hide();
        this->centralWidget()->show();
        loadGame(currentSave);
    });
    connect(btnExit, &QPushButton::clicked, this, [=](){        
        QApplication::quit();
    });
}
void MainWindow::initBagUI()
{
    bagOpen = false;
    bagWidget = new QWidget(uiLayer);
    bagWidget->setGeometry(50, 50, 700, 450);
    bagWidget->setStyleSheet(
        "QWidget { background-color: rgba(0, 0, 0, 180); color: white; }" // <-- 全局字体变白
        "QGroupBox { color: white; border: 1px solid white; margin-top: 10px; }"
        );
    tipBtn = new QPushButton("?", uiLayer);
    tipBtn->raise();
    tipBtn->show();
    tipBtn->setFixedSize(50, 50);
    tipBtn->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: white;"
        "background-color: rgb(0, 0, 0);"
        "border-radius: 25px;"
        );
    tipBtn->move(this->width() - 60, 10);

    QHBoxLayout *mainLayout = new QHBoxLayout(bagWidget);

    /* ===== 左侧 ===== */
    QWidget *left = new QWidget;
    QVBoxLayout *leftLayout = new QVBoxLayout(left);

    /* 左上：26 字母 */
    QGroupBox *letterBox = new QGroupBox("字母收集");

    letterGrid = new QGridLayout;

    for (int i = 0; i < 26; i++) {
        QLabel *lbl = new QLabel(QString(char('A' + i)));
        lbl->setFixedSize(36, 36);
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setStyleSheet(
            "background-color: gray;"
            "color: white;"
            );
        letterGrid->addWidget(lbl, i / 13, i % 13);
    }
    letterBox->setLayout(letterGrid);
    leftLayout->addWidget(letterBox);

    /* 左下：输入区（占位） */
    QGroupBox *inputBox = new QGroupBox("合成");
    QHBoxLayout *inputLayout = new QHBoxLayout;
    QLineEdit *inputEdit = new QLineEdit;
    inputEdit->setStyleSheet("color: white; background-color: #444;");

    QPushButton *craftBtn = new QPushButton("合成");
    craftBtn->setStyleSheet("color: white; background-color: #555;");

    inputLayout->addWidget(inputEdit);
    inputLayout->addWidget(craftBtn);

    inputBox->setLayout(inputLayout);
    leftLayout->addWidget(inputBox);

    /* ===== 右侧：图鉴 ===== */
    QGroupBox *rightBox = new QGroupBox("图鉴");
    //rightBox->setMaximumWidth(200);
    rightBox->setFixedSize(170, 420);
    // ✅ 右侧总布局（内容 + 按钮）
    QVBoxLayout *rightTotalLayout = new QVBoxLayout(rightBox);

    // ✅ 图鉴内容区（只放图鉴）
    rightLayout = new QVBoxLayout;
    rightTotalLayout->addLayout(rightLayout); // 图鉴内容在上面

    // ✅ 弹簧：把按钮压下去
    rightTotalLayout->addStretch();

    // ✅ 按钮区（固定在底部）
    QHBoxLayout *pageBtnLayout = new QHBoxLayout;
    prevPageBtn = new QPushButton("< 上一页");
    nextPageBtn = new QPushButton("下一页 >");

    pageBtnLayout->addWidget(prevPageBtn);
    pageBtnLayout->addWidget(nextPageBtn);

    // ✅ 把按钮区加到最下面
    rightTotalLayout->addLayout(pageBtnLayout);

    /* 加入主布局 */
    mainLayout->addWidget(left, 2);
    mainLayout->addWidget(rightBox, 1);
    connect(craftBtn, &QPushButton::clicked, this, [=](){
        if (dialogBox->isVisible())
            return; // 对话框开着不能操作
        QString text = inputEdit->text().toUpper();
        if (text == "AX")
        {
            if (encyclopedia.contains("ax")) {
                showDialog("你已经拥有斧子了！");
                return;
            }
            if (inventory.contains("A") && inventory.contains("X")) {
                heldItem = "ax";
                updateHeldItemUI();
                if (!encyclopedia.contains("ax")) {
                    encyclopedia["ax"] = encyclopedia.size();
                    updateBagUI();
                }
                inputEdit->clear();
                showDialog("很好。\n现在你可以用斧子砍树了。","旁白");
            } else {
                showDialog("材料不足。");
            }
        }
        else if (text == "BAT")
        {
            if (encyclopedia.contains("bat")) {
                showDialog("你已经拥有棒子了！");
                return;
            }
            if (inventory.contains("B") && inventory.contains("A")&& inventory.contains("T")) {
                heldItem = "bat";
                updateHeldItemUI();
                if (!encyclopedia.contains("bat")) {
                    encyclopedia["bat"] = encyclopedia.size();
                    updateBagUI();
                }
                inputEdit->clear();
                showDialog("已合成棒子。","旁白");
            } else {
                showDialog("材料不足。");
            }
        }
        else if (text == "TRAP")
        {
            if (encyclopedia.contains("trap")) {
                showDialog("你已经拥有渔网了！");
                return;
            }
            if (inventory.contains("T")&& inventory.contains("R")&& inventory.contains("A")&& inventory.contains("P")) {
                heldItem = "trap";
                updateHeldItemUI();
                if (!encyclopedia.contains("trap")) {
                    encyclopedia["trap"] = encyclopedia.size();
                    updateBagUI();
                }
                inputEdit->clear();
                showDialog("已合成网。","旁白");
            } else {
                showDialog("材料不足。");
            }
        }
        else if (text == "TORCH")
        {
            if (encyclopedia.contains("torch")) {
                showDialog("你已经拥有手电筒了！");
                return;
            }
            if (inventory.contains("T") && inventory.contains("O")&& inventory.contains("R")&& inventory.contains("C")&& inventory.contains("H")) {
                heldItem = "torch";
                updateHeldItemUI();
                if (!encyclopedia.contains("torch")) {
                    encyclopedia["torch"] = encyclopedia.size();
                    updateBagUI();
                }
                inputEdit->clear();
                showDialog("已合成手电筒。","旁白");
            } else {
                showDialog("材料不足。");
            }
        }
        else if (text == "FLOWER")
        {
            if (encyclopedia.contains("flower")) {
                showDialog("你已经拥有花了！");
                return;
            }
            if (inventory.contains("F") && inventory.contains("L")&& inventory.contains("O")&& inventory.contains("W")&& inventory.contains("E")&& inventory.contains("R")) {
                heldItem = "flower";
                updateHeldItemUI();
                if (!encyclopedia.contains("flower")) {
                    encyclopedia["flower"] = encyclopedia.size();
                    updateBagUI();
                }
                inputEdit->clear();
                showDialog("已合成花。","旁白");
            } else {
                showDialog("材料不足。");
            }
        }
        else if (text == "KEY")
        {
            if (encyclopedia.contains("key")) {
                showDialog("你已经拥有钥匙了！");
                return;
            }
            if (inventory.contains("K") && inventory.contains("E")&& inventory.contains("Y")) {
                heldItem = "key";
                updateHeldItemUI();
                if (!encyclopedia.contains("key")) {
                    encyclopedia["key"] = encyclopedia.size();
                    updateBagUI();
                }
                inputEdit->clear();
                showDialog("已合成钥匙。","旁白");
            } else {
                showDialog("材料不足。");
            }
        }
        else if (text == "GUN")
        {
            if (encyclopedia.contains("gun")) {
                showDialog("你已经拥有钥匙了！");
                return;
            }
            if (inventory.contains("G") && inventory.contains("U")&& inventory.contains("N")) {
                heldItem = "gun";
                updateHeldItemUI();
                if (!encyclopedia.contains("gun")) {
                    encyclopedia["gun"] = encyclopedia.size();
                    updateBagUI();
                }
                inputEdit->clear();
                showDialog("已合成枪，自带光束瞄准，按E键即可发射。");
            } else {
                showDialog("材料不足。");
            }
        }
        else
        {
            showDialog("配方错误！");
        }
    });
    connect(tipBtn, &QPushButton::clicked, this, [=](){
        gameTips();
    });
    connect(prevPageBtn, &QPushButton::clicked, this, [=]() {
        if (currentPage > 0) {
            currentPage--;
            updateBagUI();
        }
    });
    connect(nextPageBtn, &QPushButton::clicked, this, [=]() {
        int maxPage =
            (encyclopedia.size() - 1) / PageSize;
        if (currentPage < maxPage) {
            currentPage++;
            updateBagUI();
        }
    });
}
void MainWindow::initGameOverUI()
{
    // ===== GameOver 面板 =====

    gameOverPanel->setGeometry(200, 100, 400, 260);
    gameOverPanel->setStyleSheet(
        "background-color: rgba(0, 0, 0, 230);"
        "border: 2px solid white;"
        "border-radius: 10px;"
        );
    gameOverPanel->hide();
    gameOverPanel->raise();

    QVBoxLayout *layout = new QVBoxLayout(gameOverPanel);
    layout->setSpacing(30);

    // 死因
    deathReasonLabel = new QLabel("死于：未知");
    deathReasonLabel->setAlignment(Qt::AlignCenter);
    deathReasonLabel->setStyleSheet(
        "color: white;"
        "font-size: 26px;"
        );

    // 从上次存档点开始
    QPushButton *btnRestart = new QPushButton("从上次存档点开始");
    btnRestart->setFixedHeight(50);
    btnRestart->setStyleSheet(
        "font-size: 20px;"
        "color: white;"
        "background-color: #444;"
        );

    // 回到主菜单
    QPushButton *btnMainMenu = new QPushButton("回到主菜单");
    btnMainMenu->setFixedHeight(50);
    btnMainMenu->setStyleSheet(
        "font-size: 20px;"
        "color: white;"
        "background-color: #444;"
        );

    layout->addStretch();
    layout->addWidget(deathReasonLabel);
    layout->addSpacing(20);
    layout->addWidget(btnRestart);
    layout->addSpacing(10);
    layout->addWidget(btnMainMenu);
    layout->addStretch();

    // ===== 信号连接 =====
    connect(btnRestart, &QPushButton::clicked, this, [=]() {
        gameOverPanel->hide();
        clearGame();
        loadGame(currentSave);
    });

    connect(btnMainMenu, &QPushButton::clicked, this, [=]() {
        gameOverPanel->hide();
        menuBtn->hide();
        heldItemLabel->hide();

        clearGame();
        startWidget->show();
    });
}
void MainWindow::initGameMenu()
{
    // ===== 右上角菜单按钮 =====
    menuBtn = new QPushButton("☰", fishingWidget);
    menuBtn->raise();
    menuBtn->show();
    menuBtn->setFixedSize(50, 50);
    menuBtn->setStyleSheet(
        "font-size: 24px;"
        "color: white;"
        "background-color: rgba(0, 0, 0, 150);"
        "border-radius: 25px;"
        );
    menuBtn->move(this->width() - 60, 10);
    // ===== 暂停菜单 =====

    pauseMenu->setGeometry(200, 100, 400, 300);
    pauseMenu->setStyleSheet(
        "background-color: rgba(0, 0, 0, 220);"
        "border: 2px solid white;"
        "border-radius: 10px;"
        );
    pauseMenu->hide();

    QVBoxLayout *pauseLayout = new QVBoxLayout(pauseMenu);
    pauseLayout->setSpacing(30);

    QLabel *title = new QLabel("游戏菜单");
    title->setStyleSheet("color: white; font-size: 32px;");
    title->setAlignment(Qt::AlignCenter);

    QPushButton *btnRestart = new QPushButton("回到上个存档点");
    btnRestart->setFixedHeight(50);
    btnRestart->setStyleSheet(
        "font-size: 20px;"
        "color: white;"
        "background-color: #444;"
        );

    QPushButton *btnMainMenu = new QPushButton("回到主菜单");
    btnMainMenu->setFixedHeight(50);
    btnMainMenu->setStyleSheet(
        "font-size: 20px;"
        "color: white;"
        "background-color: #444;"
        );
    QPushButton *btnResume = new QPushButton("回到游戏");
    btnResume->setFixedHeight(50);
    btnResume->setStyleSheet(
        "font-size: 20px;"
        "color: white;"
        "background-color: #555;"
        );
    pauseLayout->addWidget(title);
    pauseLayout->addWidget(btnResume);
    pauseLayout->addWidget(btnRestart);
    pauseLayout->addWidget(btnMainMenu);
    pauseLayout->addStretch();
    connect(menuBtn, &QPushButton::clicked, this, [=](){
        if(!gameOverPanel->isVisible()&&!dialogBox->isVisible())
        {
        pauseMenu->show();
        pauseMenu->raise();
        pauseGame();  // ✅ 暂停游戏
        }
    });
    connect(btnResume, &QPushButton::clicked, this, [=](){
       pauseMenu->hide();     // ✅ 关闭菜单
        resumeGame();    // ✅ 恢复游戏
    });
    connect(btnMainMenu, &QPushButton::clicked, this, [=](){
        pauseMenu->hide();
        menuBtn->hide();
        heldItemLabel->hide();
        clearGame();   
        startWidget->show();
    });
    connect(btnRestart, &QPushButton::clicked, this, [=](){
        pauseMenu->hide();
        clearGame();
        loadGame(currentSave);
    });
}
void MainWindow::initGame()
{
    timer = new QTimer(this);
    timer->start(16);
    connect(timer, &QTimer::timeout, this, &MainWindow::gameLoop);
    initGameMenu();
}
void MainWindow::initDialog()
{
    speakerColors["维克多"]   = "#3498DB"; // 蓝色
    speakerColors["男人"]   = "#3498DB";
    speakerColors["旁白"] = "#888888"; // 灰色
    speakerColors["女王"] = "#FF0000"; // 红色
    speakerColors["女人"] = "#FF0000"; // 红色
    speakerColors["养蜂人"] = "#FFD700"; // 灰色
    speakerColors["Tips"] = "#C0C0C0"; // 灰色
    // ===== 对话框主体 =====
    dialogBox = new QWidget(this);
    dialogBox->setGeometry(0, 420, 800, 180);
    dialogBox->setStyleSheet(
        "background-color: rgba(0, 0, 0, 220);"
        "border-top: 2px solid white;"
        );
    dialogBox->hide();

    // ===== 垂直布局 =====
    QVBoxLayout *dlgLayout = new QVBoxLayout(dialogBox);
    dlgLayout->setContentsMargins(15, 10, 15, 10);
    dlgLayout->setSpacing(8);

    // ===== 说话人名字 =====
    dialogName = new QLabel("无名氏");
    dialogName->setFixedHeight(36);
    dialogName->setStyleSheet(
        "color: #FFD700;"      // 金色（GalGame 味）
        "font-size: 22px;"
        "font-weight: bold;"
        "padding-left: 5px;"
        );
    dialogName->setAlignment(Qt::AlignLeft);

    // ===== 对话正文 =====
    dialogText = new QLabel;
    dialogText->setMinimumHeight(108);
    dialogText->setStyleSheet(
        "color: white;"
        "font-size: 22px;"
        "padding: 5px;"
        );
    dialogText->setWordWrap(true);

    dlgLayout->addWidget(dialogName);
    dlgLayout->addWidget(dialogText);
    dlgLayout->setStretchFactor(dialogName, 1);
    dlgLayout->setStretchFactor(dialogText, 3);

    // ===== 打字机效果 =====
    textTimer = new QTimer(this);
    connect(textTimer, &QTimer::timeout, this, [=](){
        if (textIndex < fullText.length()) {
            dialogText->setText(fullText.left(++textIndex));
        } else {
            textTimer->stop();
        }
    });
}
void MainWindow::gameLoop()
{
    if (gamePaused) return;
    if(fishgame)
    {
        onTick();
        return;
    }
    if(rockgame)
    {
        for(GameObject* obj:objects)
        {
            if(obj->type=="rock")
            {
                Rock* rock = dynamic_cast<Rock*>(obj);
                if(!rock->isShadow)
                {
                    QRect playerRect(player->geometry());
                    QRect rockRect(rock->geometry());
                    if(playerRect.intersects(rockRect))
                    {
                        showGameOver("您被落石砸死了");
                        break;
                    }
                }
            }
        }
    }
    int speed =5; // 移动速度
    int x = player->x();
    int y = player->y();
    if(keyW)
    {
        y -= speed;
        face=0;
        if(player) player->setFacing(0);
        if(wormgame)
       shootTorchlight();
        /*if(zombiegame)
        {
            shootTorchlight2();
        }*/
    }
    if(keyS)
    {
        y += speed;
        face=1;
        if(player) player->setFacing(1);
        if(wormgame)
        shootTorchlight();
        /*if(zombiegame)
        {
            shootTorchlight2();
        }*/
    }
    if(keyA)
    {
        x -= speed;
        face=2;
        if(player) player->setFacing(2);
        if(wormgame)
            shootTorchlight();
        /*if(zombiegame)
        {
            shootTorchlight2();
        }*/
    }
    if(keyD)
    {
        x += speed;
        face=3;
        if(player) player->setFacing(3);
        if(wormgame)
            shootTorchlight();
        /*if(zombiegame)
        {
            shootTorchlight2();
        }*/
    }

    // ===== 碰撞检测 =====
    // 1. 获取玩家和墙的矩形区域
    QRect playerRect(x, y, player->width(), player->height());
    if(!wormgame&&!zombiegame)
    {
        for(GameObject* wall:objects)
        {
            if(!wall->isVisible())continue;
            //if(wall->type=="checkpoint")continue;
            if(wall->type=="rock")
            {
                Rock* rock = dynamic_cast<Rock*>(wall);
                if(rock->isShadow)continue;
            }
            QRect wallRect(wall->x(), wall->y(), wall->width(), wall->height());
            if (playerRect.intersects(wallRect)) {
                if(wall->type=="checkpoint")
                {
                    QPoint center = wallRect.center()+QPoint(-20,-20);
                    if (currentSave.number < wall->number) {
                        currentSave.lastCheckpoint = center;
                        currentSave.level=currentLevel;
                        currentSave.encyclopedia=encyclopedia;
                        currentSave.inventory=inventory;
                        currentSave.tutorialStep=tutorialStep;
                        currentSave.number = wall->number;
                    }
                }
                else if(wall->type=="tpdoor")
                {
                    if(currentLevel==2)
                    {
                        if(currentMap==0)
                        {
                            if(player->pos().x()<100)
                            {
                                currentMap=1;
                                fromAtoB(2,21);
                                return;

                            }
                            else if(player->pos().x()>600)
                            {
                                currentMap=2;
                                fromAtoB(2,22);
                                return;
                            }
                        }
                        else if(currentMap==1)
                        {
                            currentMap=0;
                            fromAtoB(21,2);
                            return;
                        }
                        else if(currentMap==2)
                        {
                            currentMap=0;
                            fromAtoB(22,2);
                            return;
                        }
                    }
                    else if(currentLevel==3)
                    {
                        if(currentMap==0)
                        {
                            currentMap=1;
                            fromAtoB(3,31);
                            return;
                        }
                        else if(currentMap==1)
                        {
                            currentMap=0;
                            fromAtoB(31,3);
                            return;
                        }
                    }
                    else if(currentLevel==4)
                    {
                        if(currentMap==0)
                        {
                            currentMap=1;
                            fromAtoB(4,41);
                            return;
                        }
                        else if(currentMap==1)
                        {
                            currentMap=0;
                            fromAtoB(41,4);
                            return;
                        }
                    }
                }
                else
                {
                    return; // 停止移动
                }
            }
        }
    }
    else if(wormgame)
    {
        QRect newplayerRect(x,y,player->width(),player->height());
        const int interactRange = 5;
        newplayerRect.adjust(-interactRange, -interactRange,
                          interactRange,  interactRange);
        for(GameObject* wall:objects)
        {
            if(wall->number!=2)continue;

            QRect wallRect(wall->x(), wall->y(), wall->width(), wall->height());
            if(newplayerRect.intersects(wallRect))
            {
                wall->show();
            }
            if (playerRect.intersects(wallRect))
            {
                if(wall->type=="worm")
                {
                    wormgame++;
                    if(wormgame==2)
                    {
                        wall->hide();
                        wall->move(690,55);
                        showDialog("这虫子（worm）一下钻到地下不见了","维克多");
                    }
                    else if(wormgame==3)
                    {
                        wall->hide();
                        wall->move(60,485);
                        showDialog("跑的真快，下次我一定要抓住你","维克多");
                    }
                    else if(wormgame==4)
                    {
                        wall->hide();
                        showDialog("抓到了worm","旁白");
                        for (const QString& letter : wall->dropLetters)
                        {
                            inventory.append(letter); // 加入背包
                        }
                        encyclopedia["worm"] = encyclopedia.size();
                        objects.removeAll(wall);
                        obj31.removeAll(wall);
                        wall->deleteLater();
                    }
                }
                if(wall->type=="tpdoor")
                {
                    fromAtoB(31,3);
                }
                return;
            }
        }
    }
    else
    {
        shootTorchlight2();
        QRect newplayerRect(x,y,player->width(),player->height());
        const int interactRange = 20;
        newplayerRect.adjust(-interactRange, -interactRange,
                             interactRange,  interactRange);
        for(GameObject* wall:objects)
        {
            Zombie* zombie = dynamic_cast<Zombie*>(wall);
            if(zombie)
            {
                if(zombie->light)
                {
                    zombie->light=false;
                    continue;
                }
                else
                {
                    zombie->move(zombie->pos() + zombie->val);
                    if (zombie->y() < zombie->start.y() || zombie->y() + zombie->height() > zombie->end.y())
                        zombie->val.setY(-zombie->val.y());
                    if (zombie->x() < zombie->start.x() || zombie->x() + zombie->width() > zombie->end.x())
                        zombie->val.setX(-zombie->val.x());
                }

            }
        }
        for(GameObject* wall:objects)
        {
            if(wall->number!=2)continue;
            if(wall->type=="book")continue;
            QRect wallRect(wall->x(), wall->y(), wall->width(), wall->height());
            if(newplayerRect.intersects(wallRect))
            {
                wall->show();
            }
            if (playerRect.intersects(wallRect))
            {
                if(wall->type=="zombie")
                {
                    showGameOver("你被僵尸发现了");
                }
                if(wall->type=="tpdoor")
                {
                    fromAtoB(41,4);
                }
                return;
            }


        }

    }
    // 边界检测（保留）
    if (x < 0) x = 0;
    if (x > 800 - player->width()) x = 800 - player->width();
    if (y < 0) y = 0;
    if (y > 600 - player->height()) y = 600 - player->height();

    // 更新玩家位置
    player->move(x, y);


    //教程部分
    //1.欢迎
    if (currentLevel == 1)
    {
        if(tutorialStep==-1)
        {
            QVector<DialogLine> lines = {
                {"？？？", "醒来吧……\n勇士维克多（victor）……"},
                {"维克多", "你是？"},
                {"女王", "我乃字母世界的女王（queen）\n近来，字母世界的字母频频失踪，王国危在旦夕\n希望你能帮帮我们"},
                 {"女王", "收下我仅有的这五个元音字母吧\n期待你收集满26个字母的那一天"},
                 {"旁白", "欢迎来到字母世界！\n先按B键打开背包看看女王的礼物吧"}
            };
            startDialog(lines);
            tutorialStep++;
        }
        else if(tutorialStep == 1)
        {
            tutorialStep=2;
            QVector<DialogLine> lines = {
                {"旁白", "在本作里，你可以按W,A,S,D进行移动，按E键与物品交互"},
                {"旁白", "现在，试着与不远处的箱子（box）交互吧"}
            };
            startDialog(lines);

        }
        else if(tutorialStep == 2&&encyclopedia.contains("box"))
        {
            QVector<DialogLine> lines = {
                {"旁白", "获得了新物品：box"},
                {"旁白", "没错，就是这样\n你可以打开背包查看你的战果\n接下来只要走到门那里就可以到下一关啦"}
            };
            startDialog(lines);
            tutorialStep=3;
        }
        else if(tutorialStep == 3)
        {
            QRect playerRect(player->geometry());
            const int interactRange = 5;
            playerRect.adjust(-interactRange, -interactRange,
                              interactRange,  interactRange);
            for (GameObject *obj : objects) {
                if (obj->type == "tree") {
                    if (playerRect.intersects(obj->geometry())) {
                        tutorialStep = 4;
                        showDialog("有些物品徒手无法破坏\n试着在背包里合成一把斧子吧","旁白");
                        break;
                    }
                }
            }
        }
        else if (tutorialStep == 5 && currentSave.number==1)
        {
            tutorialStep = 6;
            for(int i=0;i<3;i++)
            {   GameObject *rock = new Rock(false, centralWidget());
                if(i==0){x=655;y=140;}
                else if(i==1){x=700;y=180;}
                else if(i==2){x=670;y=480;}
                rock->move(x, y);
                rock->show();
                objects.append(rock);
            }
            QVector<DialogLine> lines = {
                {"旁白", "不好，前方有落石，看起来需要合成一些工具清理他们"},
                {"旁白", "如果卡关了，可以试着点击背包界面右上角的“?”获取提示"}
            };
            startDialog(lines);
            startRockRain();
        }
    }
}
void MainWindow::updateBagUI()
{
    // 清空右侧图鉴
    QLayoutItem *child;
    while ((child = rightLayout->takeAt(0)) != nullptr) {
        delete child->widget(); // 只删内容
        delete child;
    }
    // ========== 1. 字母点亮 ==========
    for (int i = 0; i < 26; i++) {
        QLabel *lbl = qobject_cast<QLabel*>(
            letterGrid->itemAt(i)->widget()
            );
        char c = 'A' + i;
        if (inventory.contains(QString(c))) {
            lbl->setStyleSheet(
                "background-color: gold;"
                "color: black;"
                "font-weight: bold;"
                "border-radius: 5px;"
                );
        } else {
            lbl->setStyleSheet(
                "background-color: gray;"
                "color: black;"
                "border-radius: 5px;"
                );
        }
    }
    buildEncyclopediaPage(currentPage);
    //教程
    // 4.打开背包
    if (tutorialStep == 0&&currentLevel==1)
    {
        QVector<DialogLine> lines = {
            {"旁白", "如你所见，左侧的上方显示了你目前拥有的字母\n刚刚女王已经送给了你五个元音字母"},
            {"旁白", "游戏的目标就是收集满26个字母！\n剩下的21个字母就得靠你自己收集了"},
            {"旁白","现在可以按Esc键退出背包界面"}
        };
        startDialog(lines);
        tutorialStep++;
    }
    // 6.打开背包
    if (tutorialStep == 4&&currentLevel==1 )
    {
        QVector<DialogLine> lines = {
            {"旁白", "试着在左下方的输入框里输入“ax”吧\n然后点击合成按钮"}
        };
        startDialog(lines);
        tutorialStep++;
    }
}
void MainWindow::buildEncyclopediaPage(int page)
{
    // ✅ 按解锁顺序排序
    QList<QString> sortedItems = encyclopedia.keys();
    std::sort(sortedItems.begin(), sortedItems.end(),
              [this](const QString &a, const QString &b) {
                  return encyclopedia[a] < encyclopedia[b];
              });

    int start = page * PageSize;
    int end   = qMin(start + PageSize, sortedItems.size());

    for (int i = start; i < end; ++i) {
        const QString &name = sortedItems[i];

        QWidget *item = new QWidget;
        item->setMinimumHeight(40);
        QHBoxLayout *hl = new QHBoxLayout(item);

        QLabel *icon = new QLabel(name);
        icon->setStyleSheet("color: white; font-size: 18px;");

        QPushButton *equip = new QPushButton("装备");
        equip->setStyleSheet(
            "color: black; background-color: lightgray;"
            );

        connect(equip, &QPushButton::clicked, this, [=]() {
            heldItem = name;
            updateHeldItemUI();
            showDialog("已装备：" + heldItem);
        });

        hl->addWidget(icon);
        hl->addWidget(equip);
        rightLayout->addWidget(item);
    }
}
void MainWindow::showGameOver(const QString &reason)
{
    deathReasonLabel->setText(reason);
    gameOverPanel->show();
    gameOverPanel->raise();

    pauseGame(); // ✅ 死亡即暂停
}
void MainWindow::showDialog(const QString &text, const QString &speakerKey)
{
    if (dialogDelay > 0)
        return;

    dialogDelay = 1;
    dialogName->setText(speakerKey);
    fullText = text;
    textIndex = 0;
    dialogText->clear();
    QString color = speakerColors.value(speakerKey, "#FFFFFF");


    dialogName->setText(speakerKey);
    dialogName->setStyleSheet(QString(
                                  "color: %1;"
                                  "font-size: 22px;"
                                  "font-weight: bold;"
                                  "padding-left: 5px;"
                                  ).arg(color));
    dialogName->show();


    dialogBox->show();
    dialogBox->raise();

    textTimer->start(40); // 打字速度
}
void MainWindow::startDialog(const QVector<DialogLine> &lines)
{
    if (lines.isEmpty()||dialogBox->isVisible())
        return;

    dialogLines = lines;
    dialogIndex = 0;
    showNextDialog();
}
void MainWindow::showNextDialog()
{
    if (dialogIndex >= dialogLines.size()) {
        dialogBox->hide();
        return;
    }

    const DialogLine &line = dialogLines[dialogIndex];
    showDialog(line.text, line.speaker);
    dialogIndex++;
}
void MainWindow::loadGame(SaveData s)
{
    loadLevel(s.level);
    player->move(s.lastCheckpoint);
    inventory = s.inventory;
    encyclopedia = s.encyclopedia;
    tutorialStep=s.tutorialStep;
    if(tutorialStep>=0&&s.level==4)
    {
        zombiegame=1;
    }
    updateBagUI();
    updateHeldItemUI();
}
void MainWindow::startRockRain()
{
    rockgame = 1;
    rockSpawnTimer = new QTimer(this);
    connect(rockSpawnTimer, &QTimer::timeout, this, &MainWindow::spawnFallingRock);
    rockSpawnTimer->start(2000); // 每 2 秒来一颗
}
void MainWindow::spawnFallingRock()
{
    if(rockgame==0||rockgame>=10)return;
    rockgame++;
    int x,y;
    if(rand()%2==1)
    {
        x = 475+rand()%70;
        y = 200+rand()%320;
    }
    else
    {
        x = 655+rand()%70;
        y = 200+rand()%320;
    }

    // 阴影
    Rock *shadow = new Rock(true, centralWidget());
    shadow->move(x, y);

    shadow->show();
    objects.append(shadow);

    // 2 秒后实体
    QTimer* newTimer = new QTimer(this);
    newTimer->setSingleShot(true);
    warningTimer.append(newTimer);
    connect(newTimer, &QTimer::timeout, this, [=]() {
        if(gamePaused)return;
        warningTimer.removeAll(newTimer);

        Rock *rock = new Rock(false, centralWidget());
        rock->move(x, y);
        rock->show();
        objects.append(rock); // ✅ 统一进世界
    });
   newTimer->start(1500);

}
void MainWindow::fromAtoB(int a,int b)
{
    if(a==2&&b==21)
    {
        for(GameObject*obj:obj21)
        {
            obj->show();
        }
        for(GameObject*obj:obj2)
        {
            obj->hide();
        }
        player->move(750,265);
    }
    else if(a==21&&b==2)
    {
        for(GameObject*obj:obj21)
        {
            obj->hide();
        }

        for(GameObject*obj:obj2)
        {
            obj->show();
        }
        player->move(30,265);
    }
    else if(a==2&&b==22)
    {
        for(GameObject*obj:obj2)
        {
            obj->hide();
        }

        for(GameObject*obj:obj22)
        {
            obj->show();
        }
        player->move(30,265);
    }
    else if(a==22&&b==2)
    {
        for(GameObject*obj:obj22)
        {
            obj->hide();
        }

        for(GameObject*obj:obj2)
        {
            obj->show();
        }
        player->move(750,265);
    }
    else if(a==3&&b==31)
    {
        for(GameObject*obj:obj3)
        {
            obj->hide();
        }
        for(GameObject*obj:obj31)
        {
            obj->show();
        }
        player->move(750,280);
        if(!encyclopedia.contains("torch"))
        {
            showDialog("好黑的环境啊，我似乎需要一个手电筒","维克多");
        }
        wormgame=1;
        torchlight = new GameObject(centralWidget());
        shootTorchlight();
    }
    else if(a==31&&b==3)
    {
        for(GameObject*obj:obj31)
        {
            obj->hide();
        }

        for(GameObject*obj:obj3)
        {
            obj->show();
        }
        player->move(30,265);
        wormgame=0;
        torchlight->hide();
    }
    else if(a==4&&b==41)
    {
        for(GameObject*obj:obj4)
        {
            obj->hide();
        }


        for(GameObject*obj:obj41)
        {
            obj->show();
        }
        player->move(405,520);
        zombiegame=1;
        torchlight = new GameObject(centralWidget());
        if(tutorialStep==0)
        {
            tutorialStep=1;
            showDialog("好黑的环境啊，又要打开手电筒了","维克多");
        }
        if(tutorialStep==1)
        {
            currentSave.tutorialStep=1;
            currentSave.lastCheckpoint=player->pos();
        }

        shootTorchlight2();
    }
    else if(a==41&&b==4)
    {
        for(GameObject*obj:obj41)
        {
            obj->hide();
        }

        for(GameObject*obj:obj4)
        {
            obj->show();
        }
        player->move(400,20);
        zombiegame=0;
        torchlight->hide();
    }
}
void MainWindow::shootTorchlight()
{
    if (!wormgame) return;
    for (QWidget* obj : objects)
    {
        obj->hide();
    }
    if(heldItem!="torch")
    {
        torchlight->hide();
        return;
    }
    QPoint start = player->pos()+QPoint(20,20) ; // 手电筒起点
    QPoint dir(1, 0);
    switch (face) {
    case 2:  dir = QPoint(-1, 0);start = start+QPoint(-20,0); break;//左
    case 3: dir = QPoint(1, 0);start = start+QPoint(20,0); break;//右
    case 0:    dir = QPoint(0, -1);start = start+QPoint(0,-20); break;//上
    case 1:  dir = QPoint(0, 1);start = start+QPoint(0,20); break;//下
    }
    int step = 1;     // 每次前进 5 像素
    int maxDist = 800; // 最大距离
    int d = 0;
    for (; d < maxDist; d += step) {
        QPoint p = start + dir * d;

        // 检测是否撞到障碍物
        bool hit = false;
        for (GameObject* obj : objects) {
            if(obj->number!=2)continue;
            if(obj->type=="tpdoor")continue;
            if (obj->geometry().contains(p)) {
                obj->show();   // ✅ 照亮
                hit = true;
                break;
            }
        }

        if (hit) break;
    }

    if(torchlight)
    {
        torchlight->hide();
    }
    torchlight->setStyleSheet("background-color: yellow;");

    switch (face) {
    case 2:  torchlight->setGeometry(start.x()-d+1, start.y()-4, d-6, 8); break;
    case 3: torchlight->setGeometry(start.x()+5, start.y()-4, d-5, 8); break;
    case 0:    torchlight->setGeometry(start.x()-4, start.y()-d+1, 8, d-6); break;
    case 1: torchlight->setGeometry(start.x()-4, start.y()+5, 8, d-5); break;
    }

    torchlight->show();
}
QPoint MainWindow::rodTip()
{
    QPoint base(400, 300);
    int x = base.x() - std::cos(rodAngle * 3.14159f / 180) * rodLength;
    int y = base.y() - std::sin(rodAngle * 3.14159f / 180) * rodLength;
    return QPoint(x, y);
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == fishingWidget && event->type() == QEvent::Paint) {
        if (!fishgame)
            return true;
        QPainter p(fishingWidget);
        p.setPen(QPen(Qt::yellow, 4));
        p.drawLine(base, tip);
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}
void MainWindow::onTick()
{
    if (!fishCaught) {
        fish->move(fish->x() + fishVel.x(),
                   fish->y() + fishVel.y());
        if (fish->y() <= 100 || fish->y() + fish->height() >= 500)
            fishVel.setY(-fishVel.y());
    }
    if (!rodCasting) {
        rodAngle += rodSpeed;
        if (rodAngle > 45)  rodSpeed = -2;
        if (rodAngle < -45) rodSpeed = 2;
    }
    if (rodCasting==1) {
        if (!fishCaught && rodLength < 300)
        {
            rodLength += 6;
        }
        else
        {
            rodCasting=2;
        }
        if (!fishCaught &&fish->geometry().contains(rodTip()))
        {
            fishCaught = true;
        }


    }
    if(rodCasting==2)
    {
        rodLength -= 6;
        if (fishCaught) {
            fish->move(rodTip());
        }
        if (rodLength <= 40)
        {
            rodCasting = 0;
            if(fishCaught)
            {
                fishgame=0;
                fish->hide();
                showDialog("解锁了新物品：fish","旁白");
                encyclopedia["fish"] = encyclopedia.size();
                inventory.append("F");inventory.append("S");inventory.append("H");
                tutorialStep=3;
            }
        }
    }
    base = QPoint(400,300);
    tip  = rodTip();
    fishingWidget->update();
}
void MainWindow::shootTorchlight2()
{
    if (!zombiegame) return;
    for (QWidget* obj : objects)
    {
        obj->hide();
    }
    if(heldItem!="torch"&&heldItem!="gun")
    {
        torchlight->hide();
        return;
    }

    QPoint start = player->pos()+QPoint(20,20) ; // 手电筒起点
    QPoint dir(1, 0);
    switch (face) {
    case 2:  dir = QPoint(-1, 0);start = start+QPoint(-20,0); break;//左
    case 3: dir = QPoint(1, 0);start = start+QPoint(20,0); break;//右
    case 0:    dir = QPoint(0, -1);start = start+QPoint(0,-20); break;//上
    case 1:  dir = QPoint(0, 1);start = start+QPoint(0,20); break;//下
    }
    int step = 1;     // 每次前进 5 像素
    int maxDist = 800; // 最大距离
    int d = 0;
    for (; d < maxDist; d += step) {
        QPoint p = start + dir * d;

        // 检测是否撞到障碍物
        bool hit = false;
        for (GameObject* obj : objects) {
            if(obj->number!=2)continue;
            if(obj->type=="tpdoor")continue;
            if (obj->geometry().contains(p)) {
                obj->show();   // ✅ 照亮
                if(obj->type=="zombie")
                {
                    Zombie* zombie = dynamic_cast<Zombie*>(obj);
                    zombie->light=true;
                    if(keyE&&heldItem=="gun")
                    {
                        obj41.removeAll(obj);
                        objects.removeAll(obj);
                        obj->hide();
                        if(!encyclopedia.contains("zombie"))
                        {
                            showDialog("获得了新物品：zombie","旁白");
                            encyclopedia["zombie"] = encyclopedia.size();
                            inventory.append("Z");
                        }
                    }
                }
                hit = true;
                break;
            }
        }

        if (hit) break;
    }

    if(torchlight)
    {
        torchlight->hide();
    }
    torchlight->setStyleSheet("background-color: yellow;");

    switch (face) {
    case 2:  torchlight->setGeometry(start.x()-d+1, start.y()-4, d-6, 8); break;
    case 3: torchlight->setGeometry(start.x()+5, start.y()-4, d-5, 8); break;
    case 0:    torchlight->setGeometry(start.x()-4, start.y()-d+1, 8, d-6); break;
    case 1: torchlight->setGeometry(start.x()-4, start.y()+5, 8, d-5); break;
    }

    torchlight->show();

}
QString MainWindow::getYourname(int i)
{
    QDialog Qdlg(this);
    Qdlg.setModal(true);
    if(i==1)
        {Qdlg.setWindowTitle("Q的单词是");}
    else if(i==2)
        {Qdlg.setWindowTitle("V的单词是");}
    Qdlg.setFixedSize(280, 100);
    QLineEdit *edit = new QLineEdit(&Qdlg);
    QPushButton *btn = new QPushButton("确认", &Qdlg);

    edit->setMaxLength(12);
    if(i==1)
    {edit->setPlaceholderText("请输入一个带有Q的单词");}
    else if(i==2)
    {
        edit->setPlaceholderText("请输入一个带有V的单词");
    }

    QVBoxLayout *layout = new QVBoxLayout(&Qdlg);
    layout->addWidget(edit);
    layout->addWidget(btn);

    QString playerName;

    connect(btn, &QPushButton::clicked, [&]() {
        playerName = edit->text().trimmed();
        Qdlg.accept();
    });
    Qdlg.exec();   // 弹出来
    return playerName;
}
void MainWindow::gameTips()
{
    if(currentLevel==1&&tutorialStep<=5)
    {
        showDialog("我是提示按钮！\n现在先跟着教程来吧","Tips");
    }
    else if(currentLevel==1&&tutorialStep==6)
    {
        showDialog("你知道西游记的故事吗！\n从石头里蹦出来的孙猴子的武器是啥？！\n好像是铁棒（bat）呢！","Tips");
    }
    else if(currentLevel==2&&tutorialStep==-1)
    {
        showDialog("你都没看完地图呢\n不要这么心急！","Tips");
    }
    else if(currentLevel==2&&tutorialStep==0)
    {
        showDialog("湖面这么不平静\n给人一种很想把某些东西丢下去的感觉（无端）","Tips");
    }
    else if(currentLevel==2&&tutorialStep==1)
    {
        showDialog("把湖上的涟漪捡起来吧","Tips");
    }
    else if(currentLevel==2&&(tutorialStep==2||tutorialStep==3))
    {
        showDialog("和渔夫对话获得线索吧","Tips");
    }
    else if(currentLevel==2&&tutorialStep>=4)
    {
        showDialog("好像也不知道该提示什么了\n把鱼给伐木工，再与树交互就好了","Tips");
    }
    else if(currentLevel==3&&tutorialStep==-1&&!encyclopedia.contains("torch"))
    {
        showDialog("先合成一个torch照明吧","Tips");
    }
    else if(currentLevel==3&&tutorialStep==-1&&encyclopedia.contains("torch"))
    {
        showDialog("抓3次沙虫以后就可以合成flower啦","Tips");
    }
    else if(currentLevel==3&&tutorialStep==0&&!encyclopedia.contains("jar"))
    {
        showDialog("jar也在洞穴里，而且离入口挺近的，仔细找找吧","Tips");
    }
    else if(currentLevel==3&&tutorialStep==1)
    {
        showDialog("合成钥匙“key”","Tips");
    }
    else if(currentLevel==4&&tutorialStep<=1)
    {
        showDialog("进矿坑也要带手电哦\n地上随机散落的笔记也许会有帮助","Tips");
    }
    else if(currentLevel==4&&tutorialStep==2)
    {
        showDialog("合成枪（gun）和僵尸们爆了！","Tips");
    }
    else if(currentLevel==4&&tutorialStep>=3)
    {
        showDialog("笔记里反复提及的不就是人的名字吗\n话说你叫啥来着？Victor？","Tips");
    }
    else
    {
        showDialog("这里暂时没有提示哦","Tips");
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
