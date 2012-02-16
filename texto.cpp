#include "texto.h"

Texto::Texto(QGraphicsItem *parent, QGraphicsScene *scene) : QGraphicsTextItem(parent, scene)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//<<<<<<< .mine
//void texto::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//=======
void Texto::focusOutEvent(QFocusEvent *event)
//>>>>>>> .r14
{
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);
    setTextInteractionFlags(Qt::NoTextInteraction);
    QGraphicsTextItem::focusOutEvent(event);
}


void Texto::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //setTextInteractionFlags(Qt::TextEditorInteraction);
    painter->setBrush(Qt::blue);
    if ( isSelected() )
        painter->setPen(QPen(Qt::red, 2, Qt::DashLine));

    //painter->drawText(0,0, QString("R%1").arg(getCount()));
    QGraphicsTextItem::paint(painter, option, widget);
}

void Texto::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseReleaseEvent(event);
}

