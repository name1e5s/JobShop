/**
 * @file arrow.h
 * This file defines how to draw an arrow on thegantt chart.
 *
 * @author Name1e5s
 */

#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class Arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 1 };
    virtual int type() const
        { return Type; }

    explicit Arrow(QGraphicsItem *parent = 0);

    Arrow(const QPointF& startPoint, const QPointF& endPoint, QGraphicsItem *parent = 0);

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;

    QPointF startPoint() const;
    void setStartPoint(const QPointF &startPoint);

    QPointF endPoint() const;
    void setEndPoint(const QPointF &endPoint);

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    QPointF private_startPoint;
    QPointF private_endPoint;
    QPolygonF private_arrowHead;
};

#endif
