/**
 * @file ganttchartoperation.h
 * This file defines how to present a operation on the m=gantt chart.
 *
 * @author Name1e5s
 */

#ifndef GANTTCHARTOPERATION_H
#define GANTTCHARTOPERATION_H

#include <QGraphicsRectItem>
#include <QString>

class GanttChartOperation : public QGraphicsRectItem
{
    Q_INTERFACES(QGraphicsItem)

public:
    GanttChartOperation(const QString& id, int time,QColor color);
    ~GanttChartOperation();

    void setTime(int time);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QString m_id;
    QColor m_color;
};
#endif // GANTTCHARTOPERATION_H
