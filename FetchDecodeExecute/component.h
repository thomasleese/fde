#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <QGraphicsItem>

class Component : public QGraphicsItem {
            
public:
    explicit Component(QString name, int x, int y, int hue, QGraphicsItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    
    QColor getColour() const;
    
    void setRawValue(QString value);
    void setDecValue(int value);
    void setHexValue(int value);
    
    QString getRawValue() const;
    int getDecValue() const;
    int getHexValue() const;
    
    void setActive(bool active);
    
private:
    int mHue;
    bool mActive;
    QString mName, mValue;
    
};

#endif // COMPONENTS_H
