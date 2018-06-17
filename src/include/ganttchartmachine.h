/**
 * @file ganttchartmachine.h
 * This file defines how to present the gantt chart per machine.
 *
 * @author Name1e5s
 */

#ifndef MACHINE_H
#define MACHINE_H

#include <QGraphicsItem>
#include <encoding.h>

class Arrow;

class GanttChartMachine : public QGraphicsItem
{
    Q_INTERFACES(QGraphicsItem)

public:
    GanttChartMachine(const QString& id, QGraphicsItem *parent = 0);
    ~GanttChartMachine();

    virtual QRectF boundingRect() const;

    void setMakespan(int cMax);

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    int makespan;
    QString machineNum;
    Arrow* privateArrow;
};

#endif // MASZYNA_H
