#include "icon.h"


Icon::Icon(int kinds,QObject *parent) : QObject(parent)
{
    switch (kinds) {
    case 0:
    {
        pix.load(":/icon/stopicon.png");
        setPos(25,25);
        break;
    }
    case 1:
    {
        pix.load(":/icon/continueicon.png");
        setPos(25,25);
        break;
    }
    case 2:
    {
        pix.load(":/icon/againicon.png");
        setPos(75,25);
        break;
    }
    default:
    {
        qDebug()<<"icon构建传参错误";
    }
    }
}

QPainterPath Icon::shape() const  {
    return QPainterPath();  // 返回空路径，禁用碰撞检测
}

QRectF Icon::boundingRect()const
{
    return QRectF(0,0,pix.width(),pix.height());
}

void Icon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(0,0,pix);
}
