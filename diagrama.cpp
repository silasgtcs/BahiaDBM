#include <QtGui>

#include "diagrama.h"

Diagrama::Diagrama( QObject *parent ) : QGraphicsScene(parent)
{
    Rselecionar = NULL;
    Lselecionar = NULL;
    novaLinha = NULL;
    ativaSelecao = false;
    ativaMover = false;
    countRelacionamento = 1;
    countEntidade = 1;
    countAtributo = 1;
    countAtributoIdentificador = 1;
    countEntidadeAssociativa = 1;
    pilhaDeAcoes = AcoesPilha::instanciaUnica();
}

void Diagrama::setDesfazer(QGraphicsItem *item)
{
    desfazerItem.push(item);
}

void Diagrama::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch(tipoER)
    {
        case(entidade):
            poli = new Poligono(Poligono::entidade,true,0,this);
            nomeEntidade = new Texto();
            nomeEntidade->setTextInteractionFlags(Qt::TextEditorInteraction);
            addItem(nomeEntidade);
            nomeEntidade->setFocus();
            nomeEntidade->setParentItem(poli);
            poli->setPos(event->scenePos());
            //Altera posição do texto para que fique no centro da figura.
            nomeEntidade->setPos(nomeEntidade->x()-10, nomeEntidade->y()-10);
            nomeEntidade->setPlainText(QString("E%1").arg(countEntidade++));
            emit itemInserido();
            setDesfazer(poli);
            break;

        case(relacionamento):
            poli = new Poligono(Poligono::relacionamento,true,0,this);
            setDesfazer(poli);
            nomeRelacionamento = new Texto();
            nomeRelacionamento->setTextInteractionFlags(Qt::TextEditorInteraction);
            addItem(nomeRelacionamento);
            nomeRelacionamento->setFocus();
            nomeRelacionamento->setParentItem(poli);
            poli->setPos(event->scenePos());
            //Altera posição do texto para que fique no centro da figura.
            nomeRelacionamento->setPos(nomeRelacionamento->x()-10, nomeRelacionamento->y()-10);
            nomeRelacionamento->setPlainText(QString("R%1").arg(countRelacionamento++));
            emit itemInserido();
            break;

        case(gen_esp):
            poli = new Poligono(Poligono::gen_esp,true,0,this);
            poli->setPos(event->scenePos());
            emit itemInserido();
            setDesfazer(poli);
            break;

        case(ent_associativa):
            poli = new Poligono(Poligono::entidade,true,0,this);
            poli2 = new Poligono(Poligono::relacionamento,false,0,this);
            //poli2->setParentItem(poli);

            nomeEntidadeAssociativa = new Texto();
            //nomeEntidadeAssociativa->setTextInteractionFlags(Qt::TextEditorInteraction);
            nomeEntidadeAssociativa->setParentItem(poli2);
            nomeEntidadeAssociativa->setPos(nomeEntidadeAssociativa->x()-10, nomeEntidadeAssociativa->y()-10);
            nomeEntidadeAssociativa->setPlainText(QString("EA%1").arg(countEntidadeAssociativa++));
            addItem(nomeEntidadeAssociativa);
            nomeEntidadeAssociativa->setFocus();

            poli2->setScale(poli2->scale()-0.2);
            poli2->setPoligonoAssociado(poli);

            poli->setPos(event->scenePos());
            poli2->setPos(event->scenePos());

            poli->setZValue(-500);

            connect(poli, SIGNAL(posicaoAlterada(const QPointF)), poli2, SLOT(acompanharObjeto(const QPointF)));
            connect(poli, SIGNAL(removido()), poli2, SLOT(remover()));

            //TODO: Rever
            poli->setTipo(Poligono::ent_associativa);
            poli2->setTipo(Poligono::ent_associativa);
            emit itemInserido();
            setDesfazer(poli);
            break;

        case(atributo):
            atr = new Atributo(Atributo::atributo,0,this);
            nomeAtributo = new Texto();
            //nomeAtributo->setTextInteractionFlags(Qt::TextEditorInteraction);
            addItem(nomeAtributo);
            nomeAtributo->setFocus();
            nomeAtributo->setParentItem(atr);
            atr->setPos(event->scenePos());
            nomeAtributo->setPos(nomeAtributo->x()-5, nomeAtributo->y()-25);
            nomeAtributo->setPlainText(QString("A%1").arg(countAtributo++));
            emit itemInserido();
            setDesfazer(atr);
            break;

        case(atributo_ident):
            atr = new Atributo(Atributo::atributo_identif,0,this);
            nomeAtributo = new Texto();
            //nomeAtributo->setTextInteractionFlags(Qt::TextEditorInteraction);
            addItem(nomeAtributo);
            nomeAtributo->setFocus();
            nomeAtributo->setParentItem(atr);
            atr->setPos(event->scenePos());
            nomeAtributo->setPos(nomeAtributo->x()-5, nomeAtributo->y()-25);
            nomeAtributo->setPlainText(QString("AI%1").arg(countAtributoIdentificador++));
            emit itemInserido();
            setDesfazer(atr);
            break;

        case(linha):
            novaLinha = new QGraphicsLineItem(QLineF(event->scenePos(), event->scenePos()));
            novaLinha->setPen(QPen(Qt::black, 1.0));
            addItem(novaLinha);
            break;

        case(mouse):
            if ( (itemAt(event->scenePos())) == 0 )
            {
                Rselecionar = new QGraphicsRectItem(NULL, this);
                Rselecionar->setRect( QRectF( event->scenePos(), event->scenePos() + QPointF(1,1) ) );
                Rselecionar->setPen(QPen(Qt::black, 1, Qt::DashDotLine));
                pontoInicial = event->scenePos();
                ativaSelecao = true;
            }
            else
            {
                pontoInicial = event->scenePos();
                ativaMover = true;
            }
            break;
    }
    QGraphicsScene::mousePressEvent(event);
}

void Diagrama::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (( tipoER == mouse ) && ( ativaSelecao ))
    {
        Rselecionar->setRect( QRectF(pontoInicial , event->scenePos()).normalized() );
        setSelectionArea(Rselecionar->shape());
    }

    if (( tipoER == linha ) && ( novaLinha != NULL ))
    {
        QLineF novaLinha2(novaLinha->line().p1(), event->scenePos());
        novaLinha->setLine(novaLinha2);
    }
    else
        QGraphicsScene::mouseMoveEvent(event);
}

void Diagrama::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (( tipoER == mouse ))
    {
        if( Rselecionar != NULL && ativaSelecao)
        {
            removeItem(Rselecionar);
            delete Rselecionar;
            Rselecionar = NULL;
            ativaSelecao = false;
        }
        if(ativaMover && event->scenePos() != pontoInicial)
        {
            pilhaDeAcoes->addAcao(new AcaoMover(this->selectedItems(), event->scenePos() - pontoInicial));
            ativaMover = false;
        }
    }

    if (( tipoER == linha ) && ( novaLinha != NULL ))
    {
        QList < QGraphicsItem *> inicioLinha = items(novaLinha->line().p1());
        QList < QGraphicsItem *> fimLinha = items(novaLinha->line().p2());

        item1 = item2 = NULL;
        cast1P = cast2P = NULL;
        castA = NULL;

        for ( int i=0; i<inicioLinha.size(); i++ )
            if ( inicioLinha[i] != novaLinha )
            {
                item1 = inicioLinha[i];
                break;
            }

        for ( int i=0; i<fimLinha.size(); i++ )
            if ( fimLinha[i] != novaLinha )
            {
                item2 = fimLinha[i];
                break;
            }

        if ((( item1 != NULL ) && ( item2 != NULL )) && ( item1 != item2 ))
        {
            lig = new Ligacao(item1, item2);
            lig->setZValue(-1000.0);
            addItem(lig);
            setDesfazer(lig);


            //EM CONSTRUÇÃO
            //Faz CAST para verificar se existe auto-relacionamento e verificar a necessidade de colocar cardinalidade.
            if (( item1->type() == Poligono::Type ) && ( item2->type() == Poligono::Type ))
            {
                //Essas duas verificações(if) servem para tratar o caso da entidade associativa, onde a linha pode estar sobre
                //um filho fazendo com que este não envie atualizações para movimentação da cardinalidade.
                while ( item1->parentItem() )
                    item1 = item1->parentItem();
                while ( item2->parentItem() )
                    item2 = item2->parentItem();

                cast1P = qgraphicsitem_cast<Poligono *>(item1);
                cast2P = qgraphicsitem_cast<Poligono *>(item2);

                while(cast1P->getPoligonoAssociado())
                    cast1P = cast1P->getPoligonoAssociado();
                while(cast2P->getPoligonoAssociado())
                    cast2P = cast2P->getPoligonoAssociado();

                if (( cast1P->getTipo() == Poligono::ent_associativa ) || ( cast2P->getTipo() == Poligono::ent_associativa ))
                {
                    if(( cast1P->getTipo() == Poligono::entidade ) || ( cast2P->getTipo() == Poligono::entidade ))
                    {
                        lig->setZValue(-250);
                    }
                }

                //Verifica se a ligação é entre entidade/relacionamento ou entidade associativa/relacionamento.
                //Em caso afirmativo, permite a colocação de cardinalidade.
                if (((( cast1P->getTipo() == Poligono::entidade ) && ( cast2P->getTipo() == Poligono::relacionamento ))
                    || (( cast1P->getTipo() == Poligono::relacionamento ) && ( cast2P->getTipo() == Poligono::entidade )))
                    || ((( cast1P->getTipo() == Poligono::entidade ) && ( cast2P->getTipo() == Poligono::ent_associativa ))
                        || (( cast1P->getTipo() == Poligono::ent_associativa ) && ( cast2P->getTipo() == Poligono::entidade ))))
                {
                    insereCardinalidade = new Cardinalidade( cast1P, cast2P, lig );
                    insereCardinalidade->setCursor(Qt::PointingHandCursor);

                    cast1P->setCardinalidades_Associadas(insereCardinalidade);
                    cast2P->setCardinalidades_Associadas(insereCardinalidade);
                    lig->setCardinalidades_Associadas(insereCardinalidade);

                    if (( cast1P->getTipo() == Poligono::entidade ) && ( cast2P->getTipo() == Poligono::relacionamento ))
                    {
                        insereCardinalidade->setEntidades_Associadas(cast1P);
                        insereCardinalidade->setRelacionamento_Associados(cast2P);
                    }
                    else if (( cast1P->getTipo() == Poligono::relacionamento ) && ( cast2P->getTipo() == Poligono::entidade ))
                    {
                        insereCardinalidade->setEntidades_Associadas(cast2P);
                        insereCardinalidade->setRelacionamento_Associados(cast1P);
                    }
                    else if (( cast1P->getTipo() == Poligono::entidade ) && ( cast2P->getTipo() == Poligono::ent_associativa ))
                    {
                        insereCardinalidade->setEntidades_Associativas_Associadas(cast2P);
                        insereCardinalidade->setEntidades_Associadas(cast1P);
                    }
                    else
                    {
                        insereCardinalidade->setEntidades_Associativas_Associadas(cast1P);
                        insereCardinalidade->setEntidades_Associadas(cast2P);
                    }

                    addItem(insereCardinalidade);
                }

                //Verifica se existe auto-relacionamento. Em caso afirmativo, permite a colocação de nomes.
                if ( cast1P->getAutoRelacionamento() )
                {
//                    nomeEntidade2 = new texto();
//                    nomeEntidade2->setTextInteractionFlags(Qt::TextEditorInteraction);
//                    addItem(nomeEntidade2);
//                    nomeEntidade2->setParentItem(poli);
//                    nomeEntidade2->setPos(nomeEntidade2->x()-20, nomeEntidade2->y()-20);
//                    nomeEntidade2->setPlainText(QString("E%1").arg(countEntidade++));

                    nomeLigacao1 = new Texto();
                    nomeLigacao2 = new Texto();
                    nomeLigacao1->setTextInteractionFlags(Qt::TextEditorInteraction);
                    nomeLigacao2->setTextInteractionFlags(Qt::TextEditorInteraction);
                    addItem(nomeLigacao1);
                    addItem(nomeLigacao2);
                    nomeLigacao1->setParentItem(cast1P);
                    nomeLigacao2->setParentItem(cast2P);
                    if ( cast1P->getTipo() == Poligono::relacionamento )
                    {
                        nomeLigacao1->setPos(nomeLigacao1->x()-14, nomeLigacao1->y()-45);
                        nomeLigacao2->setPos(nomeLigacao2->x()-14, nomeLigacao2->y()+35);
                    }
                    else
                    {
                        nomeLigacao1->setPos(nomeLigacao1->x()-14, nomeLigacao1->y()+35);
                        nomeLigacao2->setPos(nomeLigacao2->x()-14, nomeLigacao2->y()-35);
                    }
                    nomeLigacao1->setPlainText(QString("AR1"));
                    nomeLigacao2->setPlainText(QString("AR2"));
                }
            }

            //Altera posição do item para que slot seja ativado e linha seja atualizada.
            item1->setPos(item1->x()+0.1, item1->y()+0.1);
            item1->setPos(item1->x()-0.1, item1->y()-0.1);
        }

        item1 = item2 = NULL;

        removeItem(novaLinha);
        delete novaLinha;
        novaLinha = NULL;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}
