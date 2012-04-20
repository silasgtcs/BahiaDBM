#ifndef TABELA_H
#define TABELA_H

#include <QtGui>
#include "texto.h"

class Tabela : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Tabela( QString nomeTitulo, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0 );

    void setTitulo(QString);
    QString getTitulo();
    void addAtributo(QString nome, bool chavePrimaria, bool chaveEstrangeira);

private:
    QRectF *tab;

    QGraphicsLineItem *linha;

    QGraphicsScene *scene;

    QGraphicsTextItem *titulo;
    QGraphicsTextItem *atributo;

    QString nomeTitulo;

    QList<QGraphicsTextItem *> listaAtributo;
};

#endif // TABELA_H
