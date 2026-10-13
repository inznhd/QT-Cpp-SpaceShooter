#ifndef ENEMY1_H
#define ENEMY1_H

#include "enemyplanebase.h"

class Enemy1 : public EnemyPlaneBase//负责直线运动飞机、可选发射模式
{
public:
    Enemy1(int kind,double x,double y,double vx,double xy,double st,QObject *parent);//sx,sy控制一点空范围，st开始时间
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    void attack() override;
    void updatePosition() override;
    void ini_set(int kind) override; //区分不同敌机的初始化
private:
    double frequency=0;//单位是秒//初始设为0，固定发射模式在ini中初始化，手动模式开始后才初始化
    double count = 0;//固定模式自动初始化为frequency，两模式均0贞起手
    double v_x;
    double v_y;



};

#endif // ENEMY1_H
