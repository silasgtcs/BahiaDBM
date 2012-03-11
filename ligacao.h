/*Utilizada para criar a linha que liga os poligonos e atributos.*/

#ifndef LIGACOES_H
#define LIGACOES_H

#include <QtGui>
#include <atributo.h>
#include <texto.h>
class Poligono;
class Cardinalidade;

class Ligacao : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    Ligacao( QGraphicsItem *item1, QGraphicsItem *item2, QGraphicsItem *parent = 0 );
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setEntidadeFracaAtiva( bool set ) { this->entidadeFracaAtiva = set; }
    bool getEntidadeFracaAtiva () { return this->entidadeFracaAtiva; }
    QList< Cardinalidade *> getCardinalidades_Associadas() { return cardinalidades_associadas; }
    QList< Poligono *> getEntidades_Associadas() { return entidades_associadas; }
    QList< Poligono *> getRelacionamentos_Associados() { return relacionamentos_associados; }
    QList< Poligono *> getEntidades_Associativas_Associadas() { return entidades_associativas_associadas; }
    QList< Atributo *> getAtributoAssociado() { return atributo_associado; }
    void setCardinalidades_Associadas( Cardinalidade *ca ) { this->cardinalidades_associadas.push_back(ca); }
    void setEntidades_Associadas( Poligono *ea ) { this->entidades_associadas.push_back(ea); }
    void setRelacionamento_Associados( Poligono *ra ) { this->relacionamentos_associados.push_back(ra); }
    void setEntidades_Associativas_Associadas( Poligono *eaa ) { this->entidades_associativas_associadas.push_back(eaa); }
    void setAtributoAssociado( Atributo *aa ) { this->atributo_associado.push_back(aa); }
    void Remove_Cardinalidade_Associada( int index ) { this->cardinalidades_associadas.removeAt(index); }
    void Remove_Entidade_Associada( int index ) { this->entidades_associadas.removeAt(index); }
    void Remove_Relacionamento_Associado( int index ) { this->relacionamentos_associados.removeAt(index); }
    void Remove_Entidade_Associativa_Associada( int index ) { this->entidades_associativas_associadas.removeAt(index); }
    void RemoveAtributoAssociado( int index ) { this->atributo_associado.removeAt(index); }
    void RemoveTodasCardinalidadesAssociadas() { this->cardinalidades_associadas.erase(this->cardinalidades_associadas.begin(), this->cardinalidades_associadas.end()); }
    void RemoveTodasEntidadesAssociadas() { this->entidades_associadas.erase(this->entidades_associadas.begin(), this->entidades_associadas.end()); }
    void RemoveTodosRelacionamentosAssociados() { this->relacionamentos_associados.erase(this->relacionamentos_associados.begin(), this->relacionamentos_associados.end()); }
    void RemoveTodasEntidadesAssociativasAssociadas() { this->entidades_associativas_associadas.begin(), this->entidades_associativas_associadas.end(); }
    void RemoveTodosAtributosAssociados() { this->atributo_associado.erase(this->atributo_associado.begin(), this->atributo_associado.end()); }
    QList< Poligono *> getGeneralizacao_Especializacao_Associada() { return generalizacao_especializacao_associada; }
    void setGeneralizacao_Especializacao_Associada( Poligono *gea ) { this->generalizacao_especializacao_associada.push_back(gea); }
    void Remove_Generalizacao_Especializacao_Associada( int index ) { this->generalizacao_especializacao_associada.removeAt(index); }
    void RemoveTodaGeneralizacaoEspecializacaoAssociada() { this->generalizacao_especializacao_associada.erase(this->generalizacao_especializacao_associada.begin(), this->generalizacao_especializacao_associada.end()); }

    Poligono * getCastItem1P() { return this->castItem1P; }
    Poligono * getCastItem2P() { return this->castItem2P; }
    Atributo * getCastItemA() { return this->castItemA; }
    Cardinalidade * getCardItem() { return this->cardItem; }

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

    Cardinalidade *cardItem;

    Texto *nomeLigacao1, *nomeLigacao2;

    QPointF temp;

    bool entidadeFracaAtiva;

    QList< Cardinalidade * > cardinalidades_associadas;
    QList< Poligono * > entidades_associadas;
    QList< Poligono * > relacionamentos_associados;
    QList < Poligono * > entidades_associativas_associadas;
    QList< Poligono * > generalizacao_especializacao_associada;
    QList< Atributo * > atributo_associado;
signals:
    void linhaAlterada( QLineF nova_linha );

public slots:
    void atualizaPos();
    void atualizaPos2();
};

#endif // LIGACOES_H
