/**
 * @file ganttchartmachine.cpp
 * Draw machine for our gantt chart.
 *
 * @author Name1e5s
 */

#include <QFont>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QPainter>
#include <QPen>

#include <arrow.h>
#include <ganttchartbase.h>
#include <ganttchartmachine.h>

/**
 * @brief Construct a machine class.
 * @param id The given machine id.
 * @param parent
 */
GanttChartMachine::GanttChartMachine(const QString &id, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    machine_num(id),
    makespan(0)
{
    QGraphicsTextItem* text = new QGraphicsTextItem(machine_num, this);
    text->setPos(0, GanttChartBase::machineHeight/2);
    text->setFont(QFont("Arial", 12, QFont::Normal, false));


    QGraphicsLineItem* line = new QGraphicsLineItem(GanttChartBase::machineHorizontalOffset, 0, GanttChartBase::machineHorizontalOffset, GanttChartBase::machineHeight, this);
    QPen pen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    line->setPen(pen);

    private_arrow = new Arrow(this);
    private_arrow->setPen(pen);
}

GanttChartMachine::~GanttChartMachine()
{
}

QRectF GanttChartMachine::boundingRect() const
{
    const QPointF& p = private_arrow->boundingRect().bottomRight();
    return QRectF(0, 0, p.x(), p.y());
}

/**
 * @brief Paint this machine.
 * @param painter
 * @param option
 * @param widget
 */
void GanttChartMachine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

/**
 * @brief Set makespan of this machine.
 * @param cMax The makespan value.
 */
void GanttChartMachine::setMakespan(int cMax)
{
    makespan = cMax;

    QPointF p(0, GanttChartBase::machineHeight);
    private_arrow->setStartPoint(p);
    p.rx() += GanttChartBase::machineHorizontalOffset + (makespan + 4) * GanttChartBase::widthUnit;
    private_arrow->setEndPoint(p);
}
