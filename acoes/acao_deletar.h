#ifndef ACAO_DELETAR_H
#define ACAO_DELETAR_H

#include "acao.h"
#include "objetoremovivel.h"
#include <QtGui>
#include <QList>

class Texto;

class AcaoDeletar : public Acao
{
private:
    QList<QGraphicsItem *> items;
    QList<QGraphicsItem *> removidos;
    QGraphicsScene * scene;
    bool deleted;
public:
    AcaoDeletar(QGraphicsScene * scene, QList<QGraphicsItem *> selecionados);
    virtual void fazerAcao();
    virtual void desfazerAcao();
    virtual bool podeDesfazer() { return true; }
    virtual void dispose();
};

#endif // ACAO_DELETAR_H
