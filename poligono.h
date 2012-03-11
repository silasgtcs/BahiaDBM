/*Utilizada para criar entidade, relacionamento, generalização/especialização e entidade associativa.*/

#ifndef RELACIONAMENTO_H
#define RELACIONAMENTO_H

#include <QtGui>
#include <ligacao.h>
#include <atributo.h>
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
    QList<Atributo *> getAtributosAssociados() { return atributosAssociados; }
    QList< Cardinalidade *> getCardinalidades_Associadas() { return cardinalidades_associadas; }
    QList< Ligacao *> getLinhas_Associadas() { return linhas_associadas; }

    void setCardinalidades_Associadas( Cardinalidade *ca ) { this->cardinalidades_associadas.push_back(ca); }
    void setLinhas_Associadas( Ligacao *la ) { this->linhas_associadas.push_back(la); }
    void setAtributosAssociados( Atributo *aa ) { this->atributosAssociados.push_back(aa); }

    void remove_Cardinalidade_Associada( int index ) { this->cardinalidades_associadas.removeAt(index); }
    void remove_Linha_Associada( int index ) { this->linhas_associadas.removeAt(index); }
    void removeAtributoAssociado( int index ) { this->atributosAssociados.removeAt(index); }

    void removeTodasCardinalidadesAssociadas() { this->cardinalidades_associadas.erase(this->cardinalidades_associadas.begin(), this->cardinalidades_associadas.end()); }
    void removeTodasLinhasAssociadas() { this->linhas_associadas.erase(this->linhas_associadas.begin(), this->linhas_associadas.end()); }
    void removeTodosAtributosAssociados() {this->atributosAssociados.erase( this->atributosAssociados.begin(), this->atributosAssociados.end()); }

    void addPoligonosAssociado(Poligono * pol) {
        poligonosAssociados[pol->getTipo()].push_back(pol);
    }

    template<Tipo T>
    void removePoligonosAssociadoAt(int index) {
        getPoligonoAssociados<T>().removeAt(index);
    }

    template<Tipo T>
    void removeTodosPoligonosAssociados() {
        QList<Poligono *> pols = getPoligonoAssociados<T>();
        pols.erase(pols.begin(), pols.end());
    }

    template<Tipo T>
    QList<Poligono *> getPoligonoAssociados() {
        QList<Poligono *> res = poligonosAssociados[T];
        return res;
    }

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
    QList< Ligacao * > linhas_associadas;
    QList< Atributo * > atributosAssociados;
    QMap<Tipo, QList<Poligono *> > poligonosAssociados;
};

#endif // RELACIONAMENTO_H
