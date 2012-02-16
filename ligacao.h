#ifndef LIGACOES_H
#define LIGACOES_H

#include <QtGui>
#include <atributo.h>
class Poligono;

class Ligacao : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    Ligacao( QGraphicsItem *item1, QGraphicsItem *item2, QGraphicsItem *parent = 0 );
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setEntidadeFracaAtiva( bool set ) { this->entidadeFracaAtiva = set; }
    bool getEntidadeFracaAtiva () { return this->entidadeFracaAtiva; }
    QList< QGraphicsTextItem *> getCardinalidades_Associadas() { return cardinalidades_associadas; }
    QList< Poligono *> getEntidades_Associadas() { return entidades_associadas; }
    QList< Poligono *> getRelacionamentos_Associados() { return relacionamentos_associados; }
    QList< Poligono *> getEntidades_Associativas_Associadas() { return entidades_associativas_associadas; }
    void setCardinalidades_Associadas( QGraphicsTextItem *ca ) { this->cardinalidades_associadas.push_back(ca); }
    void setEntidades_Associadas( Poligono *ea ) { this->entidades_associadas.push_back(ea); }
    void setRelacionamento_Associados( Poligono *ra ) { this->relacionamentos_associados.push_back(ra); }
    void setEntidades_Associativas_Associadas( Poligono *eaa ) { this->entidades_associativas_associadas.push_back(eaa); }
    void Remove_Cardinalidade_Associada( int index ) { this->cardinalidades_associadas.removeAt(index); }
    void Remove_Entidade_Associada( int index ) { this->entidades_associadas.removeAt(index); }
    void Remove_Relacionamento_Associado( int index ) { this->relacionamentos_associados.removeAt(index); }
    void Remove_Entidade_Associativa_Associada( int index ) { this->entidades_associativas_associadas.removeAt(index); }
    void RemoveTodasCardinalidadesAssociadas() { this->cardinalidades_associadas.erase(this->cardinalidades_associadas.begin(), this->cardinalidades_associadas.end()); }
    void RemoveTodasEntidadesAssociadas() { this->entidades_associadas.erase(this->entidades_associadas.begin(), this->entidades_associadas.end()); }
    void RemoveTodosRelacionamentosAssociados() { this->relacionamentos_associados.erase(this->relacionamentos_associados.begin(), this->relacionamentos_associados.end()); }
    void RemoveTodasEntidadesAssociativasAssociadas() { this->entidades_associativas_associadas.begin(), this->entidades_associativas_associadas.end(); }

    // Garante tipo unico para objeto desse tipo
    enum { Type = UserType + 2 };
    int type() const
    {
        return Type;
    }

    QPainterPath shape() const;
private:
    Poligono *castItem1P;
    Poligono *castItem2P;

    Atributo *castItemA;

    QPointF temp;

    bool entidadeFracaAtiva;

    QList< QGraphicsTextItem * > cardinalidades_associadas;
    QList< Poligono * > entidades_associadas;
    QList< Poligono * > relacionamentos_associados;
    QList < Poligono * > entidades_associativas_associadas;
signals:
    void linhaAlterada( QLineF nova_linha );

public slots:
    void atualizaPos();
    void atualizaPos2();
};

#endif // LIGACOES_H
