#include "enemyplanebase.h"

EnemyPlaneBase::EnemyPlaneBase(double st,QObject *parent) : QObject(parent)
{
    starttime = st;
    count_start=0;
    state=0;
}

void EnemyPlaneBase::getHit(double reduce_hp)
{
    health-=reduce_hp;
    if(health<=0)
    {
        setDeath();
    }
    update();
}

void EnemyPlaneBase::setDeath()
{
//    if(pix.height()<200)
//    {
//        emit EnemyPBoom(1,x(),y());
//    }
//    else
//    {
//        emit EnemyPBoom(1,x(),y());
////        emit EnemyPBoom(1,x()+pix.width()/2,y()+pix.height()/2);
////        emit EnemyPBoom(1,x()+pix.width()/4*3,y()+pix.height());
////        emit EnemyPBoom(1,x()+pix.width()/5*2,y()+pix.height()/4*3);
////        emit EnemyPBoom(1,x()+pix.width()/5,y()+pix.height());
////        emit EnemyPBoom(1,x()+pix.width(),y()+pix.height()/4);
//    }
    setPos(-1000,-1000);
    state=4;
}

void EnemyPlaneBase::upDate()
{
    switch (state) {
    case 0:
    {
        count_start+=0.01;
        if(count_start>=starttime)
        {
            if(ini_x>=740-pix.width())
            {
                ini_x=740-pix.width();
            }
            setPos(ini_x,-pix.height()-10);
            state=1;
        }
        return;
    }
    case 1:
    {
        setY(y()+0.5);
        if(y()>=ini_y)
        {
            state=2;
        }
        return;
    }
    case 2:
    {
        updatePosition();
        attack();
        return;
    }
    case 3:
    {
        qDebug()<<"当前状态跳过，为什么会有debug？";
    }
    case 4:
    {
        return;
    }
    default:
    {
        qDebug()<<"敌机未知state";
    }
    }
//    if(state!=4)
//    {
//        if(state==0)
//        {
//            count_start+=0.01;
//            if(count_start>=starttime)
//            {
//                setPos(ini_x,-pix[kinds].height()-10);
//                state=1;
//            }
//            return;
//        }
//        updatePosition();
//        attack();
//        if(state==3)
//        {

//        }
//    }
}

QPainterPath EnemyPlaneBase::shape() const
{
    return path;
}

int EnemyPlaneBase::getState() const
{
    return state;
}
