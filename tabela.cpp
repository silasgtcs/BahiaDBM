#include "tabela.h"

Tabela::Tabela( QString nomeTitulo, QGraphicsItem *parent, QGraphicsScene *scene )
    : QGraphicsRectItem( parent, scene )
{
    this->scene = scene;
    tab = new QRectF(2424,2397,200,25);
    setRect(*tab);

    this->scene->addLine(tab->x(),tab->y()+25,tab->x()+tab->width(),tab->y()+25);

    titulo = new QGraphicsTextItem();

    //titulo->setTextInteractionFlags(Qt::TextEditorInteraction);
    titulo->setFocus();
    //titulo->setParentItem( ( tipo == Poligono::ent_associativa) ? childPol : mainPol );

    titulo->setPlainText(nomeTitulo);
    titulo->setPos(tab->x()+(tab->width()/2)-(nomeTitulo.size()*3.5),tab->y());
    this->scene->addItem(titulo);
}

void Tabela::setTitulo(QString titulo)
{
    this->nomeTitulo = titulo;
}

QString Tabela::getTitulo()
{
    return this->nomeTitulo;
}

void Tabela::addAtributo(QString nome, bool chavePrimaria, bool chaveEstrangeira)
{
    if ( chaveEstrangeira )
    {
        nome.insert(0, "[FK] ");
    }

    if ( chavePrimaria )
    {
        nome.insert(0, "[PK] ");
    }

    atributo = new QGraphicsTextItem();
    atributo->setPlainText(nome);
    atributo->setPos(tab->x(),tab->y()+tab->height());
    this->scene->addItem(atributo);
    listaAtributo.push_back(atributo);

    tab->setHeight(tab->height()+25);
    setRect(*tab);
}
