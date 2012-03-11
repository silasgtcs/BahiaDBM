#include "acao_criar_atributo.h"
#include "texto.h"

AcaoCriarAtributo::AcaoCriarAtributo(QGraphicsScene * scene, Atributo::Tipo tipo, QString nome, QPointF posicao)
{
    this->scene = scene;
    this->posicao = posicao;

    atributo = new Atributo(tipo,NULL,NULL);

    Texto * nomeAtributo = new Texto();
    nomeAtributo->setFocus();
    nomeAtributo->setParentItem(atributo);
    nomeAtributo->setPos(nomeAtributo->x()-5, nomeAtributo->y()-25);
    nomeAtributo->setPlainText(nome);


    atributo->setPos(posicao);
}

void AcaoCriarAtributo::fazerAcao()
{
    scene->addItem(atributo);
    atributo->setParent(scene);
    atributo->setPos(posicao);
}

void AcaoCriarAtributo::desfazerAcao()
{
    scene->removeItem(atributo);
}
