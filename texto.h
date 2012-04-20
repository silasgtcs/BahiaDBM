/*Utilizada para criar a interface que permite ao usuário modificar os nomes de todos os poligonos e atributos além
de ser utilizada para definir os tipos de generalização/especialização.*/

#ifndef TEXTO_H
#define TEXTO_H

#include <QtGui>
#include <dialog.h>

class Texto : public QGraphicsTextItem
{
    Q_OBJECT

public:
    Texto(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    void setCount( int countR ) { this->count = countR; }
    int getCount() { return count; }

    void setTipoGenEsp( QString tipo ) { this->tipoGenEsp = tipo; }
    QString getTipoGenEsp () { return this->tipoGenEsp; }

    bool getGenEspAtiva() { return this->genEspAtiva; }
    void setGenEspAtiva( bool gea ) { this->genEspAtiva = gea; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void focusOutEvent(QFocusEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void lostFocus(Texto *item);
    void selectedChange(QGraphicsItem *item);

private:
    int count;
    Dialog chamarTelaTipoGenEsp;
    QString tipoGenEsp;
    bool genEspAtiva;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // TEXTO_H
