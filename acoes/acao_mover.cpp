#include "acao_mover.h"
#include "../poligono.h"

AcaoMover::AcaoMover(QList<QGraphicsItem *> selecionados, QPointF deslocamento)
{
    _items = selecionados;
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
