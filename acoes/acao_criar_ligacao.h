#ifndef ACAO_CRIAR_LIGACAO_H
#define ACAO_CRIAR_LIGACAO_H

#include "acao.h"
#include "ligacao.h"
#include "cardinalidade.h"
#include "texto.h"
#include <QtGui>
#include <QGraphicsItem>
#include <QGraphicsScene>

class AcaoCriarLigacao : public Acao
{
private:
    QGraphicsItem * item1, * item2;
    Poligono *cast1P, *cast2P;
    Ligacao * ligacao;
    QGraphicsScene * scene;

    //Optionas itens:
    Cardinalidade * cardinalidade;
    Texto * nomeLigacao1, * nomeLigacao2;
public:
    AcaoCriarLigacao(QGraphicsScene * scene, QGraphicsItem * item1, QGraphicsItem * item2);
    virtual void fazerAcao();
    virtual void desfazerAcao();
    virtual bool podeDesfazer() { return true; }
    Ligacao *getLigacao() { return this->ligacao; }
};

#endif // ACAO_CRIAR_LIGACAO_H
