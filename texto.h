#ifndef TEXTO_H
#define TEXTO_H

#include <QtGui>

class Texto : public QGraphicsTextItem
{
public:
    Texto(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void focusOutEvent(QFocusEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void setCount( int countR ) { this->count = countR; }
    int getCount() { return count; }

private:
    int count;
};

#endif // TEXTO_H
