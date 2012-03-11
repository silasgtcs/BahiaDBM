#include "acao_criar_ligacao.h"

AcaoCriarLigacao::AcaoCriarLigacao(QGraphicsScene * scene, QGraphicsItem * item1, QGraphicsItem * item2)
{
    //Essas duas verificações(if) servem para tratar o caso da entidade associativa, onde a linha pode estar sobre
    //um filho fazendo com que este não envie atualizações para movimentação da cardinalidade.

    if ( item1->type() == Poligono::Type )
        while ( item1->parentItem() )
            item1 = item1->parentItem();
    if ( item2->type() == Poligono::Type )
        while ( item2->parentItem() )
            item2 = item2->parentItem();

    ligacao = new Ligacao(item1, item2);
    ligacao->setZValue(-1000.0);
    cast1P = ligacao->getCastItem1P();
    cast2P = ligacao->getCastItem2P();
    //    this->item1 = item1;
    //    this->item2 = item2;
    this->scene = scene;

    cardinalidade = ligacao->getCardItem();
    nomeLigacao1 = NULL;
    nomeLigacao2 = NULL;

    if (( cast1P == NULL ) && ( cast2P == NULL ))
    {
        delete ligacao;
        ligacao = NULL;
    }

    //EM CONSTRUÇÃO
    //Faz CAST para verificar se existe auto-relacionamento e verificar a necessidade de colocar cardinalidade.
    if (( cast1P != NULL ) && ( cast2P != NULL ))
    {
        while(cast1P->getPoligonoAssociado())
            cast1P = cast1P->getPoligonoAssociado();
        while(cast2P->getPoligonoAssociado())
            cast2P = cast2P->getPoligonoAssociado();

        if (( cast1P->getTipo() == Poligono::ent_associativa ) || ( cast2P->getTipo() == Poligono::ent_associativa ))
        {
            if(( cast1P->getTipo() == Poligono::entidade ) || ( cast2P->getTipo() == Poligono::entidade ))
            {
                ligacao->setZValue(-250);
            }
        }
    }

    //Altera posição do item para que slot seja ativado e linha seja atualizada.
    if ( cast1P != NULL )
    {
        cast1P->setPos(cast1P->x()+0.1, cast1P->y()+0.1);
        cast1P->setPos(cast1P->x()-0.1, cast1P->y()-0.1);
    }
    else if ( cast2P != NULL )
    {
        cast2P->setPos(cast2P->x()+0.1, cast2P->y()+0.1);
        cast2P->setPos(cast2P->x()-0.1, cast2P->y()-0.1);
    }
}

void AcaoCriarLigacao::fazerAcao()
{
    if(cardinalidade)
    {
        //        Poligono * cast1P = qgraphicsitem_cast<Poligono *>(item1);
        //        Poligono * cast2P = qgraphicsitem_cast<Poligono *>(item2);

        cast1P->setCardinalidades_Associadas(cardinalidade);
        cast2P->setCardinalidades_Associadas(cardinalidade);

        scene->addItem(cardinalidade);
    }
    scene->addItem(ligacao);
    if(nomeLigacao1 && nomeLigacao2)
    {
        scene->addItem(nomeLigacao1);
        scene->addItem(nomeLigacao2);
    }
}

void AcaoCriarLigacao::desfazerAcao()
{
    if(cardinalidade)
    {
        //        Poligono * cast1P = qgraphicsitem_cast<Poligono *>(item1);
        //        Poligono * cast2P = qgraphicsitem_cast<Poligono *>(item2);

        cast1P->getCardinalidades_Associadas().removeAll(cardinalidade);
        cast2P->getCardinalidades_Associadas().removeAll(cardinalidade);
        scene->removeItem(cardinalidade);
    }
    scene->removeItem(ligacao);
    if(nomeLigacao1 && nomeLigacao2)
    {
        scene->removeItem(nomeLigacao1);
        scene->removeItem(nomeLigacao2);
    }
}
