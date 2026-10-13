#ifndef ENEMY2_H
#define ENEMY2_H

#include "enemyplanebase.h"



class Enemy2 : public EnemyPlaneBase      //子弹可以随机
{
public:
    Enemy2();
public:
    Enemy2(int kind,double x,double y,double vx,double xy,double st,int num,double *sec,QObject *parent);//sx,sy控制一点空范围，st开始时间
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;


protected:
    void ini_bulletSetion(int num,double* sec);//需要传入6个参数
    void attack() override;
    void updatePosition() override;
    void ini_set(int kind) override; //区分不同敌机的初始化
private:
    double frequency=0;//单位是秒//初始设为0，固定发射模式在ini中初始化，手动模式开始后才初始化
    double count = 0;//固定模式自动初始化为frequency，两模式均0贞起手
    double v_x;
    double v_y;
    //第几批次的子弹、下一间隔时长(秒)、子弹类型、子弹方向选择（1，手动方向，0，战机方向）、手动方向x、手动方向y//单信息长度
    //如果子弹类型有更多，需要回来改下面的length还有相关逻辑
    std::vector<double>bulletDataVec;
    int bulletAmount=0;
    int numcount=0;
    //子弹单信息长度使用的地方：ini_bulletSetion()中的i


};

#endif // ENEMY2_H
