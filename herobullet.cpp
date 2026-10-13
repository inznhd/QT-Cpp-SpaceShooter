#include "herobullet.h"

HeroBullet::HeroBullet(int kind,double x,double y,QObject *parent,double toX,double toY) : BulletBase(parent)
  ,kinds(kind)
{
    //默认方向
    v_x=0;
    v_y=-1;
    state=2;//暂时测试，后期初始为0收到发射信号才设置为2
    setPos(x,y);
    //非默认方向时
    if(!(toX==0&&toY==0))
    {
        v_x=toX/sqrt(toX*toX+toY*toY);
        v_y=toY/sqrt(toX*toX+toY*toY);
    ini_set();
    }

    //问题：
}
//空构造
HeroBullet::HeroBullet(int kind,QObject *parent) : BulletBase(parent)
    ,kinds(kind)
{
    setPos(-200,-200);
    v_x=0;
    v_y=-1;
    state=0;
    ini_set();
}

QRectF HeroBullet::boundingRect() const
{
    return QRectF(0,0,pix.width(),pix.height());
}

void HeroBullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0,0,pix);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void HeroBullet::upDate()
{
    if(state==0)
    {
        return;
    }

    updatePosition();
    detectCollision();
}

void HeroBullet::updatePosition()
{
    setX(x()+v_x);
    setY(y()+v_y);
    if(x()<0||x()>750||y()<0)
    {
        setDeath();
    }
}

void HeroBullet::detectCollision()
{

    //获取子弹自身的边界矩形（转换到场景坐标）
    QRectF bulletRect = mapToScene(boundingRect()).boundingRect();
    //检测于子弹矩形相交的对象
    QList<QGraphicsItem *> collidingList = scene()->items(bulletRect,Qt::IntersectsItemShape);
    //强制转换成敌机，成功则继续//筛选出敌机目标
    foreach(QGraphicsItem *item, collidingList)
    {
        if(EnemyPlaneBase* enyB = dynamic_cast<EnemyPlaneBase *>(item))
        {
            Q_UNUSED(enyB);
            emit hitEnemyp(enyB,damage);
            emit bulletBoom(0,x(),y()-30);
            setDeath();
            break;
        }
    }

}

void HeroBullet::reUseBullet(double x, double y)
{
    setPos(x-pix.width()/2,y);
    v_y=speed;
    v_y*=-1;
    v_x=0;
    state=2;

}

void HeroBullet::reUseBullet(double x, double y, double tox, double toy)
{
    setPos(x-pix.width()/2,y);
    v_x=speed*tox/sqrt(tox*tox+toy*toy);
    v_y=speed*toy/sqrt(tox*tox+toy*toy);
    state=2;
}

void HeroBullet::ini_set()
{
    switch (kinds) {
    case 1:
    {
        pix.load(":/heroBullet/heroBullet01.png");
        damage=10;
        frequency=0.5;
        speed=3;
        break;
    }
    case 2:
    {
        pix.load(":/heroBullet/heroBullet02.png");
        damage=20;
        frequency=0.3;
        speed=4;
        break;
    }
    case 3:
    {
        pix.load(":/heroBullet/heroBullet03.png");
        damage=40;
        frequency=0.4;
        speed=5.5;
        break;
    }
    case 4:
    {
        pix.load(":/heroBullet/heroBullet04.png");
        damage=40;
        frequency=0.2;
        speed=4;
        break;
    }
    case 5:
    {
        pix.load(":/heroBullet/heroBullet05.png");
        damage=225;
        frequency=1;
        speed=8;
        break;
    }
    case 6:
    {
        pix.load(":/heroBullet/heroBullet06.png");
        damage=175;
        frequency=0.75;
        speed=2.5;
        break;
    }
    case 7:
    {
        pix.load(":/heroBullet/heroBullet07.png");
        damage=250;
        frequency=0.75;
        speed=5;
        break;
    }
    }
    setX(x()-pix.width()/2);
    v_x*=speed;
    v_y*=speed;
}

////void EnemyButton::setDeath()
////{
////    state=0;
////    setPos(-100,-100);
////    qDebug()<<"敌人子弹死亡";
////    //kinds保持不变？
////}



