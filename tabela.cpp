#include "tabela.h"

Tabela::Tabela( QGraphicsItem *parent, QGraphicsScene *scene )
    : QGraphicsRectItem( parent, scene )
{
    setAltura(50.0);
    setLargura(200.0);

    tab = new QRectF(2424,2397,getLargura(),getAltura());
    setRect(2424,2397,200,50);

    scene->addLine(2424,2397+25,2424+200,2397+25);

    titulo = new Texto();

    titulo->setTextInteractionFlags(Qt::TextEditorInteraction);
    titulo->setFocus();
    //titulo->setParentItem( ( tipo == Poligono::ent_associativa) ? childPol : mainPol );

    titulo->setPlainText("Teste");
    titulo->setPos(2424,2397);
    scene->addItem(titulo);
}

void Tabela::setAltura(double a)
{
    this->altura = a;
}

void Tabela::setLargura(double l)
{
    this->largura = l;
}

double Tabela::getAltura()
{
    return this->altura;
}

double Tabela::getLargura()
{
    return this->largura;
}
