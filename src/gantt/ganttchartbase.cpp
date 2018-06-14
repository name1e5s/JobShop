/**
 * @file ganttchartbase.cpp
 * Basic definition of our gantt chart.
 *
 * @author Name1e5s
 */

#include <ganttchartbase.h>
#include <bottle.h>
#include <algorithm>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsLineItem>
#include <QFont>
#include <QPen>
#include <QtSvg/QSvgGenerator>
#include <QPainter>
#include <QtDebug>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>

/**
 * @brief Construct a gantt chart.
 * @param makespan The makespan of this project.
 */
GanttChartBase::GanttChartBase(int makespan) :
    QGraphicsItem(nullptr),
    makespan(makespan)
{
    QGraphicsLineItem* line;
    QGraphicsSimpleTextItem* text;
    QFont font("Arial", 12);
    double x;
    double max_y = machineHeight * machine_size  + operationHeight;

    x = machineHorizontalOffset + makespan * widthUnit;
    line = new QGraphicsLineItem(x, operationHeight, x, max_y, this);
    line->setPen(QPen(Qt::magenta));
    text = new QGraphicsSimpleTextItem(QString("makespan = %1").arg(makespan), this);
    text->setFont(font);
    text->setX(x+3);
    text->setY(max_y-15);
}

QRectF GanttChartBase::boundingRect() const
{
    return this->childrenBoundingRect();
}

/**
 * @brief Paint this gantt chart.
 * @param painter
 * @param option
 * @param widget
 */
void GanttChartBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
