#include "acao_mover.h"
#include "../poligono.h"

AcaoMover::AcaoMover(QList<QGraphicsItem *> selecionados, QPointF deslocamento)
{
    _items = selecionados;
    foreach(QGraphicsItem * item, _items)
    {
        QGraphicsItem * parent = item->parentItem();
        while(parent)
        {
            if(_items.contains(parent))
            {
                _items.removeOne(item);
                break;
            }

            parent = parent->parentItem();
        }
    }

    _deslocamento = deslocamento;
}

void AcaoMover::desfazerAcao()
{
    foreach(QGraphicsItem * item, _items)
        if(item->type() == Poligono::Type)
            item->moveBy( - _deslocamento.x(), - _deslocamento.y());
}

void AcaoMover::fazerAcao()
{
    foreach(QGraphicsItem * item, _items)
        if(item->type() == Poligono::Type)
            item->moveBy(_deslocamento.x(), _deslocamento.y());
}
