#include "acao_criar_poligono.h"
#include "texto.h"

AcaoCriarPoligono::AcaoCriarPoligono(QGraphicsScene * scene, Poligono::Tipo tipo, QString nome, QPointF posicao)
{
    this->scene = scene;
    this->posicao = posicao;
    this->childPol = NULL;
    if(tipo != Poligono::ent_associativa) {
        mainPol = new Poligono(tipo, true, NULL, NULL);
    } else {
        mainPol = new Poligono(Poligono::entidade,true,NULL,NULL);
        childPol = new Poligono(Poligono::relacionamento,false,NULL,NULL);
    }


    Texto * nomeEntidade = new Texto();

    nomeEntidade->setTextInteractionFlags(Qt::TextEditorInteraction);
    nomeEntidade->setFocus();
    nomeEntidade->setParentItem( ( tipo == Poligono::ent_associativa) ? childPol : mainPol );

    //Altera posição do texto para que fique no centro da figura.
    nomeEntidade->setPos(nomeEntidade->x()-10, nomeEntidade->y()-10);
    nomeEntidade->setPlainText(nome);

    if(tipo == Poligono::gen_esp) {
        nomeEntidade->setCursor(Qt::PointingHandCursor);
        nomeEntidade->setPos(nomeEntidade->x()+25, nomeEntidade->y()-30);
        nomeEntidade->setGenEspAtiva(true);
        nomeEntidade->setPlainText(nomeEntidade->getTipoGenEsp());
    }

    mainPol->setPos(posicao);

    if(tipo == Poligono::ent_associativa)
    {
        childPol->setScale(childPol->scale()-0.2);
        childPol->setPoligonoAssociado(mainPol);
        childPol->setPos(posicao);

        mainPol->setZValue(-500);

        mainPol->connect(mainPol, SIGNAL(posicaoAlterada(const QPointF)), childPol, SLOT(acompanharObjeto(const QPointF)));
        mainPol->connect(mainPol, SIGNAL(removido()), childPol, SLOT(remover()));

        //TODO: Rever
        mainPol->setTipo(Poligono::ent_associativa);
        childPol->setTipo(Poligono::ent_associativa);
    }
}

void AcaoCriarPoligono::fazerAcao()
{
    scene->addItem(mainPol);
    if(childPol)
        scene->addItem(childPol);
    mainPol->setParent(scene);
    mainPol->setPos(posicao);
}

void AcaoCriarPoligono::desfazerAcao()
{
    scene->removeItem(mainPol);
    if(childPol)
        scene->removeItem(childPol);
}
