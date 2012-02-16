#ifndef RELACIONAMENTO_H
#define RELACIONAMENTO_H

#include <QtGui>
#include <ligacao.h>
class Cardinalidade;

class Poligono : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT

public:
    enum Tipo { entidade, relacionamento, gen_esp, ent_associativa };
    Poligono(Tipo tipo, bool pai, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setTipo(Tipo tipo) { this->tipo = tipo; }
    Tipo getTipo() { return tipo; }
    bool getConectado() { return this->conectado; }
    void setConectado( bool c ) { this->conectado = c; }
    bool getAutoRelacionamento() { return this->autoRelacionamento; }
    void setAutoRelacionamento( bool a ) { this->autoRelacionamento = a; }
    void atualizaItem();
    void setPoligonoAssociado(Poligono * pol) { polig_associado = pol; }
    void itemRemovido();
    Poligono * getPoligonoAssociado() { return polig_associado; }
    QList< Cardinalidade *> getCardinalidades_Associadas() { return cardinalidades_associadas; }
    QList< Poligono *> getEntidades_Associadas() { return entidades_associadas; }
    QList< Poligono *> getRelacionamentos_Associados() { return relacionamentos_associados; }
    QList< Ligacao *> getLinhas_Associadas() { return linhas_associadas; }
    QList< Poligono *> getEntidades_Associativas_Associadas() { return entidades_associativas_associadas; }
    void setCardinalidades_Associadas( Cardinalidade *ca ) { this->cardinalidades_associadas.push_back(ca); }
    void setEntidades_Associadas( Poligono *ea ) { this->entidades_associadas.push_back(ea); }
    void setRelacionamentos_Associados( Poligono *ra ) { this->relacionamentos_associados.push_back(ra); }
    void setLinhas_Associadas( Ligacao *la ) { this->linhas_associadas.push_back(la); }
    void setEntidades_Associativas_Associadas( Poligono *eaa ) { this->entidades_associativas_associadas.push_back(eaa); }

    void Remove_Cardinalidade_Associada( int index ) { this->cardinalidades_associadas.removeAt(index); }
    void Remove_Entidade_Associada( int index ) { this->entidades_associadas.removeAt(index); }
    void Remove_Relacionamento_Associado( int index ) { this->relacionamentos_associados.removeAt(index); }
    void Remove_Linha_Associada( int index ) { this->linhas_associadas.removeAt(index); }
    void Remove_Entidade_Associativa_Associada( int index ) { this->entidades_associativas_associadas.removeAt(index); }

    void RemoveTodasCardinalidadesAssociadas() { this->cardinalidades_associadas.erase(this->cardinalidades_associadas.begin(), this->cardinalidades_associadas.end()); }
    void RemoveTodasEntidadesAssociadas() { this->entidades_associadas.erase(this->entidades_associadas.begin(), this->entidades_associadas.end()); }
    void RemoveTodosRelacionamentosAssociados() { this->relacionamentos_associados.erase(this->relacionamentos_associados.begin(), this->relacionamentos_associados.end()); }
    void RemoveTodasLinhasAssociadas() { this->linhas_associadas.erase(this->linhas_associadas.begin(), this->linhas_associadas.end()); }
    void RemoveTodasEntidadesAssociativasAssociadas() { this->entidades_associativas_associadas.erase(this->entidades_associativas_associadas.begin(), this->entidades_associativas_associadas.end()); }

    // Garante tipo unico para objeto desse tipo
    enum { Type = UserType + 1 };
    int type() const
    {
        return Type;
    }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void posicaoAlterada();
    void posicaoAlterada(const QPointF newPos);
    void removido();

public slots:
    void acompanharObjeto(const QPointF objNewPos);
    void remover();

private:
    QPolygonF poligono;
    Tipo tipo;

    bool conectado;
    bool autoRelacionamento;
    Poligono * polig_associado;

    //Necessário para verificar os itens associados à determinada instância de outros itens.
    QList< Cardinalidade * > cardinalidades_associadas;
    QList< Poligono * > entidades_associadas;
    QList< Poligono * > relacionamentos_associados;
    QList< Ligacao * > linhas_associadas;
    QList< Poligono * > entidades_associativas_associadas;
};

#endif // RELACIONAMENTO_H
