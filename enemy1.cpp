#include "enemy1.h"

Enemy1::Enemy1(int kind,double x,double y,double vx,double vy,double st,QObject *parent
               ):EnemyPlaneBase(st,parent)
{
    kinds=kind;
    v_x=vx;
    v_y=vy;
    ini_x=x;
    ini_y=y;
    setPos(-1000,-1000);
    //区分不同敌机的参数
    ini_set(kinds);

    //问题：
    //状态3还没写

}

void Enemy1::ini_set(int kind)
{
    switch (kind) {
    case 0:
    {
        pix.load("D:\\Qt_program\\text01.png");
        health=100;
        ini_health=100;
        frequency=2.5;
        count=2.5;
        break;
    }
    case 1:
    {
        pix.load(":/enemy/01.png");
        health=35;
        ini_health=35;
        frequency=2.5;
        count=2.5;
        break;
    }
    case 2:
    {
        pix.load(":/enemy/02.png");
        health=100;
        ini_health=100;
        frequency=2.5;
        count=2.5;
        break;
    }
    case 3:
    {
        pix.load(":/enemy/03.png");
        health=300;
        frequency=2.5;
        count=2.5;
        break;
    }
    default:
    {
        qDebug()<<"无此类战机，初始化错误(from enemy1.cpp)";
    }
    }
    QImage img = pix.toImage();
    for(int x=0;x<img.width();x++)
    {
        for(int y=0;y<img.height();y++)
        {
            // 检查像素是否完全透明（alpha值为0）
            if (qAlpha(img.pixel(x, y)) > 0)
            {
            // 添加1x1矩形到路径（性能较低但精确）
            path.addRect(x, y, 1, 1);
            }
        }
    }

    /*这些是刚开始在构造函数中的代码，如果最后确保能正常运行可以删掉
     *     health=ini_health[kinds];
    setPos(-1000,-1000);
    size_x=sx-pix[kinds].width();
    size_y=sy-300-pix[kinds].height()
*/
}

QRectF Enemy1::boundingRect() const
{
    return QRectF(0,0,pix.width(),pix.height());
}

void Enemy1::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0,0,pix);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}


void Enemy1::updatePosition()
{

    setX(x()+v_x);
    setY(y()+v_y);
    if(x()>=750-pix.width())
    {
        setX(1500-2*pix.width()-x());//单位时间后反弹后的位置
        v_x*=-1;
    }
    else if(x()<=0)
    {
        setX(-x());
        v_x*=-1;
    }
    if(y()>=800-pix.height())
    {
        setY(1600-2*pix.height()-y());
        v_y*=-1;
    }
    else if(y()<=0)
    {
        setY(-y());
        v_y*=-1;
    }

}

void Enemy1::attack()
{
    count+=0.01;
    switch (kinds)
    {
    case 1:
    {
        if(count>=frequency)
        {
            emit emitBullet3(1,x()+pix.width()/2,y()+pix.height());
            count=0;
        }
        break;
    }
    case 2:
    {
        if(count>=frequency)
        {
            emit emitBullet3(2,x()+pix.width()/2,y()+pix.height());
            count=0;
        }
        break;
    }
    case 3:
    {
        if(count>=frequency)
        {
            emit emitBullet3(2,x()+pix.width()/2,y()+pix.height());
            count=0;
        }
        break;
    }
    }
}
