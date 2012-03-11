/*Tanto diagrama.h quanto diagramaview.h são utilizados para fornecer uma interface para que as figuras geometricas
utilizadas nessa ferramenta sejam criadas.*/

#ifndef DSCENE_H
#define DSCENE_H

#include <QtGui>
#include "poligono.h"
#include "atributo.h"
#include "ligacao.h"
#include "texto.h"
#include "acoespilha.h"
#include <cardinalidade.h>

class Diagrama : public QGraphicsScene
{
    Q_OBJECT

public:
    Diagrama(QObject *parent = 0);
    enum TipoER { entidade, relacionamento, gen_esp, ent_associativa, atributo, atributo_ident, linha, mouse };
    void setDesfazer( QGraphicsItem *item );
    QStack<QGraphicsItem*> getDesfazerItem() { return this->desfazerItem; }
    void popDesfazerItem() { this->desfazerItem.pop(); }
    void popRefazerItem() { this->refazerItem.pop(); }

public slots:
    TipoER getTipoER() { return tipoER; }
    void setTipoER( TipoER tipo ) { this->tipoER = tipo; }

signals:
    void itemInserido();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    int countRelacionamento;
    int countEntidade;
    int countAtributo;
    int countAtributoIdentificador;
    int countEntidadeAssociativa;

    AcoesPilha * pilhaDeAcoes;

    TipoER tipoER;

    Poligono *poli;
    Poligono *poli2;
    Atributo *atr;
    Poligono *cast1P;
    Poligono *cast2P;
    Atributo *castA;
    Cardinalidade *insereCardinalidade;

    QGraphicsLineItem *novaLinha;
    QGraphicsLineItem *Lselecionar;
    QGraphicsRectItem *Rselecionar;

    QPointF pontoInicial;

    QGraphicsItem *item1;
    QGraphicsItem *item2;

    Ligacao *lig;
    Ligacao *cast_linha;

    Texto *nomeRelacionamento;
    Texto *nomeEntidade;
    Texto *nomeAtributo;
    Texto *nomeAtributoIdentificador;
    Texto *nomeEntidadeAssociativa;
    Texto *nomeLigacao1;
    Texto *nomeLigacao2;
    Texto *card;
    Texto *tipoGenEsp;

    bool ativaSelecao;
    bool ativaMover;

    QStack<QGraphicsItem*> desfazerItem;
    QStack<QGraphicsItem*> refazerItem;
};

#endif // DSCENE_H
