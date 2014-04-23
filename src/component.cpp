#include <QPainter>
#include <QDebug>

#include "component.h"

const static int WIDTH = 180;
const static int HEIGHT = 180;


class ComponentPrivate {
public:
    int hue;
    bool active;
    QString name, value;
};


Component::Component(QString name, int x, int y, int hue, QGraphicsItem *parent) :
    QGraphicsItem(parent), d(new ComponentPrivate) {
    setPos(x, y);
    d->hue = hue;
    d->name = name;
    d->value = "NULL";
    d->active = false;
}

Component::~Component() {
    delete d;
}

void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget) {
    painter->fillRect(boundingRect(), getColour());

    painter->setFont(QFont("Monospace", 16, 200));
    painter->setPen(QColor(Qt::white));
    painter->drawText(boundingRect().adjusted(4, 4, -4, -HEIGHT / 2),
                      Qt::AlignCenter | Qt::TextWordWrap, d->name);

    painter->setFont(QFont("Monospace", 12, 200));
    painter->setPen(QColor(Qt::white));
    painter->drawText(boundingRect().adjusted(4, HEIGHT / 2, -4, -4),
                      Qt::AlignCenter, d->value);

    if (d->active) {
        painter->setPen(QPen(QBrush(Qt::black), 4));
        painter->drawRect(boundingRect());
    }
}

QRectF Component::boundingRect() const {
    return QRectF(x(), y(), WIDTH, HEIGHT);
}

QColor Component::getColour() const {
    return QColor::fromHsl(d->hue, 100, 60);
}

void Component::setRawValue(QString value) {
    d->value = value;
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
    return d->value;
}

int Component::getDecValue() const {
    return getRawValue().toInt(0, 10);
}

int Component::getHexValue() const {
    return getRawValue().toInt(0, 16);
}

void Component::setActive(bool active) {
    d->active = active;
    update();
}
