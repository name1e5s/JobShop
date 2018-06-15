/**
 * @file ganttchartopertaion.cpp
 * Draw operation for our gantt chart.
 *
 * @author Name1e5s
 */

#include <QDebug>
#include <QPainter>
#include <cstdlib>

#include <ganttchartoperation.h>
#include <ganttchartbase.h>

/**
 * @brief Construct a GanttChartOperation class
 * @param id Job id.
 * @param time Start time.
 * @param color Color of this operation.
 */
GanttChartOperation::GanttChartOperation(const QString& id, int time, QColor color) :
    QGraphicsRectItem(nullptr),
    privateColor(color)
{
    privateId = id;
    this->setRect(0, 0, time * GanttChartBase::widthUnit, GanttChartBase::operationHeight);
}

/**
 * @brief Paint this operation.
 * @param painter
 * @param option
 * @param widget
 */
void GanttChartOperation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QFont font("Microsoft YaHei UI", 10, QFont::Bold, false);
    QPen pen;
    pen.setWidth(0);

    const QRectF& rect = this->rect();
    painter->setPen(pen);
    painter->setBrush(QBrush(privateColor));
    painter->drawRect(rect);

    painter->setFont(font);
    painter->drawText(rect, Qt::AlignCenter, privateId);
}
