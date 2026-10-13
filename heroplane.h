#ifndef HEROPLANE_H
#define HEROPLANE_H

#include <QObject>
#include <QGraphicsItem>
#include <QRectF>
#include <QPixmap>
#include <QPainter>
#include <QKeyEvent>
#include <QSet>
#include <QtDebug>
#include <QPointF>
#include <QGraphicsScene>
#include <enemyplanebase.h>
#include <math.h>
#include <QPainterPath>
#include <QImage>

class HeroPlane : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit HeroPlane(int kind,int *bulletKinds,QObject *parent);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void update();
    double getRestHealthRate();//留个接口给dialog绘制血条
    QPointF getPos();//暴露一个接口给敌人子弹获取位置
    void getHit(double reduce_hp);
    void ini_section(int *sec);
    QPainterPath shape() const override;
    void clearKeySet();

private slots:

signals:
    void emitHeroBullet(int kind,double x,double y);
    void emitHeroBullet5(int kind,double x,double y,double toX,double toY);
    void heroPBoom(int kind,double x,double y);
    void heroPDie();


private:
    void updatePosition();
    void updateAttack();
    void detectCollision();
    void setDeath();
    //添加button样式
    int state;//0代表未出场？1代表出场中？2代表活跃、3代表死亡（动画）中、4代表死亡
    int kinds;
    double ini_health;
    int bulletNum;
    //子弹仓
    int bulletKind[7];
    double bulletfreq[7];
    double freq_count[7];

    const int speed=3;
    //const QPixmap pix[1]={(QPixmap("D:\\Qt_program\\text02.png"))};//最后连接QString在构造函数中加载pixMap，减小内存和运算
    QPixmap pix;//最后连接QString在构造函数中加载pixMap，减小内存和运算
    double health;
    QSet<int>keySet;

    QPainterPath path;

    //测试移动问题：
//    double x;
//    double y;
//    const double MyPlaneSpeed = 5;

signals:

};

#endif // HEROPLANE_H
