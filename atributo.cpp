#include "atributo.h"
#include "poligono.h"
#include "ligacao.h"

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

void Atributo::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
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

void Atributo::doRemove(bool value)
{

    foreach(Ligacao * ligacao, linhaAssociada)
        ligacao->doRemove(value);
}

QList<QGraphicsItem *> Atributo::getToDelete()
{
    int i, tam;
    Texto aux;

    tam = aux.listaTextos.size(); // deleta Texto da lista de textos existentes
    for (i=0; i<tam; i++)
    {
        if (aux.listaTextos.at(i)->parentItem() == this)
        {
            aux.listaTextos.removeAt(i);
            break;
        }
    }

    QList<QGraphicsItem *> toDelete;
    foreach(Ligacao * lig, linhaAssociada) {
        toDelete.push_back(lig);

        foreach(QGraphicsItem * subitem, lig->getToDelete()) {
            toDelete.push_back(subitem);
        }
    }
    return toDelete;
}
