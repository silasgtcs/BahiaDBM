#include "ligacao.h"
#include <poligono.h>

Ligacao::Ligacao(QGraphicsItem *item1, QGraphicsItem *item2, QGraphicsItem *parent ) : QGraphicsLineItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    castItem1P = castItem2P =  NULL; //Poligonos
    castItemA = NULL; //Atributos
    setEntidadeFracaAtiva(false);

    //Faz CAST em Item1, de acordo com o tipo que for recebido
    if (item1->type() == Poligono::Type)
    {
        if ( item1->parentItem() != 0 ) //Caso linha esteja sobre filho, coloque-a no pai.
            item1 = item1->parentItem();

        castItem1P = qgraphicsitem_cast<Poligono *>(item1);
    }

    else if ((item1->type() == Atributo::Type) && ( !item1->parentItem() ))
    {
        castItemA = qgraphicsitem_cast<Atributo *>(item1);
    }

    //Faz CAST em Item2, de acordo com o tipo que for recebido
    if (item2->type() == Poligono::Type)
    {
        if ( item2->parentItem() != 0 )
            item2 = item2->parentItem();

        castItem2P = qgraphicsitem_cast<Poligono *>(item2);
    }

    else if ((item2->type() == Atributo::Type) && ( !item2->parentItem() ))
    {
        castItemA = qgraphicsitem_cast<Atributo *>(item2);
        //connect(castItemA, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos()) );
    }

    /*Verifica se itens já estão conectados. Em caso afirmativo, não insere nova linha.
      Verifica também validade de itens que estão sendo ligados. Por exemplo: entidade com entidade não é permitida a ligação.*/
    if ((( castItem1P != NULL ) && ( castItem2P != NULL )) && (( !castItem1P->getConectado() ) || ( !castItem2P->getConectado() )))
    {
        if ( castItem1P->getTipo() != castItem2P->getTipo() )
        {
            castItem1P->setConectado(true);
            castItem2P->setConectado(true);
        }

        else
        {
            castItem1P = NULL;
            castItem2P = NULL;
        }
    }

    //Auto-relacionamento
    else if ((( castItem1P != NULL ) && ( castItem2P != NULL )) && (( castItem1P->getConectado() ) && ( castItem2P->getConectado() )))
    {
        if (((( castItem1P->getTipo() == 0 ) || (castItem2P->getTipo() == 0))
           && (( castItem1P->getTipo() == 1 ) || ( castItem2P->getTipo() == 1 ))) && ((!castItem1P->getAutoRelacionamento())
           && (!castItem1P->getAutoRelacionamento())))
        {
            bool temp = false;
            for ( int i=0; i<castItem1P->getLinhas_Associadas().size(); i++ )
            {
                if ((( castItem1P->getLinhas_Associadas().at(i)->getEntidades_Associadas().at(0) == castItem1P )
                    || ( castItem1P->getLinhas_Associadas().at(i)->getRelacionamentos_Associados().at(0) == castItem1P ))
                    && (( castItem1P->getLinhas_Associadas().at(i)->getEntidades_Associadas().at(0) == castItem2P )
                    || ( castItem1P->getLinhas_Associadas().at(i)->getRelacionamentos_Associados().at(0) == castItem2P )))
                {
                    castItem1P->setConectado(true);
                    castItem2P->setConectado(true);
                    castItem1P->setAutoRelacionamento(true);
                    castItem2P->setAutoRelacionamento(true);
                    temp = true;
                    break;
                }
            }

            if (( !temp ) && ( castItem1P->getTipo() != castItem2P->getTipo() ))
            {
                castItem1P->setConectado(true);
                castItem2P->setConectado(true);
            }
            else if ( !temp )
            {
                castItem1P = NULL;
                castItem2P = NULL;
            }
        }
        else
        {
            castItem1P = NULL;
            castItem2P = NULL;
        }
    }

    else if ((( castItem1P != NULL ) || ( castItem2P != NULL )) && ( castItemA != NULL ))
    {
        if (( castItem1P != NULL ) && (( !castItem1P->getConectado() ) || ( !castItemA->getConectado() )))
        {
            castItem1P->setConectado(true);
            castItemA->setConectado(true);
        }

        else if (( castItem1P != NULL ) && (( castItem1P->getConectado() ) || ( castItemA->getConectado() )))
        {
            castItem1P = NULL;
            castItemA = NULL;
        }

        else if (( castItem2P != NULL ) && (( !castItem2P->getConectado() ) || ( !castItemA->getConectado() )))
        {
            castItem2P->setConectado(true);
            castItemA->setConectado(true);
        }

        else
        {
            castItem2P = NULL;
            castItemA = NULL;
        }
    }
    //Fim de verificações.

    if (( castItem1P != NULL ) && ( castItem2P != NULL ))
    {
        //Adiciona no vector dessa entidade que esse relacionamento e esta linha pertence-a.
        if (( castItem1P->getTipo() == Poligono::Tipo(0) ) && ( castItem2P->getTipo() == Poligono::Tipo(1) ))
        {
            castItem1P->setRelacionamentos_Associados(castItem2P);
            castItem2P->setEntidades_Associadas(castItem1P);
            castItem1P->setLinhas_Associadas(this);
            castItem2P->setLinhas_Associadas(this);
            this->setEntidades_Associadas(castItem1P);
            this->setRelacionamento_Associados(castItem2P);
        }
        else if (( castItem2P->getTipo() == Poligono::Tipo(0) ) && ( castItem1P->getTipo() == Poligono::Tipo(1) ))
        {
            castItem2P->setRelacionamentos_Associados(castItem1P);
            castItem1P->setEntidades_Associadas(castItem2P);
            castItem2P->setLinhas_Associadas(this);
            castItem1P->setLinhas_Associadas(this);
            this->setEntidades_Associadas(castItem2P);
            this->setRelacionamento_Associados(castItem1P);
        }

        else if (( castItem1P->getTipo() == Poligono::Tipo(0) ) && ( castItem2P->getTipo() == Poligono::Tipo(3) ))
        {
            castItem1P->setEntidades_Associativas_Associadas(castItem2P);
            castItem2P->setEntidades_Associadas(castItem1P);
            castItem1P->setLinhas_Associadas(this);
            castItem2P->setLinhas_Associadas(this);
            this->setEntidades_Associadas(castItem1P);
            this->setEntidades_Associativas_Associadas(castItem2P);
        }

        else if (( castItem1P->getTipo() == Poligono::Tipo(3) ) && ( castItem2P->getTipo() == Poligono::Tipo(0) ))
        {
            castItem1P->setEntidades_Associadas(castItem2P);
            castItem2P->setEntidades_Associativas_Associadas(castItem1P);
            castItem1P->setLinhas_Associadas(this);
            castItem2P->setLinhas_Associadas(this);
            this->setEntidades_Associadas(castItem2P);
            this->setEntidades_Associativas_Associadas(castItem1P);
        }

        connect(castItem1P, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos()) );
        connect(castItem2P, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos()) );

        if (( castItem1P->getAutoRelacionamento() ) && ( castItem2P->getAutoRelacionamento() ))
        {
            connect(castItem1P, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos2()) );
            connect(castItem2P, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos2()) );
        }
    }
    else if ((( castItem1P != NULL ) || ( castItem2P != NULL )) && ( castItemA != NULL ))
    {
        if ( castItem1P != NULL )
        {
            //Adiciona no vector dessa entidade que esta linha pertence-a.
            castItem1P->setLinhas_Associadas(this);

            temp = mapToItem(castItem1P, castItemA->scenePos());
            castItemA->setParentItem(castItem1P);
            castItemA->setPos(temp); //Seta atributo como filho e atualiza posição
            connect(castItem1P, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos()) );
        }
        else
        {
            //Adiciona no vector dessa entidade que esta linha pertence-a.
            castItem2P->setLinhas_Associadas(this);

            temp = mapToItem(castItem2P, castItemA->scenePos());
            castItemA->setParentItem(castItem2P);
            castItemA->setPos(temp); //Seta atributo como filho e atualiza posição
            connect(castItem2P, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos()) );
        }

        castItemA->setLinhaAssociada(this);
        connect(castItemA, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos()) );
    }
    else
        castItem1P = castItem2P = NULL;
}

void Ligacao::atualizaPos()
{
    if (( castItem1P != NULL ) && ( castItem2P != NULL ))
    {
        if (( castItem1P->getAutoRelacionamento() ) && ( castItem2P->getAutoRelacionamento() ))
        {
            QLineF line(mapFromItem( castItem1P, 30,0 ), mapFromItem( castItem2P, 30,0 ));
            setLine(line);
        }
        else
        {
            QLineF line(mapFromItem( castItem1P, 0, 0 ), mapFromItem( castItem2P, 0, 0 ));
            setLine(line);
            //emit linhaAlterada(line);
        }
    }

    else if (( castItem1P != NULL ) && ( castItemA != NULL ))
    {
        QLineF line(mapFromItem( castItem1P, 0, 0 ),
                    mapFromItem( castItemA, castItemA->rect().width()/2, castItemA->rect().height()/2 ));
        setLine(line);
    }

    else if (( castItem2P != NULL ) && ( castItemA != NULL ))
    {
        QLineF line(mapFromItem( castItem2P, 0, 0 ),
                    mapFromItem( castItemA, castItemA->rect().width()/2, castItemA->rect().height()/2 ));
        setLine(line);
    }
}

void Ligacao::atualizaPos2()
{
    if (( castItem1P != NULL ) && ( castItem2P != NULL ))
    {
        QLineF line(mapFromItem( castItem1P, -30, 0 ), mapFromItem( castItem2P, -30, 0 ));
        setLine(line);
    }
}

void Ligacao::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if ( !getEntidadeFracaAtiva() )
        painter->setPen(QPen(Qt::black, 1.0, Qt::SolidLine));
    else
        painter->setPen(QPen(Qt::black, 4));

    if ( isSelected() )
        painter->setPen(QPen(Qt::red, 2, Qt::DashLine));

    painter->drawLine(line());
}

QPainterPath Ligacao::shape() const
{
    QPainterPath path;
    if (line() == QLineF())
         return path;

    path.moveTo(line().p1());
    path.lineTo(line().p2());

    QPainterPathStroker stroker;
    stroker.setWidth(15.0);

    QPainterPath p = stroker.createStroke(path);
    p.addPath(path);

    return p;
}
