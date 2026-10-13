#ifndef ENEMYPLANEBASE_H
#define ENEMYPLANEBASE_H

#include <QObject>
#include <QGraphicsItem>
#include <QRectF>
#include <QtDebug>
#include <vector>
#include <QPainter>
#include <QList>
#include <vector>
#include <QPainterPath>


class EnemyPlaneBase : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit EnemyPlaneBase(double st,QObject *parent = nullptr);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) =0;
    virtual QRectF boundingRect() const =0;
    QPainterPath shape() const;
    virtual void upDate();
    void getHit(double reduce_hp);
    int getState()const;

signals:
    void emitBullet3(int bulletKind,double x,double y);//重载发射信号connect的时候要连接两次，不如直接别名
    void emitBullet5(int bulletKind,double x,double y,double tox,double toy);
    void EnemyPBoom(int kinds,double x,double y);

protected slots:
    //还要定义并连接受击函数
    //还要定义死亡函数，调整state阻止或者调控派生类的操作
    //该操作可以在基类完成

protected:
    virtual void ini_set(int kind)=0;//负责处理因为kind不同的参数
    virtual void attack()=0;
    virtual void updatePosition()=0;
    void setDeath();
    double health;
    double ini_health;
    double starttime;
    double count_start;
    int state;//状态，0表示未出场，1表示出场中，2表示活跃，4表示死亡
    int ini_x;
    int ini_y;
    QPixmap pix;
    int kinds;
    QPainterPath path;

private:

};

#endif // ENEMYPLANEBASE_H
