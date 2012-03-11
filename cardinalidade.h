/*Classe utilizada para criar cardinalidade. O objeto é chamado no momento em que a ligação é feita entre os poligonos.*/

#ifndef CARDINALIDADE_H
#define CARDINALIDADE_H

#include <QtGui>
#include <poligono.h>
#include <ligacao.h>
#include <dialog.h>

class Cardinalidade : public QGraphicsTextItem
{
    Q_OBJECT
public:
    Cardinalidade(Poligono *item1, Poligono *item2, Ligacao *linha, QGraphicsItem *parent = 0);
    void setItemA ( Poligono *item ) { this->itemA = item; }
    void setItemB ( Poligono *item ) { this->itemB = item; }
    void setLinha_Atual ( Ligacao *linha ) { this->linha_atual = linha; }
    QString getAtualCardinalidade() { return this->atualCardinalidade; }
    void setAtualCardinalidade( QString c ) { this->atualCardinalidade = c; }
    QList< Poligono *> getEntidades_Associadas() { return entidades_associadas; }
    QList< Poligono *> getRelacionamentos_Associados() { return relacionamentos_associados; }
    QList< Poligono *> getEntidades_Associativas_Associadas() { return entidades_associativas_associadas; }
    void setEntidades_Associadas( Poligono *ea ) { this->entidades_associadas.push_back(ea); }
    void setRelacionamento_Associados( Poligono *ra ) { this->relacionamentos_associados.push_back(ra); }
    void setEntidades_Associativas_Associadas( Poligono *eaa ) { this->entidades_associativas_associadas.push_back(eaa); }
    void Remove_Entidade_Associada( int index ) { this->entidades_associadas.removeAt(index); }
    void Remove_Relacionamento_Associado( int index ) { this->relacionamentos_associados.removeAt(index); }
    void Remove_Entidade_Associativa_Associada( int index ) { this->entidades_associativas_associadas.removeAt(index); }
    void RemoveTodasEntidadesAssociadas() { this->entidades_associadas.erase(this->entidades_associadas.begin(), this->entidades_associadas.end()); }
    void RemoveTodosRelacionamentosAssociados() { this->relacionamentos_associados.erase(this->relacionamentos_associados.begin(), this->relacionamentos_associados.end()); }
    void RemoveTodasEntidadesAssociativasAssociadas() { this->entidades_associativas_associadas.erase(this->entidades_associativas_associadas.begin(), this->entidades_associativas_associadas.end()); }

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    Poligono *itemA;
    Poligono *itemB;
    Ligacao *linha_atual;
    Dialog chamarTelaCardinalidade;
    QString atualCardinalidade;

    QList< Poligono * > entidades_associadas;
    QList< Poligono * > relacionamentos_associados;
    QList< Poligono * > entidades_associativas_associadas;
public slots:
    void atualizaPos();
};

#endif // CARDINALIDADE_H
