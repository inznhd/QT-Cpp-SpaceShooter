#ifndef ICON_H
#define ICON_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QtDebug>

class Icon : public QObject , public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Icon(int kinds,QObject *parent);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:

    QPixmap pix;
};

#endif // ICON_H
