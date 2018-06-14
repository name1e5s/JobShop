/**
 * @file arrow.cpp
 * Draw arrow.
 *
 * @author Name1e5s
 */

#include <arrow.h>

#include <QtGui>
#define USE_MATH_DEFINES
#include <cmath>

const qreal Pi = M_PI;
const qreal arrowSize = 10;

/**
 * @brief Creat an arrow.
 * @param parent
 */
Arrow::Arrow(QGraphicsItem *parent) :
    QGraphicsLineItem(parent)
{
    setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

/**
 * @brief Creat an arrow : The harder version.
 * @param startPoint
 * @param endPoint
 * @param parent
 */
Arrow::Arrow(const QPointF &startPoint, const QPointF &endPoint, QGraphicsItem *parent) :
    QGraphicsLineItem(parent)
{
    private_startPoint = startPoint;
    private_endPoint = endPoint;

    setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

QRectF Arrow::boundingRect() const {
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath Arrow::shape() const {
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(private_arrowHead);
    return path;
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    QColor myColor;
    if(this->parentItem()->isSelected())
        myColor = QColor::fromRgb(0xFF8800);
    else
    {
        QPalette p;
        myColor = p.windowText().color();
    }

    QPen myPen = pen();
    myPen.setColor(myColor);
    painter->setPen(myPen);
    painter->setBrush(myColor);

    setLine(QLineF(private_endPoint, private_startPoint));

    double angle = ::acos(line().dx() / line().length());
    if (line().dy() >= 0)
        angle = (Pi * 2) - angle;

    QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize, cos(angle + Pi / 3) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize, cos(angle + Pi - Pi / 3) * arrowSize);

    private_arrowHead.clear();
    private_arrowHead << line().p1() << arrowP1 << arrowP2;

    painter->drawLine(line());
    painter->drawPolygon(private_arrowHead);
}
QPointF Arrow::endPoint() const{
    return private_endPoint;
}

void Arrow::setEndPoint(const QPointF &endPoint){
    private_endPoint = endPoint;
}

QPointF Arrow::startPoint() const{
    return private_startPoint;
}

void Arrow::setStartPoint(const QPointF &startPoint) {
    private_startPoint = startPoint;
}

