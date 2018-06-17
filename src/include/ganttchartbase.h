/**
 * @file ganttchartbase.h
 * This file contains the base class of the gantt chart.
 *
 * @author Name1e5s
 */

#ifndef GANTTCHARTBASE_H
#define GANTTCHARTBASE_H

#include <QGraphicsItem>

#include <encoding.h>
class GanttChartBase:public QGraphicsItem
{
    Q_INTERFACES(QGraphicsItem)
public:
    GanttChartBase(int makespan);

    static const int widthUnit = 10;
    static const int operationHeight = 20;
    static const int machineHeight = operationHeight * 3;
    static const int machineHorizontalOffset = 35;

    static QPointF operationPosition(int time)
    { return QPointF(time * widthUnit, 0); }

    static QPointF machineOffset()
    { return QPointF(machineHorizontalOffset, machineHeight - operationHeight - 1); }

    virtual QRectF boundingRect() const;

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    int makespan;
};

#endif // GANTTCHARTBASE_H
