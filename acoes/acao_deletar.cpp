#include "acao_deletar.h"
#include "objetoremovivel.h"
#include "poligono.h"
#include <exception>

AcaoDeletar::AcaoDeletar(QGraphicsScene * scene, QList<QGraphicsItem *> selecionados)
{
    this->items = selecionados;
    this->scene = scene;
    this->deleted = false;
}

void AcaoDeletar::fazerAcao()
{
    removidos.clear();
    foreach(QGraphicsItem * item, items) {
        scene->removeItem(item);
        ObjetoRemovivel * objeto = dynamic_cast<ObjetoRemovivel *>(item);
        if(objeto) {
            foreach(QGraphicsItem * subItem, objeto->getToDelete()){
                scene->removeItem(subItem);
                removidos.push_back(subItem);
            }
            objeto->doRemove(true);
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

        Poligono * pol = qgraphicsitem_cast<Poligono *>(item);
        if(pol)
            if(pol->getPoligonoAssociado())
                objectsToRemove.insert(pol->getPoligonoAssociado());
    }
    foreach(QGraphicsItem * toRemove, objectsToRemove) {
        if(parentIsInSet(objectsToRemove, toRemove)) //Verifica se parentItem ja nao esta no set
           continue;
        Poligono * pol = qgraphicsitem_cast<Poligono *>(toRemove);
        if(pol){
            if(objectsToRemove.contains(pol->getPoligonoAssociado()))
                continue;
        }
        QObject * obj = dynamic_cast<QObject *>(toRemove);
        if(obj)
            obj->deleteLater();
        else
            delete obj;
    }
}
