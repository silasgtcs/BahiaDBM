/*Utilizada para criar a interface que permite ao usuário modificar os nomes de todos os poligonos e atributos além
de ser utilizada para definir os tipos de generalização/especialização.*/

#ifndef TEXTO_H
#define TEXTO_H

#include <QtGui>
#include <dialog.h>
#include <atributo.h>
//#include "atributo.h"

class Atributo;

struct Restricao
{
    QString nome;
    int pos;
    bool alterarRestricao;
    bool nulo;
};

class Texto : public QGraphicsTextItem
{
    Q_OBJECT

public:
    Texto(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    void setCount( int countR ) { this->count = countR; }
    int getCount() { return count; }

    void setTipoGenEsp( QString tipo ) { this->tipoGenEsp = tipo; }
    QString getTipoGenEsp () { return this->tipoGenEsp; }

    void setTextoTabelaLogico( QString txt, int pos, bool alterarRestricao, bool nulo );
    Restricao getTextoTabelaLogico() { return this->textoTabelaLogico; }

    bool getGenEspAtiva() { return this->genEspAtiva; }
    void setGenEspAtiva( bool gea );

    void setTipoOwner(int t) { tipoOwner = t; };

    bool getTabelaAtiva() { return this->tabelaAtiva; }
    void setTabelaAtiva( bool tla ) { this->tabelaAtiva = tla; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void focusOutEvent(QFocusEvent *event);
    void focusInEvent(QFocusEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    QList<Texto *> listaTextos;

signals:
    void lostFocus(Texto *item);
    void selectedChange(QGraphicsItem *item);
    void textoTabelaLogicoAlterado(int);


private:
    int count;
    Dialog chamarTelaTipoGenEsp;
    Dialog chamarTelaLogico;

    int tipoOwner; // 1 = atributo, 2 = atributo ID, 3 = entidade, 4 = ent. associativa
                   // 5 = relacionamento, 6 = generalizacao/especializacao

    QString tipoGenEsp;
    Restricao textoTabelaLogico;

    bool genEspAtiva;
    bool tabelaAtiva;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // TEXTO_H
