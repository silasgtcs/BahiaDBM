#ifndef ACAO_CRIAR_ATRIBUTO_H
#define ACAO_CRIAR_ATRIBUTO_H

#include "acao.h"
#include "atributo.h"
#include <QtGui>
#include <QGraphicsScene>

class AcaoCriarAtributo : public Acao
{
private:
    Atributo * atributo;
    QPointF posicao;
    QGraphicsScene * scene;
public:
    AcaoCriarAtributo(QGraphicsScene * scene, Atributo::Tipo tipo, QString nome, QPointF posicao);
    virtual void fazerAcao();
    virtual void desfazerAcao();
    virtual bool podeDesfazer() { return true; }
};

#endif // ACAO_CRIAR_ATRIBUTO_H
