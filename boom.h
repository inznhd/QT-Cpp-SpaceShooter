#ifndef BOOM_H
#define BOOM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPixmap>
#include <QTimer>
#include <QString>
#include <QPainter>
#include <QDebug>

class Boom : public QObject , public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Boom(int kind,QObject *parent,double x=-300,double y=-300 );
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    int getState();
    void upDate();
    void reUse(double x,double y);
signals:

private:
    void ini_set();
    int kinds;      //0是子弹，1是飞机
    QPixmap *pix;
    int totalnum;
    int state;//0表示空闲，2表示播放中
    int nowpix;

};

#endif // BOOM_H
