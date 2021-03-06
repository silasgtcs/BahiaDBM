#include "ligacao.h"
#include <poligono.h>
#include <cardinalidade.h>

Ligacao::Ligacao(QGraphicsItem *item1, QGraphicsItem *item2, QGraphicsItem *parent ) : QGraphicsLineItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    castItem1P = castItem2P =  NULL; //Poligonos
    castItem1T = castItem2T = NULL; //Tabelas modelo lógico
    castItemA = NULL; //Atributos
    cardItem = NULL; //Cardinalidades
    setEntidadeFracaAtiva(false);

    //Faz CAST em Item1, de acordo com o tipo que for recebido
    if (item1->type() == Poligono::Type)
    {
        if ( item1->parentItem() != 0 ) //Caso linha esteja sobre filho, coloque-a no pai.
            item1 = item1->parentItem();

        castItem1P = qgraphicsitem_cast<Poligono *>(item1);
    }

    else if (item1->type() == Atributo::Type)
    {
        castItemA = qgraphicsitem_cast<Atributo *>(item1);
    }

    else if ( item1->type() == Tabela::Type )
    {
        castItem1T = qgraphicsitem_cast<Tabela *>(item1);
    }

    //Faz CAST em Item2, de acordo com o tipo que for recebido
    if (item2->type() == Poligono::Type)
    {
        if ( item2->parentItem() != 0 )
            item2 = item2->parentItem();

        castItem2P = qgraphicsitem_cast<Poligono *>(item2);
    }

    else if (item2->type() == Atributo::Type)
    {
        castItemA = qgraphicsitem_cast<Atributo *>(item2);
        //connect(castItemA, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos()) );
    }

    else if ( item2->type() == Tabela::Type )
    {
        castItem2T = qgraphicsitem_cast<Tabela *>(item2);
    }

    /*Verifica se itens já estão conectados. Em caso afirmativo, não insere nova linha.
      Verifica também validade de itens que estão sendo ligados. Por exemplo: entidade com entidade não é permitida a ligação.*/
    if (( castItem1P != NULL ) && ( castItem2P != NULL ))
    {
        if (( castItem1P->getTipo() == Poligono::entidade ) && ( castItem2P->getTipo() == Poligono::entidade ))
            castItem1P = castItem2P = NULL;

        else if ((( castItem1P->getTipo() == Poligono::relacionamento ) && ( castItem1P->getAutoRelacionamento() ))
                 || (( castItem2P->getTipo() == Poligono::relacionamento ) && ( castItem2P->getAutoRelacionamento() )))
        {
            castItem1P = castItem2P = NULL;
        }

        else if ((( castItem1P->getTipo() == Poligono::gen_esp ) && ( castItem2P->getTipo() != Poligono::entidade ))
            || (( castItem2P->getTipo() == Poligono::gen_esp ) && ( castItem1P->getTipo() != Poligono::entidade )))
        {
            castItem1P = castItem2P = NULL;
        }

        else if ( castItem1P->getLinhasAssociadas().size() > 0 )
        {
            bool controleAR = false;
            if ( castItem2P->getTipo() == Poligono::entidade )
            {
                QList<Poligono *> verifica = castItem1P->getPoligonosAssociadoOfType<Poligono::entidade>();
                for ( int i=0; i<verifica.size(); i++ )
                    if ( verifica[i] == castItem2P )
                    {
                        if (( castItem1P->getAutoRelacionamento() ) || ( castItem1P->getTipo() != Poligono::relacionamento ))
                        {
                            castItem1P = castItem2P = NULL;
                            break;
                        }
                        else
                        {
                            castItem1P->setAutoRelacionamento(true);
                            castItem2P->setAutoRelacionamento(true);
                            controleAR = true;
                            break;
                        }
                    }
            }

            else if ( castItem2P->getTipo() == Poligono::relacionamento )
            {
                QList<Poligono *> verifica = castItem1P->getPoligonosAssociadoOfType<Poligono::relacionamento>();
                for ( int i=0; i<verifica.size(); i++ )
                    if ( verifica[i] == castItem2P )
                    {
                        if (( castItem1P->getAutoRelacionamento() ) || ( castItem1P->getTipo() != Poligono::entidade ))
                        {
                            castItem1P = castItem2P = NULL;
                            break;
                        }
                        else
                        {
                            castItem1P->setAutoRelacionamento(true);
                            castItem2P->setAutoRelacionamento(true);
                            controleAR = true;
                            break;
                        }
                    }
            }

            else if ( castItem2P->getTipo() == Poligono::gen_esp )
            {
                QList<Poligono *> verifica = castItem1P->getPoligonosAssociadoOfType<Poligono::gen_esp>();
                for ( int i=0; i<verifica.size(); i++ )
                    if ( verifica[i] == castItem2P )
                    {
                        castItem1P = castItem2P = NULL;
                        break;
                    }
            }

            else if ( castItem2P->getTipo() == Poligono::ent_associativa )
            {
                QList<Poligono *> verifica = castItem1P->getPoligonosAssociadoOfType<Poligono::ent_associativa>();
                for ( int i=0; i<verifica.size(); i++ )
                    if ( verifica[i] == castItem2P )
                    {
                        castItem1P = castItem2P = NULL;
                        break;
                    }
            }

            //Verifica se existe auto-relacionamento. Em caso afirmativo, permite a colocação de nomes.
            if (controleAR)
            {
                nomeLigacao1 = new Texto();
                nomeLigacao2 = new Texto();
                nomeLigacao1->setTextInteractionFlags(Qt::TextEditorInteraction);
                nomeLigacao2->setTextInteractionFlags(Qt::TextEditorInteraction);
                if ( castItem1P->getTipo() == Poligono::relacionamento )
                {
                    nomeLigacao1->setParentItem(castItem1P);
                    nomeLigacao2->setParentItem(castItem1P);
                }
                else
                {
                    nomeLigacao1->setParentItem(castItem2P);
                    nomeLigacao2->setParentItem(castItem2P);
                }
                nomeLigacao1->setPos(nomeLigacao1->x()+40, nomeLigacao1->y()-40);
                nomeLigacao2->setPos(nomeLigacao2->x()-70, nomeLigacao2->y()-40);
                nomeLigacao1->setPlainText(QString("AR1"));
                nomeLigacao2->setPlainText(QString("AR2"));
            }
        }
    }

    else if ((( castItem1P != NULL  ) || ( castItem2P != NULL )) && ( castItemA != NULL ))
    {
        Poligono *aux;
        bool control = false;
        if ( castItem2P != NULL )
        {
            aux = castItem1P;
            castItem1P = castItem2P;
            control = true;
        }

        if ( castItemA->getConectado() )
        {
            castItem1P = castItem2P = NULL;
            castItemA = NULL;
        }

        else if ( castItem1P->getTipo() == Poligono::gen_esp )
        {
            castItem1P = castItem2P = NULL;
            castItemA = NULL;
        }

        else if ( castItem1P->getLinhasAssociadas().size() > 0 )
        {
            QList<Atributo *> verifica = castItem1P->getAtributosAssociados();
            for ( int i=0; i<verifica.size(); i++ )
                if ( verifica[i] == castItemA )
                {
                    castItem1P = castItem2P = NULL;
                    castItemA = NULL;
                    break;
                }
        }

        if (control)
            castItem1P = aux;
    }

    if (( castItem1P != NULL ) && ( castItem2P != NULL ))
    {
        connect(castItem1P, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos()) );
        connect(castItem2P, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos()) );

        if (( castItem1P->getAutoRelacionamento() ) && ( castItem2P->getAutoRelacionamento() ))
        {
            connect(castItem1P, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPosAutoRelacionamento()) );
            connect(castItem2P, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPosAutoRelacionamento()) );
        }
    }
    else if ((( castItem1P != NULL ) || ( castItem2P != NULL )) && ( castItemA != NULL ))
    {
        Poligono * properPoligono = (castItem1P != NULL) ? castItem1P : castItem2P;
        if ( properPoligono != NULL )
        {
            connect(castItemA, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos()) );
            connect(properPoligono, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos()) );
        }
    }
    else if (( castItem1T != NULL ) && ( castItem2T != NULL ))
    {
        tabelasAssociadas = qMakePair(castItem1T, castItem2T);
        connect(castItem1T, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos()));
        connect(castItem2T, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos()));
    }

    //Verifica se a ligação é entre entidade/relacionamento, entidade associativa/relacionamento ou relacionamento/entidade associativa.
    //Em caso afirmativo, permite a colocação de cardinalidade.
    if (( castItem1P != NULL ) && ( castItem2P != NULL ))
    {
        if ((((( castItem1P->getTipo() == Poligono::entidade ) && ( castItem2P->getTipo() == Poligono::relacionamento ))
              || (( castItem1P->getTipo() == Poligono::relacionamento ) && ( castItem2P->getTipo() == Poligono::entidade )))
             || ((( castItem1P->getTipo() == Poligono::entidade ) && ( castItem2P->getTipo() == Poligono::ent_associativa ))
                 || (( castItem1P->getTipo() == Poligono::ent_associativa ) && ( castItem2P->getTipo() == Poligono::entidade ))))
                || ((( castItem1P->getTipo() == Poligono::relacionamento ) && ( castItem2P->getTipo() == Poligono::ent_associativa ))
                    || (( castItem1P->getTipo() == Poligono::ent_associativa ) && ( castItem2P->getTipo() == Poligono::relacionamento ))))
        {
            cardItem = new Cardinalidade( castItem1P, castItem2P, this );
            cardItem->setCursor(Qt::PointingHandCursor);
            this->addCardinalidadeAssociada(cardItem);
        }
    }

    conectarObjetos();
}

void Ligacao::conectarObjetos()
{
    if (( castItem1P != NULL ) && ( castItem2P != NULL ))
    {
        //Adiciona no vector dessa entidade que esse relacionamento e esta linha pertence-a.

        //Nao precisa verificar tipo de cada item,  funcao addPoligonoAssociado ja trata internamente
        castItem1P->addPoligonoAssociado(castItem2P);
        castItem2P->addPoligonoAssociado(castItem1P);
        castItem2P->addLinhasAssociadas(this);
        castItem1P->addLinhasAssociadas(this);
        this->addPoligonoAssociado(castItem1P);
        this->addPoligonoAssociado(castItem2P);
        castItem1P->setConectado(true);
        castItem2P->setConectado(true);
    }
    else if ((( castItem1P != NULL ) || ( castItem2P != NULL )) && ( castItemA != NULL ))
    {
        Poligono * properPoligono = (castItem1P != NULL) ? castItem1P : castItem2P;
        if ( properPoligono != NULL )
        {
            //Adiciona no vector dessa entidade que esta linha pertence-a.
            properPoligono->addLinhasAssociadas(this);
            properPoligono->addAtributosAssociados(castItemA);

            if(castItemA->parentItem() == NULL) {
                temp = mapToItem(properPoligono, castItemA->scenePos());
                castItemA->setPos(temp); //Seta atributo como filho e atualiza posição
            }
            castItemA->setParentItem(properPoligono);

            castItemA->addPoligonoAssociado(properPoligono);
            this->addPoligonoAssociado(properPoligono);
        }

        this->addAtributoAssociado(castItemA);
        castItemA->addLinhaAssociada(this);
        castItemA->setConectado(true);
    }
    else
        castItem1P = castItem2P = NULL;

    //Verifica se a ligação é entre entidade/relacionamento, entidade associativa/relacionamento ou relacionamento/entidade associativa.
    //Em caso afirmativo, permite a colocação de cardinalidade.
    if (( castItem1P != NULL ) && ( castItem2P != NULL ))
    {
        if ((((( castItem1P->getTipo() == Poligono::entidade ) && ( castItem2P->getTipo() == Poligono::relacionamento ))
              || (( castItem1P->getTipo() == Poligono::relacionamento ) && ( castItem2P->getTipo() == Poligono::entidade )))
             || ((( castItem1P->getTipo() == Poligono::entidade ) && ( castItem2P->getTipo() == Poligono::ent_associativa ))
                 || (( castItem1P->getTipo() == Poligono::ent_associativa ) && ( castItem2P->getTipo() == Poligono::entidade ))))
                || ((( castItem1P->getTipo() == Poligono::relacionamento ) && ( castItem2P->getTipo() == Poligono::ent_associativa ))
                    || (( castItem1P->getTipo() == Poligono::ent_associativa ) && ( castItem2P->getTipo() == Poligono::relacionamento ))))
        {
            this->addCardinalidadeAssociada(cardItem);

            cardItem->addPoligonoAssociado(castItem1P);
            cardItem->addPoligonoAssociado(castItem2P);
        }
    }
    //Altera posição do item para que slot seja ativado e linha seja atualizada.
    Poligono * properPoligono = (castItem1P != NULL) ? castItem1P : castItem2P;
    if ( properPoligono != NULL )
    {
        properPoligono->moveBy(0.1, 0.1);
        properPoligono->moveBy(-0.1, -0.1);
    }
    if(castItemA){
        castItemA->moveBy(0.1, 0.1);
        castItemA->moveBy(-0.1, -0.1);
    }
    if ( castItem1T )
    {
        castItem1T->moveBy(0.1, 0.1);
        castItem1T->moveBy(-0.1, -0.1);
    }
}

void Ligacao::desconectarObjetos(){
    if (( castItem1P != NULL ) && ( castItem2P != NULL ))
    {
        //Adiciona no vector dessa entidade que esse relacionamento e esta linha pertence-a.

        //Nao precisa verificar tipo de cada item,  funcao addPoligonoAssociado ja trata internamente
        castItem1P->removerPoligonoAssociado(castItem2P);
        castItem2P->removerPoligonoAssociado(castItem1P);
        castItem2P->removerLinhaAssociada(this);
        castItem1P->removerLinhaAssociada(this);
        this->removerPoligonoAssociado(castItem1P);
        this->removerPoligonoAssociado(castItem2P);
    }
    else if ((( castItem1P != NULL ) || ( castItem2P != NULL )) && ( castItemA != NULL ))
    {
        Poligono * properPoligono = (castItem1P != NULL) ? castItem1P : castItem2P;
        if ( properPoligono != NULL )
        {
            //Adiciona no vector dessa entidade que esta linha pertence-a.
            properPoligono->removerLinhaAssociada(this);
            properPoligono->removerAtributoAssociada(castItemA);

            castItemA->setPos(castItemA->scenePos());
            castItemA->setParentItem(NULL);

            castItemA->removerPoligonoAssociado(properPoligono);
            this->removerPoligonoAssociado(properPoligono);
        }

        this->removerAtributoAssociado(castItemA);
        castItemA->removerLinhaAssociada(this);
        castItemA->setConectado(false);
    }
    else
        castItem1P = castItem2P = NULL;

    //Verifica se a ligação é entre entidade/relacionamento, entidade associativa/relacionamento ou relacionamento/entidade associativa.
    //Em caso afirmativo, permite a colocação de cardinalidade.
    if (( castItem1P != NULL ) && ( castItem2P != NULL ))
    {
        if ((((( castItem1P->getTipo() == Poligono::entidade ) && ( castItem2P->getTipo() == Poligono::relacionamento ))
              || (( castItem1P->getTipo() == Poligono::relacionamento ) && ( castItem2P->getTipo() == Poligono::entidade )))
             || ((( castItem1P->getTipo() == Poligono::entidade ) && ( castItem2P->getTipo() == Poligono::ent_associativa ))
                 || (( castItem1P->getTipo() == Poligono::ent_associativa ) && ( castItem2P->getTipo() == Poligono::entidade ))))
                || ((( castItem1P->getTipo() == Poligono::relacionamento ) && ( castItem2P->getTipo() == Poligono::ent_associativa ))
                    || (( castItem1P->getTipo() == Poligono::ent_associativa ) && ( castItem2P->getTipo() == Poligono::relacionamento ))))
        {
            this->removerCardinalidadeAssociada(cardItem);

            cardItem->removerPoligonoAssociado(castItem1P);
            cardItem->removerPoligonoAssociado(castItem2P);
        }
    }
}

void Ligacao::atualizaPos()
{
    Poligono * properPoligono = (castItem1P != NULL) ? castItem1P : castItem2P;

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

    else if (( castItem1T != NULL ) && ( castItem2T != NULL ))
    {
        QLineF line(mapFromItem( castItem1T, castItem1T->rect().width()/2, castItem1T->rect().height()/2 ), mapFromItem( castItem2T, castItem2T->rect().width()/2, castItem2T->rect().height()/2 ));
        setLine(line);
    }

    else if (( properPoligono != NULL ) && ( castItemA != NULL ))
    {
        QLineF line(mapFromItem( properPoligono, 0, 0 ),
                    mapFromItem( castItemA, castItemA->rect().width()/2, castItemA->rect().height()/2 ));
        setLine(line);
    }
}

void Ligacao::atualizaPosAutoRelacionamento()
{
    if (( castItem1P != NULL ) && ( castItem2P != NULL ))
    {
        QLineF line(mapFromItem( castItem1P, -30, 0 ), mapFromItem( castItem2P, -30, 0 ));
        setLine(line);
    }
}

void Ligacao::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
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

void Ligacao::doRemove(bool value)
{
    if(value)
        desconectarObjetos();
    else // processo inverso, re-adicona
        conectarObjetos();
}

QList<QGraphicsItem *> Ligacao::getToDelete()
{
    QList<QGraphicsItem *> toDelete;
    foreach(Cardinalidade * card, cardinalidades_associadas)
        toDelete.push_back(card);
    return toDelete;
}
