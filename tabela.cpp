#include "tabela.h"

Tabela::Tabela( QString nomeTitulo, QGraphicsItem *parent, QGraphicsScene *sceneConceitual, QGraphicsScene *sceneLogico )
    : QGraphicsRectItem( parent, sceneLogico )
{
    setFlag(QGraphicsRectItem::ItemIsMovable, true);
    setFlag(QGraphicsRectItem::ItemIsSelectable, true);
    setFlag(QGraphicsRectItem::ItemSendsGeometryChanges, true);

    setQtdAtributo(0);
    this->sceneC = sceneConceitual;
    this->sceneL = sceneLogico;
    tab = new QRectF(0,0,216,25);
    setRect(*tab);

    linha = this->sceneL->addLine(tab->x(),tab->y()+25,tab->x()+tab->width(),tab->y()+25);
    linha->setParentItem(this);

    titulo = new Texto();
    titulo->setParentItem(this);
    titulo->setTabelaLogicoAtiva(true);
    setTitulo(nomeTitulo);

    connect(titulo, SIGNAL(textoTabelaLogicoAlterado(int)), this, SLOT(atualizarTitulo()));
}

void Tabela::alterarNomeConceitualELogico(QString antigo, QString novo)
{
    //Procura todos os textos com nome antigo do projeto conceitual e substitui pelo nome novo.
    QList<QGraphicsItem *> itensC = sceneC->items();
    for ( int i=0; i<itensC.size(); i++ )
        if ( itensC[i]->type() == Texto::Type )
        {
            Texto *txt = qgraphicsitem_cast<Texto *>(itensC[i]);
            if ( txt->toPlainText() == antigo )
            {
                txt->setPlainText(novo);
                break;
            }
        }

    //Procura todos os textos com nome antigo do projeto lógico e substitui pelo nome novo.
    QList<QGraphicsItem *> itensL = sceneL->items();
    for ( int i=0; i<itensL.size(); i++ )
    {
        if ( itensL[i]->type() == Tabela::Type )
        {
            Tabela *tab = qgraphicsitem_cast<Tabela *>(itensL[i]);
            QList<Texto *> lista = tab->getListaAtributo();
            for ( int j=0; j<lista.size(); j++ )
            {
                QString aux = formataAtributo(lista[j]->toPlainText());
                if ( (aux == antigo) && ( (lista[j]->toPlainText().contains("[PK]", Qt::CaseSensitive)) || (lista[j]->toPlainText().contains("[FK]", Qt::CaseSensitive)) ) )
                    tab->atualizarNomeAtributo(novo, lista[j]->toPlainText(), -1);
            }
        }
    }
}

void Tabela::atualizarLargura()
{
    int maior=11;

    if ( getTitulo().size() > maior )
        maior = getTitulo().size();

    for ( int i=0; i<listaAtributo.size(); i++ )
        if ( listaAtributo[i]->toPlainText().size() > maior )
            maior = listaAtributo[i]->toPlainText().size();

    tab->setWidth(maior*8.5);
    setRect(*tab);

    linha->setLine(tab->x(),tab->y()+25,tab->x()+tab->width(),tab->y()+25);
    this->titulo->setPos(tab->x()+(tab->width()/2)-(this->nomeTitulo.size()*3.5),tab->y());
    setMaiorLargura(maior);
}

void Tabela::setTitulo(QString titulo)
{
    this->nomeTitulo = titulo;
    this->titulo->setPlainText(this->nomeTitulo);
    atualizarLargura();
    this->titulo->setPos(tab->x()+(tab->width()/2)-(this->nomeTitulo.size()*3.5),tab->y());
    this->titulo->setTextoTabelaLogico(titulo,-1,false,false);
    this->titulo->setCursor(Qt::PointingHandCursor);
}

void Tabela::atualizarNomeAtributo(QString novoNome, QString nomeAntigo, int pos)
{
    bool pk,fk,nulo;

    pk = (nomeAntigo.contains("[PK]", Qt::CaseSensitive)) ? true : false;
    fk = (nomeAntigo.contains("[FK]", Qt::CaseSensitive)) ? true : false;
    nulo = (nomeAntigo.contains("NOT NULL", Qt::CaseSensitive)) ? false : true;

    if ((pos != -1) && ( listaAtributo[pos]->getTextoTabelaLogico().alterarRestricao ))
        nulo = listaAtributo[pos]->getTextoTabelaLogico().nulo;

    if ( fk ) novoNome.insert(0, "[FK] ");
    if ( pk ) novoNome.insert(0, "[PK] ");
    if ( nulo ) novoNome.insert(novoNome.size(), " \"NULL\"");
    else novoNome.insert(novoNome.size(), " \"NOT NULL\"");

    for ( int i=0; i<listaAtributo.size(); i++ )
        if ( listaAtributo[i]->toPlainText() == nomeAntigo )
        {
            listaAtributo[i]->setPlainText(novoNome);
            break;
        }

    atualizarLargura();
}

QString Tabela::getTitulo()
{
    return this->nomeTitulo;
}

void Tabela::addAtributo(QString nome, bool chavePrimaria, bool chaveEstrangeira, bool nulo)
{
    atributo = new Texto();
    atributo->setTextoTabelaLogico(nome,listaAtributo.size(), !chavePrimaria, nulo);

    if ( chaveEstrangeira )
    {
        nome.insert(0, "[FK] ");
    }

    if ( chavePrimaria )
    {
        nome.insert(0, "[PK] ");
    }

    if ( nulo )
    {
        nome.insert(nome.size(), " \"NULL\"");
    }
    else
    {
        nome.insert(nome.size(), " \"NOT NULL\"");
    }

    atributo->setPlainText(nome);
    atributo->setPos(tab->x(),tab->y()+tab->height());
    atributo->setParentItem(this);
    atributo->setTabelaLogicoAtiva(true);
    atributo->setCursor(Qt::PointingHandCursor);
    listaAtributo.push_back(atributo);
    connect(listaAtributo[listaAtributo.size()-1], SIGNAL(textoTabelaLogicoAlterado(int)), this, SLOT(atualizarAtributo(int)));
    atualizarLargura();

    tab->setHeight(tab->height()+25);
    setRect(*tab);
    setQtdAtributo(getQtdAtributo()+1);
}

//Formata atributo
QString Tabela::formataAtributo(QString nome)
{
    QString nomeFormatado = NULL;
    for ( int i=nome.size()-2; i>=0; i-- )
        if ( nome[i] == '"' )
        {
            for ( int j=i-2;; j-- )
            {
                if ( j == 0 )
                {
                    nomeFormatado.insert(0, nome[j]);
                    break;
                }

                if ( nome[j-1] == ']' )
                    break;

                nomeFormatado.insert(0, nome[j]);
            }

            break;
        }

    return nomeFormatado;
}

void Tabela::atualizarTitulo()
{
    alterarNomeConceitualELogico(getTitulo(), this->titulo->getTextoTabelaLogico().nome);
    setTitulo(this->titulo->getTextoTabelaLogico().nome);
}

void Tabela::atualizarAtributo(int pos)
{
    QString atributoFormatado = formataAtributo(listaAtributo[pos]->toPlainText());
    alterarNomeConceitualELogico(atributoFormatado, listaAtributo[pos]->getTextoTabelaLogico().nome);
    atualizarNomeAtributo(listaAtributo[pos]->getTextoTabelaLogico().nome, listaAtributo[pos]->toPlainText(), pos);
}

QVariant Tabela::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if ((change == QGraphicsItem::ItemPositionChange) || ( change == QGraphicsItem::ItemPositionHasChanged ))
    {
        emit posicaoAlterada();
    }

    return value;
}

void Tabela::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(Qt::white);
    if ( isSelected() )
        painter->setPen(QPen(Qt::red, 2, Qt::DashLine));
    painter->drawRect(*tab);
}
