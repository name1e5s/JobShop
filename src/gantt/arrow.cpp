/**
 * @file arrow.cpp
 * Draw arrow.
 *
 * @author Name1e5s
 */

#include <QtGui>

#define USE_MATH_DEFINES
#include <cmath>

#include <arrow.h>

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
    privateStartpoint = startPoint;
    privateEndpoint = endPoint;

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
    path.addPolygon(privateArrowhead);
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

    setLine(QLineF(privateEndpoint, privateStartpoint));

    double angle = ::acos(line().dx() / line().length());
    if (line().dy() >= 0)
        angle = (Pi * 2) - angle;

    QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize, cos(angle + Pi / 3) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize, cos(angle + Pi - Pi / 3) * arrowSize);

    privateArrowhead.clear();
    privateArrowhead << line().p1() << arrowP1 << arrowP2;

    painter->drawLine(line());
    painter->drawPolygon(privateArrowhead);
}
QPointF Arrow::endPoint() const{
    return privateEndpoint;
}

void Arrow::setEndPoint(const QPointF &endPoint){
    privateEndpoint = endPoint;
}

QPointF Arrow::startPoint() const{
    return privateStartpoint;
}

void Arrow::setStartPoint(const QPointF &startPoint) {
    privateStartpoint = startPoint;
}

