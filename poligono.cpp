#include "poligono.h"
#include <cardinalidade.h>

Poligono::Poligono(Tipo tipo, bool pai, QGraphicsItem *parent, QGraphicsScene *scene )
    : QGraphicsPolygonItem( parent, scene )
{
    this->tipo = tipo;

    switch(tipo)
    {
        case(entidade):
            poligono << QPointF(-70, -35) << QPointF(70, -35)
                     << QPointF(70, 35) << QPointF(-70, 35)
                     << QPointF(-70, -35);
            break;

        case(relacionamento):
            poligono << QPointF(-70, 0) << QPointF(0, 30)
                     << QPointF(70, 0) << QPointF(0, -30)
                     << QPointF(-70, 0);
            break;

        case(gen_esp):
            poligono << QPointF(0, -16) << QPointF(-32, 16)
                     << QPointF(32, 16);
            break;

        case ent_associativa:
            break;
    }
    poligonoPai = NULL;
    poligonoFilho = NULL;

    setPolygon(poligono);
    setConectado(false);
    setAutoRelacionamento(false);
    setEntidadeFraca(false);

    setFlag(QGraphicsItem::ItemIsMovable, pai);
    setFlag(QGraphicsItem::ItemIsSelectable, pai);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

QVariant Poligono::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if ((change == QGraphicsItem::ItemPositionChange) || ( change == QGraphicsItem::ItemPositionHasChanged ))
    {
        //printf("%lf %lf\n", this->scenePos().x(), this->y());
        emit posicaoAlterada();
        emit posicaoAlterada(value.toPointF());
    }

    return value;
}

void Poligono::atualizaItem()
{
    setPos(x()+0.1, y()+0.1);
    setPos(x()-0.1, y()-0.1);
}

void Poligono::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(Qt::white);
    if ( isSelected() )
        painter->setPen(QPen(Qt::red, 2, Qt::DashLine));
    painter->drawPolygon(poligono);
}

void Poligono::acompanharObjeto(const QPointF objNewPos)
{
    if(pos() != objNewPos)
        setPos(objNewPos);
}

void Poligono::remover()
{
    scene()->removeItem(this);
}

void Poligono::itemRemovido()
{
    emit removido();
}

void Poligono::doRemove(bool value)
{
    foreach(Ligacao * ligacao, linhas_associadas)
        ligacao->doRemove(value);
}

QList<QGraphicsItem *> Poligono::getToDelete()
{
    QList<QGraphicsItem *> toDelete;
    foreach(Ligacao * lig, linhas_associadas) {
        toDelete.push_back(lig);
        foreach(QGraphicsItem * subitem, lig->getToDelete()) {
            toDelete.push_back(subitem);
        }
    }
    if(poligonoFilho) {
        toDelete.push_back(poligonoFilho);
        foreach(Ligacao * lig, poligonoFilho->getLinhasAssociadas()) {
            toDelete.push_back(lig);
            foreach(QGraphicsItem * subitem, lig->getToDelete()) {
                toDelete.push_back(subitem);
            }
        }
    }
    if(poligonoPai) {
        toDelete.push_back(poligonoPai);
        foreach(Ligacao * lig, poligonoPai->getLinhasAssociadas()) {
            toDelete.push_back(lig);
            foreach(QGraphicsItem * subitem, lig->getToDelete()) {
                toDelete.push_back(subitem);
            }
        }
    }

    return toDelete;
}
