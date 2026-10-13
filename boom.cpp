#include "boom.h"

Boom::Boom(int kind,QObject *parent,double x,double y) : QObject(parent)
{
    kinds=kind;
    ini_set();
    nowpix=0;
    setPos(x,y);
}

void Boom::ini_set()
{
    if(x()==-300&&y()==-300)
    {
        state=0;
    }
    else
    {
        state=2;
    }
    switch (kinds) {
    case 0:
    {
        totalnum=7;
        pix = new QPixmap[7];
        for(int i=0;i<7;i++)
        {
            QString path=":/bomb/bomb";
            path+=(QString::number(i));
            path+=".png";
            pix[i].load(path);
        }
        break;
    }
    case 1:
    {
        totalnum=32;
        pix = new QPixmap[32];
        for(int i=0;i<32;i++)
        {
            QString path=":/bomb/Klight_";
            path+=(QString::number(i));
            path+=".png";
            pix[i].load(path);
        }
        break;
    }
    }
}

QRectF Boom::boundingRect() const
{
    return QRectF(0,0,pix[nowpix].width(),pix[nowpix].height());
}

void Boom::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(state==2)
    {
        painter->drawPixmap(0,0,pix[nowpix]);
    }
        Q_UNUSED(option);
        Q_UNUSED(widget);
}

void Boom::upDate()
{
    if(state==2)
    {
        nowpix++;
        if(nowpix>=totalnum)
        {
            nowpix=0;
            state=0;
        }
        update();
    }
}

void Boom::reUse(double x, double y)
{
    setPos(x,y);
    state=2;
}

int Boom::getState()
{
    return state;
}
