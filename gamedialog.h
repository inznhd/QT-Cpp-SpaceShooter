#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include <QDialog>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <string>
#include <fstream>
#include <QPainter>

#include <QObject>
#include <QVector>
#include <vector>
#include "bulletbase.h"
#include "enemybullet.h"
#include "herobullet.h"
#include "enemy1.h"
#include "heroplane.h"
#include "enemy2.h"
#include "enemy3.h"
#include <fstream>
#include <string>
#include "boom.h"
#include <QMessageBox>

#include "icon.h"
#include <QMouseEvent>

namespace Ui {
class GameDialog;
}

class GameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameDialog(int PlaneKind,int*bulletdata,int user_num,int acturalLevel,QWidget *parent);
    ~GameDialog();
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void win();
    void lose(double record);

private slots:
    void updateVector();
    void enemyEmitSlot3(int bulletKind,double x,double y);
    void enemyEmitSlot5(int bulletKind,double x,double y,double tox,double toy);
    void HitFromEmbuToHerop(double hp);
    void heroEmitSlot(int kind,double x,double y);
    void heroEmitSlot5(int kind,double x,double y,double toX,double toY);
    void HitFromHrbuToEmp(EnemyPlaneBase *emyp,double hp);
    void BombAddSlot(int kind,double x,double y);
    void winSlot();

private:
    Ui::GameDialog *ui;
    //一些基本参数
    //外边框宽高，可修改//以下两个数据在enemy、heroPlane、enemyButton中都同步不关联使用，要手动一起改
    const int heigth = 1000;
    const int width = 750;
    //其他参数
    QGraphicsView *view;//放在dialog上
    QGraphicsScene *scene;//放在view上，并装下游戏对象
    QTimer *timer;
    int acturalLevel;


    void upDateSolidRect();
    void readFile();//初始化敌人
    void upDateText();//更新剩余敌人数的显示
    //QVector<EnemyBullet*>enemyBulletList;//这个不用多态的形式，后期会有许多个子弹vector
    std::vector<std::vector<EnemyBullet*>>enemyBulletList1;
    std::vector<std::vector<HeroBullet*>>heroBulletList;//二维vector
    //int heroBulletKind[7];//不需要了，直接用新的herobullet【】代替
    //敌人vector
    QVector<EnemyPlaneBase*>enemyList;
    HeroPlane *heroP;
    //爆炸vector
    std::vector<Boom*>bombList0;
    std::vector<Boom*>bombList1;
    int enemyamount;
    int restEnemyamount;


    QGraphicsTextItem *text;
    QGraphicsRectItem *solidRect;
    QGraphicsRectItem *hollowRect;


    void start();
    void loseSlot();
    void deleteAllReNewObject();
    void updateMessageText();
    std::string filePath;
    int heroBullet[4]={-1,-1,-1,-1};//接受新式的子弹参数
    int bulletamount=0;
    int heroKind;
    int gamestate=0;//0为暂停中，1为游戏中
    Icon *stopIcon;
    Icon *continueIcon;
    Icon *againIcon;
    const QRectF iconRect[2]={
        (QRect(25,25,40,40)),
       (QRect(75,25,40,40)),
    };
    const QRectF messageRect=QRectF(-10,375,760,250);
    QGraphicsTextItem *messageText = nullptr;
    QGraphicsTextItem *messageText2 = nullptr;
    QGraphicsRectItem *messageRectItem = nullptr;
    int messageState=0;//0是游戏中，1是暂停中，2是继续倒计时，3是胜利，4是失败

};

#endif // GAMEDIALOG_H
