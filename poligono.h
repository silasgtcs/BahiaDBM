/*Utilizada para criar entidade, relacionamento, generalização/especialização e entidade associativa.*/

#ifndef RELACIONAMENTO_H
#define RELACIONAMENTO_H

#include <QtGui>
#include <objetoremovivel.h>
class Atributo;
class Cardinalidade;
class Ligacao;

class Poligono : public QObject, public QGraphicsPolygonItem, public ObjetoRemovivel
{
    Q_OBJECT

public:
    enum Tipo { entidade, relacionamento, gen_esp, ent_associativa };
    Poligono(Tipo tipo, bool pai, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    virtual void doRemove(bool value);
    virtual QList<QGraphicsItem *> getToDelete();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setTipo(Tipo tipo) { this->tipo = tipo; }
    Tipo getTipo() { return tipo; }

    bool getConectado() { return this->conectado; }
    void setConectado( bool c ) { this->conectado = c; }

    bool getAutoRelacionamento() { return this->autoRelacionamento; }
    void setAutoRelacionamento( bool a ) { this->autoRelacionamento = a; }

    void atualizaItem();
    void itemRemovido();

    void setPoligonoPai(Poligono * pol) { poligonoPai = pol; }
    Poligono * getPoligonoPai() { return poligonoPai; }

    void setPoligonoFilho(Poligono * filho) { poligonoFilho = filho; }
    Poligono * getPoligonoFilho() { return poligonoFilho; }

    QList<Atributo *> getAtributosAssociados() { return atributosAssociados.toList(); }
    QList< Cardinalidade *> getCardinalidadesAssociadas() { return cardinalidades_associadas.toList(); }
    QList< Ligacao *> getLinhasAssociadas() { return linhas_associadas.toList(); }

    void addCardinalidadesAssociadas( Cardinalidade *ca ) { this->cardinalidades_associadas.insert(ca); }
    void addLinhasAssociadas( Ligacao *la ) { this->linhas_associadas.insert(la); }
    void addAtributosAssociados( Atributo *aa ) { this->atributosAssociados.insert(aa); }

    void limparCardinalidadesAssociadas() { this->cardinalidades_associadas.clear(); }
    void limparLinhasAssociadas() { this->linhas_associadas.clear(); }
    void limparAtributosAssociados() {this->atributosAssociados.clear(); }

    void removerCardinalidadeAssociada(Cardinalidade * it) { cardinalidades_associadas.remove(it); }
    void removerLinhaAssociada(Ligacao * it) { linhas_associadas.remove(it); }
    void removerAtributoAssociada(Atributo * it) { atributosAssociados.remove(it); }

    void removerPoligonoAssociado(Poligono * poligono) {
        poligonosAssociados[poligono->getTipo()].remove(poligono);
    }

    template<Poligono::Tipo T>
    void limparPoligonosOfType() {
        poligonosAssociados[T].clear();
    }

    void addPoligonoAssociado(Poligono * pol) {
        poligonosAssociados[pol->getTipo()].insert(pol);
    }

    template<Poligono::Tipo T>
    QList<Poligono *> getPoligonosAssociadoOfType() {
       return poligonosAssociados[T].toList();
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
    Poligono * poligonoPai;
    Poligono * poligonoFilho;

    //Necessário para verificar os itens associados à determinada instância de outros itens.
    QSet< Cardinalidade * > cardinalidades_associadas;
    QSet< Ligacao * > linhas_associadas;
    QSet< Atributo * > atributosAssociados;
    QMap<Tipo, QSet<Poligono *> > poligonosAssociados;
};

#endif // RELACIONAMENTO_H
