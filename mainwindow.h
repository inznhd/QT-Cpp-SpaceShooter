#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QGraphicsScene>
#include <QPainter>
#include <QPushButton>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QColor>
#include <QPoint>
#include <QRect>
#include <QMouseEvent>
#include <QtDebug>
#include <QVector>
#include "gamedialog.h"
#include <vector>
#include <fstream>
#include <QMessageBox>
#include "editor.h"
#include "userexchange.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    //重写绘制事件
    void paintEvent(QPaintEvent *event)override;
    //重写鼠标点击事件
    void mouseReleaseEvent(QMouseEvent *event)override;

private slots:
    //关卡胜利函数
    void winSlot();
    //关卡失败函数
    void loseSlot(double rate);

private:
    Ui::MainWindow *ui;
    //开始游戏函数入口
    void startGame();
    //读取用户数据
    void readFile(int userNumber);
    //刷新解锁情况
    void updateUnlock();
    //自定义新建关卡
    void newLevel();
    //切换用户
    void exchangeUser();

    //用户识别
    char userName[16]="Default User";
    //用户识别id
    int userNumber;
    //战机解锁数量
    int heroPlaneAmount;
    //当前选择的战机
    int nowPlaneType=1;
    //解锁的子弹数量
    int heroBulletAmount;
    const int max_bulletKind[4]={2,3,3,4};
    int nowBulletType[4]={1,1,-1,-1};
    //关卡的总数（非下标，从1开始)
    int levelCount;
    std::vector<int>levelNumber;
    std::vector<double>levelRecord;
    //正在改变的项
    int changePlaneType=-1;
    int changeBulletType=-1;
    const QVector<QRect> pixRect =
    {
         (QRect(25,25,75,75)),
        (QRect(100,25,75,75)),
        (QRect(175,25,75,75)),
        (QRect(250,25,75,75)),
        (QRect(325,25,75,75)),
    };
    const QVector<QRect> textRect =
    {
         (QRect(75,100,250,25)),
    };

    const QVector<QRect> choiseRect =
    {
         (QRect(25,125,75,75)),
        (QRect(100,125,75,75)),
        (QRect(175,125,75,75)),
        (QRect(250,125,75,75)),
        (QRect(325,125,75,75)),
        (QRect(400,125,75,75)),
        (QRect(475,125,75,75)),
    };
    QPixmap scalledPlanePix[4];
    QPixmap scalledBulletPix[7];

    //相关的初始固定参数：
    //初始界面的宽度和高度
    const int height=648;
    const int width=1150;
    //关卡位置参数
    const QVector<QRect> lvRect =
    {
        (QRect(width/6-75,height/2-75,150,150)),     //五个默认关卡的矩形
        (QRect(width/6*2-75,height/2,150,150)),
        (QRect(width/6*3-75,height/2-75,150,150)),
        (QRect(width/6*4-75,height/2,150,150)),
        (QRect(width/6*5-75,height/2-75,150,150)),
    };
    const QVector<QRect> lvRect2 =
    {
        (QRect(width/6-75,height/2,150,50)),     //五个默认关卡的矩形
        (QRect(width/6*2-75,height/2+75,150,50)),
        (QRect(width/6*3-75,height/2,150,50)),
        (QRect(width/6*4-75,height/2+75,150,50)),
        (QRect(width/6*5-75,height/2,150,50)),
    };
    const QRect exchangeUseRect=QRect(width-150,20,100,50);

    int totalPage = 0;
    int nowPage = 0;
    //进入的关卡编号
    int realLevel = 0;
};
#endif // MAINWINDOW_H

