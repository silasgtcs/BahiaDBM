#include "acao_criar_atributo.h"
#include "texto.h"
#include <exception>

AcaoCriarAtributo::AcaoCriarAtributo(QGraphicsScene * scene, Atributo::Tipo tipo, QString nome, QPointF posicao)
{
    this->scene = scene;
    this->posicao = posicao;

    atributo = new Atributo(tipo,NULL,NULL);

    nomeAtributo = new Texto();
    nomeAtributo->setFocus();
    nomeAtributo->setParentItem(atributo);
    nomeAtributo->setTipoOwner(1);
    nomeAtributo->setPos(nomeAtributo->x()-5, nomeAtributo->y()-25);
    nomeAtributo->setPlainText(nome);

    atributo->setPos(posicao);
}

void AcaoCriarAtributo::fazerAcao()
{
    scene->addItem(atributo);
    atributo->setParent(scene);
    atributo->setPos(posicao);
    atributo->doRemove(false);
}

void AcaoCriarAtributo::desfazerAcao()
{
    scene->removeItem(atributo);
    atributo->doRemove(true);
}

void AcaoCriarAtributo::dispose()
{
    try {
        if(atributo)
            delete atributo;
    } catch(std::exception ex) {
        qDebug() << "dispose do AcaoCriarAtributo tenta deletar atributo ja deletado";
        // provavelmente atributo ja foi deletado anteriormente
    }
}
