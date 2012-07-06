#ifndef ACAO_CRIAR_LIGACAO_H
#define ACAO_CRIAR_LIGACAO_H

#include "acao.h"
#include "ligacao.h"
#include "cardinalidade.h"
#include "texto.h"
#include "tabela.h"
#include <QtGui>
#include <QGraphicsItem>
#include <QGraphicsScene>

class AcaoCriarLigacao : public Acao
{
private:
    QGraphicsItem * item1, * item2;
    Poligono *cast1P, *cast2P;
    Tabela *cast1T, *cast2T;
    Ligacao * ligacao;
    QGraphicsScene * scene;

    //Optionas itens:
    Cardinalidade * cardinalidade;
public:
    AcaoCriarLigacao(QGraphicsScene * scene, QGraphicsItem * item1, QGraphicsItem * item2);
    virtual void fazerAcao();
    virtual void desfazerAcao();
    virtual bool podeDesfazer() { return true; }
    Ligacao *getLigacao() { return this->ligacao; }
    virtual void dispose();
};

#endif // ACAO_CRIAR_LIGACAO_H
