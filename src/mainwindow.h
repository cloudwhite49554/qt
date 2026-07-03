#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include<QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include<QPushButton>
#include <QStandardPaths>
#include <QDir>
#include "gameobject.h"
//#include<fishingwidget.h>
#include "player.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QWidget *fishingWidget;
    QWidget *gameLayer;
    QWidget *uiLayer;//背包
    QWidget *startWidget;//开始界面
    QWidget *pauseMenu;    // 暂停菜单界面
    QWidget *cgWidget;
    QWidget *gameOverPanel = nullptr;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    struct SaveData {
        int level = 0;
        QStringList inventory;
        QMap<QString, int> encyclopedia;
        int tutorialStep;
        QPoint lastCheckpoint;
        int number=0;
    };
    struct DialogLine {
        QString speaker;
        QString text;
    };

protected:
    // 重写这两个函数（重点！）
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event)override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void loadLevel(int level);
    void updateBagUI();//更新图鉴，，，加物品改
    void updateHeldItemUI();//更新手持物品，，，不用改
    void initStartMenu();//开始界面，，，不用改
    void initGameMenu();
    void initGameOverUI();
    void initBagUI();//初始化背包界面，，，有合成系统，，，加物品改
    void initGame();//初始化游戏界面，，，不用改
    void gameLoop();//初始化游戏界面，，，有教程
    void loadGame(SaveData x);//从上次保存的位置载入   
    void initDialog();
    void showDialog(const QString &text, const QString &name="");
    void startDialog(const QVector<DialogLine> &lines);
    void showNextDialog();
    void showGameOver(const QString &reason);
    void spawnFallingRock();//落石
    void startRockRain();
    void pauseGame();
    void resumeGame();
    void clearGame();
    void fromAtoB(int a,int b);
    bool eventFilter(QObject *obj, QEvent *event) override;


private:
    Ui::MainWindow *ui;

    Player *player=nullptr; // 把玩家定义为成员变量，方便所有函数访问
    QList<GameObject*> objects;

    QVector<GameObject*> checkpoints;
    bool keyW=false, keyA=false, keyS=false, keyD=false,keyE=false; // 记录按键状态
    int face=0;//记录朝向
    QList<QString> inventory;//字母列表
    QMap<QString, int> encyclopedia; // ✅ 图鉴集合
    QWidget *bagWidget;   // ✅ 背包窗口
    QGridLayout *letterGrid;   // 左上
    QVBoxLayout *rightLayout;  // 右侧图鉴
    bool bagOpen = false; // ✅ 背包是否打开
    int currentPage=0;
    QPushButton *prevPageBtn = nullptr;
    QPushButton *nextPageBtn = nullptr;
    void buildEncyclopediaPage(int page);
    const int PageSize=7;
    void gameTips();
    QTimer *timer=nullptr;   // ✅ 改成成员变量
    QString heldItem = "";  //手持物品
    QLabel *heldItemLabel=nullptr; //手持物品显示
    bool gamePaused = false;
    //
    QWidget *dialogBox;
    QLabel *dialogName;
    QLabel *dialogText;
    QTimer *textTimer;
    QString fullText;
    QVector<DialogLine> dialogLines;
    int dialogIndex = 0;
    int dialogDelay = 0;
    QMap<QString, QString> speakerColors;

    int textIndex;
    int tutorialStep=-1;
    int currentLevel;
    //
    QPushButton *menuBtn;   // 右上角菜单按钮
    QPushButton *tipBtn;
    QLabel  *deathReasonLabel = nullptr;

    //有关石の
    SaveData currentSave;
    int rockgame=0;
    QTimer *rockSpawnTimer = nullptr;
    QList<QTimer*> warningTimer;
    //
    //第二关
    QList<GameObject*>obj2;
    QList<GameObject*>obj21;
    QList<GameObject*>obj22;
    int currentMap=0;
    bool fishgame=0;
    QPoint rodTip();
    void onTick();
    bool firstmeetlake=0,ifthrowrock=0,iftalkwithfishman=0,ifgivefish=0,iftalkwithlumberjack=0;

    float rodAngle   = 0;
    float rodSpeed   = 2;
    float rodLength  = 40;
    int  rodCasting = 0;
    GameObject* fish = nullptr;
    QPoint fishVel;
    bool fishCaught = false;
    QPoint base;              // ✅ 线起点
    QPoint tip;

    //
    //第三关
    QList<GameObject*>obj3;
    QList<GameObject*>obj31;
    int wormgame=0;
    GameObject* torchlight=nullptr;
    void shootTorchlight();
    //
    //第四关
    QList<GameObject*>obj4;
    QList<GameObject*>obj41;
    void shootTorchlight2();
    int zombiegame=0;
    QString getYourname(int i);
};
#endif // MAINWINDOW_H
