#include "atributo.h"
#include "poligono.h"

Atributo::Atributo( Tipo tipo, QGraphicsItem *parent, QGraphicsScene *scene ) : QGraphicsEllipseItem( parent, scene )
{
    this->tipo = tipo;
    circ = new QRectF(0,0,15,15);
    setRect(*circ);

    setConectado(false);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

QVariant Atributo::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if ((change == QGraphicsItem::ItemPositionChange) || ( change == QGraphicsItem::ItemPositionHasChanged ))
        emit posicaoAlterada();

    return value;
}

void Atributo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    switch(tipo)
    {
        case(atributo):
            painter->setBrush(Qt::white);
            break;
        case(atributo_identif):
            painter->setBrush(Qt::black);
            break;
    }

    if ( isSelected() )
        painter->setPen(QPen(Qt::red, 2, Qt::DashLine));
    painter->drawEllipse(*circ);
}
