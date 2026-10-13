#ifndef HEROBULLET_H
#define HEROBULLET_H


#include "bulletbase.h"


class HeroBullet : public BulletBase
{
public:
    //不允许空父类对象
    explicit HeroBullet(int kind,double x,double y,QObject *parent,double toX=0,double toY=0);
    explicit HeroBullet(int kind,QObject *parent);//空构造，为对象池声明设计
    //重载默认自动管理的绘画和边界函数
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
    //重载刷新函数
    void upDate() override;
    //重载复用函数
    void reUseBullet(double x,double y);
    void reUseBullet(double x,double y,double tox,double toy);
    //初始化
    void ini_set();

protected:
    //重载位置更新函数
    void updatePosition() override;
    //重载碰撞检测函数（针对战机的碰撞）
    void detectCollision() override;
    //该类不需要重载
    //void setDeath() override;

private:
    //子弹类型，和pix以及各种基础参数关联
    int kinds;//类型(这里全部都是直线类型下的细分类型）后续考虑跟踪弹（可以尝试用switch区分逻辑）
    //子弹类型的基础参数：
    double speed;
    double damage;
    double frequency;
    //其他参数：
    QPixmap pix;//子弹图片
    double v_x;//速度参数
    double v_y;
    //QGRaphicsItem自带坐标；



};

#endif // HEROBULLET_H
