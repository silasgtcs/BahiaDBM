#include <QtGui>

#include "diagrama.h"
#include "acoespilha.h"

Diagrama::Diagrama( QObject *parent, AcoesPilha * pilhaDeAcoes ) : QGraphicsScene(parent)
{
    novaLinha = NULL;
    ativaSelecao = false;
    ativaMover = false;
    countRelacionamento = 1;
    countEntidade = 1;
    countAtributo = 1;
    countAtributoIdentificador = 1;
    countEntidadeAssociativa = 1;
    this->pilhaDeAcoes = pilhaDeAcoes;
}

void Diagrama::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Acao * acao = NULL;
    switch(tipoER)
    {
    case(entidade):
        acao = new AcaoCriarPoligono(this, Poligono::entidade, QString("E%1").arg(countEntidade++), event->scenePos());
        break;

    case(relacionamento):
        acao = new AcaoCriarPoligono(this, Poligono::relacionamento, QString("R%1").arg(countEntidade++), event->scenePos());
        break;

    case(gen_esp):
        acao = new AcaoCriarPoligono(this, Poligono::gen_esp, QString(""), event->scenePos());
        break;

    case(ent_associativa):
        acao = new AcaoCriarPoligono(this, Poligono::ent_associativa, QString("EA%1").arg(countEntidadeAssociativa++), event->scenePos());
        break;

    case(atributo):
        acao = new AcaoCriarAtributo(this, Atributo::atributo, QString("A%1").arg(countAtributo++), event->scenePos());
        break;

    case(atributo_ident):
        acao = new AcaoCriarAtributo(this, Atributo::atributo_identif, QString("AI%1").arg(countAtributoIdentificador++), event->scenePos());
        break;

    case(linha):
        novaLinha = new QGraphicsLineItem(QLineF(event->scenePos(), event->scenePos()));
        novaLinha->setPen(QPen(Qt::black, 1.0));
        addItem(novaLinha);
        break;

    case(mouse):
        if ( (itemAt(event->scenePos())) == 0 )
        {
            selecionarBox = new QGraphicsRectItem(NULL, this);
            selecionarBox->setRect( QRectF( event->scenePos(), event->scenePos() + QPointF(1,1) ) );
            selecionarBox->setPen(QPen(Qt::black, 1, Qt::DashDotLine));
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
    if(acao) //Se alguma acao foi executada
    {
        pilhaDeAcoes->addAcao(acao, true);
        emit itemInserido();
    }
    QGraphicsScene::mousePressEvent(event);
}

void Diagrama::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (( tipoER == mouse ) && ( ativaSelecao ))
    {
        selecionarBox->setRect( QRectF(pontoInicial , event->scenePos()).normalized() );
        setSelectionArea(selecionarBox->shape());
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
        if( selecionarBox != NULL && ativaSelecao)
        {
            removeItem(selecionarBox);
            delete selecionarBox;
            selecionarBox = NULL;
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

        QGraphicsItem * item1, * item2;
        item1 = item2 = NULL;

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
            AcaoCriarLigacao * acao = new AcaoCriarLigacao(this, item1, item2);
            if ( acao->getLigacao() )
            {
                pilhaDeAcoes->addAcao(acao);
                acao->fazerAcao();
            }
        }

        item1 = item2 = NULL;

        removeItem(novaLinha);
        delete novaLinha;
        novaLinha = NULL;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}
