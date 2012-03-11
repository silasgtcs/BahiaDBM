#ifndef ACAO_CRIAR_POLIGONO_H
#define ACAO_CRIAR_POLIGONO_H

#include "acao.h"
#include "poligono.h"
#include <QtGui>
#include <QGraphicsScene>

class AcaoCriarPoligono : public Acao
{
private:
    Poligono * mainPol;
    Poligono * childPol;
    QPointF posicao;
    QGraphicsScene * scene;
public:
    AcaoCriarPoligono(QGraphicsScene * scene, Poligono::Tipo tipo, QString nome, QPointF posicao);
    virtual void fazerAcao();
    virtual void desfazerAcao();
    virtual bool podeDesfazer() { return true; }
    Poligono * getPoligono() { return this->mainPol; }
};

#endif // ACAO_CRIAR_POLIGONO_H
