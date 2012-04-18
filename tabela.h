#ifndef TABELA_H
#define TABELA_H

#include <QtGui>
#include "texto.h"

class Tabela : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Tabela( QGraphicsItem *parent = 0, QGraphicsScene *scene = 0 );

    void setAltura(double);
    double getAltura();
    void setLargura(double);
    double getLargura();

    QRectF *tab;
    QGraphicsLineItem *linha;
    Texto *titulo;

private:
    double altura;
    double largura;
};

#endif // TABELA_H
