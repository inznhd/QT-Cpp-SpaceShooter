#ifndef BULLETBASE_H
#define BULLETBASE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>
#include <QPixmap>
#include <math.h>
#include <QString>
#include "enemyplanebase.h"
#include <QGraphicsScene>
#include "heroplane.h"

class BulletBase : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    //不允许空父类对象，保证堆空间正常自动析构
    explicit BulletBase(QObject *parent);
    //重载QGraphisItem必须的边界检测函数和绘画函数（qt自动调用），保持纯虚函数
    virtual QRectF boundingRect() const =0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) =0;
    //刷新纯虚函数函数，里面需要包含状态检测，非活跃状态跳过（遍历容器的时候直接调用这个）
    virtual void upDate()=0;
    //直接获取状态（需要在对象池中查找空闲对象的时候用这个）？？是不是还需要获取子弹类别
    int getState();


signals:
    //命中战机发射信号
    //下一个版本考虑拆开两个信号，把Object放到子类中再继承
    void hitHerop(double hp);
    //撞到敌机的信号
    void hitEnemyp(EnemyPlaneBase *emy,double hp);
    //死亡播放动画
    void bulletBoom(int kind,double x,double y);

protected:
    //刷新位置的纯虚函数
    virtual void updatePosition()=0;
    //刷新检测碰撞的纯虚函数
    virtual void detectCollision()=0;
    //子弹消失的纯虚函数
    virtual void setDeath();

    //参数：
    int state;//0为空闲，2为发射中，
};

#endif // BULLETBASE_H
