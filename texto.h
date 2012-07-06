/*Utilizada para criar a interface que permite ao usuário modificar os nomes de todos os poligonos e atributos além
de ser utilizada para definir os tipos de generalização/especialização.*/

#ifndef TEXTO_H
#define TEXTO_H

#include <QtGui>
#include <dialog.h>

struct restricao
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
    restricao getTextoTabelaLogico() { return this->textoTabelaLogico; }

    bool getGenEspAtiva() { return this->genEspAtiva; }
    void setGenEspAtiva( bool gea );

    bool getTabelaLogicoAtiva() { return this->tabelaLogicoAtiva; }
    void setTabelaLogicoAtiva( bool tla ) { this->tabelaLogicoAtiva = tla; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void focusOutEvent(QFocusEvent *event);
    void focusInEvent(QFocusEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void lostFocus(Texto *item);
    void selectedChange(QGraphicsItem *item);
    void textoTabelaLogicoAlterado(int);

private:
    int count;
    Dialog chamarTelaTipoGenEsp;
    Dialog chamarTelaLogico;

    QString tipoGenEsp;
    restricao textoTabelaLogico;

    bool genEspAtiva;
    bool tabelaLogicoAtiva;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // TEXTO_H
