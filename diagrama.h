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

    QGraphicsLineItem *novaLinha;
    QGraphicsRectItem * selecionarBox;

    QPointF pontoInicial;

    bool ativaSelecao;
    bool ativaMover;
};

#endif // DSCENE_H
