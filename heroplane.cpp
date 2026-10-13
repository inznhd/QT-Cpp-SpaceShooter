#include "heroplane.h"

HeroPlane::HeroPlane(int kind,int *bulletKinds,QObject *parent) : QObject(parent)//接受的子弹参数式新式的
{
    kinds=kind;
    state=2;
    setFlag(ItemIsFocusable,true);
    setFocus();
    setPos(300,400);
    ini_section(bulletKinds);
    //以下测试移动问题：
//    x=300;
//    y=400;
}

void HeroPlane::ini_section(int *sec)
{
    //初始化一些基本参数
    switch (kinds) {
    case 1:
    {
        health=50;
        ini_health=50;
        bulletNum=3;
        pix.load(":/hero/1.png");
        break;
    }
    case 2:
    {
        health=300;
        ini_health=300;
        bulletNum=5;
        pix.load(":/hero/2.png");
        break;
    }
    case 3:
    {
        health=500;
        ini_health=500;
        bulletNum=5;
        pix.load(":/hero/3.png");
        break;
    }
    case 4:
    {
        health=800;
        ini_health=800;
        bulletNum=7;
        pix.load(":/hero/4.png");
        break;
    }
    default:
    {
        qDebug()<<"战机型号出错，初始化失败";
        return;
    }
    }
    //加载图像非透明区域，后面作为碰撞边界
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

    //初始化不同弹舱的参数
    for(int i=0;i<bulletNum;i++)
    {
        bulletKind[i]=sec[(i+1)/2];//把新式的子弹参数转换成旧式的，其他代码不改了
        switch (bulletKind[i]) {
        case 1:
        {
            bulletfreq[i]=0.5;
            break;
        }
        case 2:
        {
            bulletfreq[i]=0.3;
            break;
        }
        case 3:
        {
            bulletfreq[i]=0.4;
            break;
        }
        case 4:
        {
            bulletfreq[i]=0.2;
            break;
        }
        case 5:
        {
            bulletfreq[i]=1;
            break;
        }
        case 6:
        {
            bulletfreq[i]=0.75;
            break;
        }
        case 7:
        {
            bulletfreq[i]=0.75;
            break;
        }
        case -1:
        {   bulletfreq[i]=99999;
            break;
        }
        default:
        {
            qDebug()<<"战机子弹频率初始化失败";
        }
        }
        freq_count[i]=0;
    }

}

QRectF HeroPlane::boundingRect() const
{
    return QRectF(0,0,pix.width(),pix.height());
}
//QRectF HeroPlane::boundingRect() const
//{
//    return QRectF(pix.width()/2-5,pix.height()/2-5,pix.width()/2+5,pix.height()/2+5);
//}

QPainterPath HeroPlane::shape() const
{
    return path;
}

void HeroPlane::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0,0,pix);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void HeroPlane::keyPressEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat())
    {
        keySet.insert(event->key());
    }
    QGraphicsItem::keyPressEvent(event);
}

void HeroPlane::keyReleaseEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat())
    {
        keySet.remove(event->key());
    }
    QGraphicsItem::keyReleaseEvent(event);
}

void HeroPlane::update()
{
    if(state==2)
    {
        updatePosition();
        updateAttack();
        detectCollision();
    }
}

void HeroPlane::updatePosition()
{
    setFocus();
    double v = speed;
    if(keySet.size()>1)
    {
        v/=1.41;
    }
    if(keySet.contains(Qt::Key_W))
    {
        setY(y()-v);
    }
    if(keySet.contains(Qt::Key_D))
    {
       setX(x()+v);
    }
    if(keySet.contains(Qt::Key_S))
    {
        setY(y()+v);
    }
    if(keySet.contains(Qt::Key_A))
    {
        setX(x()-v);
    }
    if(y()<0)
    {
        setY(0);
    }
    if(y()>1000-pix.height())
    {
        setY(1000-pix.height());
    }
    if(x()<0)
    {
        setX(0);
    }
    if(x()>750-pix.width())
    {
        setX(750-pix.width());
    }
}

//void HeroPlane::updatePosition()
//{
//    setFocus();
//    double speed = MyPlaneSpeed;
//    if(keySet.size()>1)
//    {
//        speed/=1.41;
//    }
//    if(keySet.contains(Qt::Key_W))
//    {
//        y-=speed;
//    }
//    if(keySet.contains(Qt::Key_D))
//    {
//        x+=speed;
//    }
//    if(keySet.contains(Qt::Key_S))
//    {
//        y+=speed;
//    }
//    if(keySet.contains(Qt::Key_A))
//    {
//        x-=speed;
//    }
//    if(y<0){y=0;}
//    if(y>722){y=722;}
//    if(x<0){x=0;}
//    if(x>522){x=522;}
//    setPos(x,y);
//}

void HeroPlane::updateAttack()
{
    for(int i=0;i<bulletNum;i++)
    {
        freq_count[i]+=0.01;
        if(freq_count[i]>=bulletfreq[i])
        {
            if(i<5)
            {
                emit emitHeroBullet(bulletKind[i],x()+pix.width()/2+((i+1)/2)*pow(-1,i)*30,y()+pix.height()/2);
                freq_count[i]=0;
            }
            else{
                emit emitHeroBullet5(bulletKind[i],x()+pix.width()/2+((i+1)/2)*pow(-1,i)*40,y()+pix.height()/2,pow(-1,i),-3);
                freq_count[i]=0;
            }
        }
    }
//    fre_count[0]+=0.01;
//    if(fre_count[0]>=frequemcy[0])
//    {
//        emit emitHeroBullet(0,x()+pix->width()/2,y());
//        fre_count[0]=0;
//    }
}

//留个接口给dialog绘制血条
double HeroPlane::getRestHealthRate()
{
    return health/ini_health;
}

//暴露一个接口给敌人子弹获取位置
QPointF HeroPlane::getPos()
{
    return QPointF(x(),y());

}

void HeroPlane::getHit(double reduce_hp)
{
    health-=reduce_hp;
    qDebug()<<health;
    if(health<=0)
    {
        setDeath();
    }

}

void HeroPlane::detectCollision()
{
    //获取子弹自身的边界矩形（转换到场景坐标）
    QRectF planeRect = mapToScene(boundingRect()).boundingRect();
    //检测于子弹矩形相交的对象
    QList<QGraphicsItem *> collidingList = scene()->items(planeRect,Qt::IntersectsItemShape);
    //强制转换成敌机，成功则继续//筛选出敌机目标
    foreach(QGraphicsItem *item, collidingList)
    {
        if(EnemyPlaneBase* enyB = dynamic_cast<EnemyPlaneBase *>(item))
        {
            Q_UNUSED(enyB);
            setDeath();
            break;
        }
    }
}

void HeroPlane::setDeath()
{
    emit heroPDie();
    state=4;
    emit heroPBoom(1,x(),y());
    setPos(-500,-500);
}

void HeroPlane::clearKeySet()
{
    keySet.clear();
}
