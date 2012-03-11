#include "acao_criar_ligacao.h"
#include <exception>

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

    if (( cast1P == NULL ) && ( cast2P == NULL ))
    {
        delete ligacao;
        ligacao = NULL;
        return;
    }

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
    Poligono * properPoligono = (cast1P != NULL) ? cast1P : cast2P;
    if ( properPoligono != NULL )
    {
        properPoligono->moveBy(0.1, 0.1);
        properPoligono->moveBy(-0.1, -0.1);
    }
    Atributo * attr = ligacao->getCastItemA();
    if(attr){
        attr->moveBy(0.1, 0.1);
        attr->moveBy(-0.1, -0.1);
    }

}

void AcaoCriarLigacao::fazerAcao()
{
    if(cardinalidade)
    {
        //        Poligono * cast1P = qgraphicsitem_cast<Poligono *>(item1);
        //        Poligono * cast2P = qgraphicsitem_cast<Poligono *>(item2);

        cast1P->addCardinalidadesAssociadas(cardinalidade);
        cast2P->addCardinalidadesAssociadas(cardinalidade);

        scene->addItem(cardinalidade);
    }
    ligacao->doRemove(false);
    scene->addItem(ligacao);
}

void AcaoCriarLigacao::desfazerAcao()
{
    if(cardinalidade)
    {
        //        Poligono * cast1P = qgraphicsitem_cast<Poligono *>(item1);
        //        Poligono * cast2P = qgraphicsitem_cast<Poligono *>(item2);

        cast1P->getCardinalidadesAssociadas().removeAll(cardinalidade);
        cast2P->getCardinalidadesAssociadas().removeAll(cardinalidade);
        scene->removeItem(cardinalidade);
    }
    ligacao->doRemove(true);
    scene->removeItem(ligacao);
}

void AcaoCriarLigacao::dispose()
{
    try {
        if(ligacao)
            delete ligacao;
    } catch(std::exception ex) {
        qDebug() << "dispose do AcaoCriarLigacao tenta deletar item ja deletado";
        // provavelmente atributo ja foi deletado anteriormente
    }
}
