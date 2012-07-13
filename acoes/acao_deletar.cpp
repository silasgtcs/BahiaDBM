#include "acao_deletar.h"
#include "objetoremovivel.h"
#include "poligono.h"
#include "texto.h"
#include <exception>

AcaoDeletar::AcaoDeletar(QGraphicsScene * scene, QList<QGraphicsItem *> selecionados)
{
    this->items = selecionados;
    this->scene = scene;
    this->deleted = false;
}

void AcaoDeletar::fazerAcao()
{
    int i, tam;

    removidos.clear();
    foreach(QGraphicsItem * item, items) {
        ObjetoRemovivel * objeto = dynamic_cast<ObjetoRemovivel *>(item);
        if(objeto) {
            foreach(QGraphicsItem * subItem, objeto->getToDelete()){
                //if (subItem->Type == Texto::Type)
                //{
                    tam = Texto::listaTextos.size();
                    for (i=0; i<tam; i++)
                    {
                        if (Texto::listaTextos.at(i) == item)
                        {
                            Texto::listaTextos.removeAt(i);
                            break;
                        }
                    }
                //}

                scene->removeItem(subItem);
                removidos.push_back(subItem);
            }
            objeto->doRemove(true);
            scene->removeItem(item);
        }
    }
    this->deleted = true;
}

void AcaoDeletar::desfazerAcao()
{
    foreach(QGraphicsItem * item, items) {
        scene->addItem(item);

        foreach(QGraphicsItem *removido, removidos) {
            scene->addItem(removido);
            ObjetoRemovivel * objeto = dynamic_cast<ObjetoRemovivel *>(removido);
            if(objeto) {
                objeto->doRemove(false);
            }
        }
    }
    this->deleted = false;
}

bool parentIsInSet(QSet<QGraphicsItem *> & set, QGraphicsItem * item) {
    if(item == NULL)
        return false;

    QGraphicsItem * parent = item;
    while( (parent = parent->parentItem()) != NULL) //enquanto existir parent
        if(set.contains(parent))
            return true;

    return false;
}

void AcaoDeletar::dispose()
{
    if(!deleted)
        return;

    QSet<QGraphicsItem *> objectsToRemove;
    foreach(QGraphicsItem * item, items) {
        objectsToRemove.insert(item);
        ObjetoRemovivel * objeto = dynamic_cast<ObjetoRemovivel *>(item);
        if(objeto) {
            foreach(QGraphicsItem * subItem, objeto->getToDelete()) // Percorre itens associados que devem ser deletados
                objectsToRemove.insert(subItem);
        }
    }


    foreach(QGraphicsItem * toRemove, objectsToRemove) {
        if(parentIsInSet(objectsToRemove, toRemove)) //Verifica se parentItem ja nao esta no set
           continue;

        QObject * obj = dynamic_cast<QObject *>(toRemove);
        if(obj)
            obj->deleteLater();
        else
            delete obj;
    }
}
