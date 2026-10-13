#include "enemybullet.h"

EnemyBullet::EnemyBullet(int kind,double x,double y,QObject *parent,double toX,double toY) : BulletBase(parent)
  ,kinds(kind)
{

    state=2;//暂时测试，后期初始为0收到发射信号才设置为2
    setPos(x,y);
    if(toX==0&&toY==0)
    {
        //默认方向
        v_y=1;
        v_x=0;
    }else
    {
        v_x=toX/sqrt(toX*toX+toY*toY);
        v_y=toY/sqrt(toX*toX+toY*toY);
    }
    ini_set();

    //问题：
}
//空构造
EnemyBullet::EnemyBullet(int kind,QObject *parent) : BulletBase(parent)
    ,kinds(kind)
{
    setPos(-100,-100);
    v_x=0;
    v_y=1;
    state=0;
    ini_set();
}

QRectF EnemyBullet::boundingRect() const
{
    return QRectF(0,0,pix.width(),pix.height());
}

void EnemyBullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0,0,pix);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void EnemyBullet::upDate()
{
    if(state==0)
    {
        return;
    }

    updatePosition();
    detectCollision();
}

void EnemyBullet::updatePosition()
{
    setX(x()+v_x);
    setY(y()+v_y);
    if(x()<0||x()>750||y()<0||y()>1000)
    {
        setDeath();
    }
}

void EnemyBullet::detectCollision()
{
    //获取子弹自身的边界矩形（转换到场景坐标）
    QRectF bulletRect = mapToScene(boundingRect()).boundingRect();
    //检测于子弹矩形相交的对象
    QList<QGraphicsItem *> collidingList = scene()->items(bulletRect,Qt::IntersectsItemShape);
    //强制转换成战机，成功则继续//筛选出战机目标
    foreach(QGraphicsItem *item, collidingList)
    {
        if(HeroPlane* enyB = dynamic_cast<HeroPlane *>(item))
        {
            Q_UNUSED(enyB);
            emit hitHerop(damage);
            emit bulletBoom(0,x(),y()+10);
            setDeath();
            break;
        }
    }

}

void EnemyBullet::reUseBullet(double x, double y)
{
    setPos(x-pix.width()/2,y);
    v_y=baseSpeed;
    v_x=0;
    state=2;

}

void EnemyBullet::reUseBullet(double x, double y, double tox, double toy)
{
    setPos(x-pix.width()/2,y);
    v_x=baseSpeed*tox/sqrt(tox*tox+toy*toy);
    v_y=baseSpeed*toy/sqrt(tox*tox+toy*toy);
    state=2;
}

void EnemyBullet::ini_set()
{
    switch (kinds) {
    case 1:
    {
        pix.load(":/enemyBullet/enemyBullet01.png");
        damage=10;
        baseSpeed=1;
        break;
    }
    case 2:
    {
        pix.load(":/enemyBullet/enemyBullet02.png");
        damage=15;
        baseSpeed=1;
        break;
    }
    case 3:
    {
        pix.load(":/enemyBullet/enemyBullet03.png");
        damage=20;
        baseSpeed=1;
        break;
    }
    case 4:
    {
        pix.load(":/enemyBullet/enemyBullet04.png");
        damage=30;
        baseSpeed=1.5;
        break;
    }
    case 5:
    {
        pix.load(":/enemyBullet/enemyBullet05.png");
        damage=60;
        baseSpeed=1.75;
        break;
    }
    case 6:
    {
        pix.load(":/enemyBullet/enemyBullet06.png");
        damage=100;
        baseSpeed=2;
        break;
    }
    default:
    {
        qDebug()<<"敌人子弹类别识别错误";
        qDebug()<<kinds;
    }
    }
    setX(x()-pix.width()/2);//让他生成再我想要的位置中间
    v_x*=baseSpeed;
    v_y*=baseSpeed;
}

//void EnemyButton::setDeath()
//{
//    state=0;
//    setPos(-100,-100);
//    qDebug()<<"敌人子弹死亡";
//    //kinds保持不变？
//}


