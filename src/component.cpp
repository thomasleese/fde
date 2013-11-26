#include <QPainter>
#include <QDebug>

#include "component.h"

const static int WIDTH = 180;
const static int HEIGHT = 180;

Component::Component(QString name, int x, int y, int hue, QGraphicsItem *parent) :
    QGraphicsItem(parent) {
    setPos(x, y);
    mHue = hue;
    mName = name;
    mValue = "NULL";
    mActive = false;
}

void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->fillRect(boundingRect(), getColour());
    
    painter->setFont(QFont("Monospace", 16, 200));
    painter->setPen(QColor(Qt::white));
    painter->drawText(boundingRect().adjusted(4, 4, -4, -HEIGHT / 2), Qt::AlignCenter | Qt::TextWordWrap, mName);
    
    painter->setFont(QFont("Monospace", 12, 200));
    painter->setPen(QColor(Qt::white));
    painter->drawText(boundingRect().adjusted(4, HEIGHT / 2, -4, -4), Qt::AlignCenter, mValue);
    
    if (mActive) {
        painter->setPen(QPen(QBrush(Qt::black), 4));
        painter->drawRect(boundingRect());
    }
}

QRectF Component::boundingRect() const {
    return QRectF(x(), y(), WIDTH, HEIGHT);
}

QColor Component::getColour() const {
    return QColor::fromHsl(mHue, 100, 60);
}

void Component::setRawValue(QString value) {
    mValue = value;
    update();
}

void Component::setDecValue(int value) {
    setRawValue(QString::number(value, 10));
}

void Component::setHexValue(int value) {
    QString s = QString::number(value, 16);
    if (s.length() == 1) {
        setRawValue("0x0" + s);
    } else {
        setRawValue("0x" + s);
    }
}

QString Component::getRawValue() const {
    return mValue;
}

int Component::getDecValue() const {
    return getRawValue().toInt(0, 10);
}

int Component::getHexValue() const {
    return getRawValue().toInt(0, 16);
}

void Component::setActive(bool active) {
    mActive = active;
    update();
}
