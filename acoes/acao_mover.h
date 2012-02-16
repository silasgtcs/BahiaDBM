#ifndef ACAO_MOVER_H
#define ACAO_MOVER_H

#include "acao.h"
#include "QList"
#include "QPointF"
#include "QGraphicsItem"

class AcaoMover : public Acao
{
private:
    QList<QGraphicsItem *> _items;
    QPointF _deslocamento;
public:
    AcaoMover(QList<QGraphicsItem *> selecionados, QPointF deslocamento);
    virtual void fazerAcao();
    virtual void desfazerAcao();
    virtual bool podeDesfazer() { return true; }
};

#endif // ACAO_MOVER_H
