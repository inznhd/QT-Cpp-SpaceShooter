#include "enemy2.h"

Enemy2::Enemy2(int kind,double x,double y,double vx,double vy,double st,int num,double *sec,QObject *parent
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
    ini_bulletSetion(num,sec);

    //问题：
    //状态3还没写

}

void Enemy2::ini_set(int kind)
{
    switch (kind) {
    case 0:
    {
        pix.load("D:\\Qt_program\\text01.png");
        health=100;
        ini_health=100;
        break;
    }
    case 1:
    {
        pix.load(":/enemy/11.png");
        health=35;
        ini_health=35;
        frequency=2.5;
        count=2.5;
        break;
    }
    case 2:
    {
        pix.load(":/enemy/12.png");
        health=100;
        ini_health=100;
        frequency=2.5;
        count=2.5;
        break;
    }
    case 3:
    {
        pix.load(":/enemy/13.png");
        health=300;
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

QRectF Enemy2::boundingRect() const
{
    return QRectF(0,0,pix.width(),pix.height());
}

void Enemy2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0,0,pix);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}


void Enemy2::updatePosition()
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

void Enemy2::attack()
{
    count+=0.01;
    if(count>=frequency)
    {

        if(bulletDataVec.empty())//理论上不能进入这里
        {
            qDebug()<<"战机子弹容器为空(from enemy2.cpp,构造后未调用ini_bulletSetion())";
        }
        //第几批次的子弹、下一间隔时长(秒)、子弹类型、子弹方向选择（1，手动方向，0，战机方向）、手动方向x、手动方向y//单信息长度
        double bulletData[6];
        for(;1;)//此循环决定该时刻该发射多少子弹
        {
            for(int i=0;i<6;i++)//此循环读取单颗子弹的发射数据
            {
                bulletData[i]=bulletDataVec.at(numcount*6+i);
            }
            if(bulletData[3]==1)//此if根据读取的数据判断如何发射
            {
                emit emitBullet5(bulletData[2],x()+pix.width()/2,y()+pix.height(),bulletData[4],bulletData[5]);
            }
            else {
                emit emitBullet3(bulletData[2],x()+pix.width()/2,y()+pix.height());
            }
            //一颗发射结束，准备下一颗（含检测
            numcount++;
            if(numcount==bulletAmount)
            {
                frequency=bulletData[1];
                numcount=0;
                //一组发完了重置等CD
                count=0;
                break;
            }
            if(bulletData[0]!=bulletDataVec.at(numcount*6))
            {
                frequency=bulletData[1];
                //一组发完了重置等CD
                count=0;
                break;
            }
        }

    }
}

void Enemy2::ini_bulletSetion(int num,double *sec)
{
    bulletAmount=num;
    for(int i=0;i<6*bulletAmount;i++)
    {
        bulletDataVec.push_back(sec[i]);
    }
}




