/**
 * @file ganttchartopertaion.cpp
 * Draw operation for our gantt chart.
 *
 * @author Name1e5s
 */

#include <ganttchartoperation.h>
#include <ganttchartbase.h>
#include <cstdlib>
#include <QDebug>
#include <QPainter>

GanttChartOperation::GanttChartOperation(const QString& id, int time, QColor color) :
    QGraphicsRectItem(nullptr),
    m_color(color)
{
    m_id = id;
    this->setRect(0, 0, time * GanttChartBase::widthUnit, GanttChartBase::operationHeight);
}

GanttChartOperation::~GanttChartOperation()
{
}

void GanttChartOperation::setTime(int time)
{
    this->setRect(0, 0, time * GanttChartBase::widthUnit, GanttChartBase::operationHeight);
}

void GanttChartOperation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QFont font("Helvetica", 10, QFont::Bold, false);
    QPen pen;
    pen.setWidth(0);

    const QRectF& rect = this->rect();
    painter->setPen(pen);
    painter->setBrush(QBrush(m_color));
    painter->drawRect(rect);

    painter->setFont(font);
    painter->drawText(rect, Qt::AlignCenter, m_id);
}
