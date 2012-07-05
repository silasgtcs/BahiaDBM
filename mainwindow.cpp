#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    pilhaDeAcoes = new AcoesPilha();
    connect(pilhaDeAcoes, SIGNAL(changed()), this, SLOT(houveModificacao()));

    createActions();
    createMenu();
    createToolBar();
    criarScene();
    resetWindowState();

    statusBar()->showMessage(tr("Bahia Database Modeler"), 2000);
}

MainWindow::~MainWindow()
{
}

void MainWindow::resetWindowState() {

    if(scroolZoom && viewConceitual) {
        connect(scroolZoom, SIGNAL(valueChanged(int)), viewConceitual, SLOT(setarZoom(int)));
        scroolZoom->setValue(100);

        //Associa scrool ao graphicView(assim wheel do mouse pode ser redirecionado para o scroll)
        viewConceitual->setScrool(scroolZoom);
    }
}

void MainWindow::criarScene()
{
    //Cria diagrama conceitual
    sceneConceitual = new Diagrama(this, pilhaDeAcoes);
    pilhaDeAcoes->setUnchaged();
    sceneConceitual->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(sceneConceitual, SIGNAL(itemInserido()),
            this, SLOT(itemInserido()));
    sceneConceitual->setTipoER(Diagrama::TipoER(7));
    viewConceitual = new DiagramaView(sceneConceitual);

    //Cria diagrama lógico
    sceneLogico = new Diagrama(this, pilhaDeAcoes);
    pilhaDeAcoes->setUnchaged();
    sceneLogico->setSceneRect(QRectF(0, 0, 5000, 5000));
    sceneLogico->setTipoER(Diagrama::TipoER(7));
    viewLogico = new DiagramaView(sceneLogico);

    //    Tabela *t = new Tabela("Funcionario", NULL, sceneLogico);
    //    t->addAtributo("codFuncionario", true, true, true);
    //    t->addAtributo("nome", false, true, false);
    //    t->addAtributo("salario", false, false, true);

    //Cria diagrama físico
    sceneFisico = new Diagrama(this, pilhaDeAcoes);
    pilhaDeAcoes->setUnchaged();
    sceneFisico->setSceneRect(QRectF(0, 0, 5000, 5000));
    sceneFisico->setTipoER(Diagrama::TipoER(7));
    viewFisico = new DiagramaView(sceneFisico);

    //    QHBoxLayout *layout = new QHBoxLayout;
    //    layout->addWidget(view);

    //    QWidget *widget = new QWidget;
    //    widget->setLayout(layout);

    tabWidget = new QTabWidget;
    tabWidget->addTab(viewConceitual, trUtf8("Conceitual"));
    tabWidget->addTab(viewLogico, trUtf8("  Lógico  "));
    tabWidget->addTab(viewFisico, trUtf8("  Físico  "));
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(abaAlterada(const int)));

    setCentralWidget(tabWidget);
    setWindowTitle("BahiaDBM");

    manipulacoes->setEnabled(true);
    mouseLinha->setEnabled(true);
    formas->setEnabled(true);
    exibicaoToolBar->setEnabled(true);
}

void MainWindow::deletarScene()
{
    pilhaDeAcoes->limpar();
    pilhaDeAcoes->setUnchaged();
    sceneConceitual->deleteLater();
    viewConceitual->deleteLater();
    sceneConceitual = NULL;
    viewConceitual = NULL;
    sceneLogico->deleteLater();
    viewLogico->deleteLater();
    sceneLogico = NULL;
    viewLogico = NULL;
    sceneFisico->deleteLater();
    viewFisico->deleteLater();
    sceneFisico = NULL;
    viewFisico = NULL;
    setArquivoAtualTitulo("");

    manipulacoes->setEnabled(false);
    mouseLinha->setEnabled(false);
    formas->setEnabled(false);
    exibicaoToolBar->setEnabled(false);
}

void MainWindow::itemInserido()
{
    botoesER->button(int(sceneConceitual->getTipoER()))->setChecked(false);
    sceneConceitual->setTipoER(Diagrama::TipoER(7)); //Tipo(7) = nenhum
    botoesML->button(0)->setChecked(true);
    botoesML->button(1)->setChecked(false);
}

void MainWindow::botoesERClicked(int id)
{
    QList<QAbstractButton *> botoes = botoesER->buttons();
    foreach ( QAbstractButton *botao, botoes )
        if ( botoesER->button(id) != botao )
            botao->setChecked(false);

    sceneConceitual->setTipoER(Diagrama::TipoER(id));

    /*Inicialmente verifica se botão clicado é o "EF". Caso afirmativo, verifica se existe alguma linha selecionada, e então aumenta
      a sua espessura.*/
    if ( id == 6 )
    {
        sceneConceitual->setTipoER(Diagrama::TipoER(7));

        QList<QGraphicsItem *> entidade = sceneConceitual->selectedItems();

        if ((entidade.length() > 0) && ( entidade[0]->type() == Ligacao::Type ))
        {
            Ligacao * castLinha = qgraphicsitem_cast<Ligacao *>(entidade[0]);
            castLinha->setEntidadeFracaAtiva(true);
            QList<Poligono *> p = castLinha->getPoligonosAssociadoOfType<Poligono::entidade>();
            if ( p.size() == 1 )
                p[0]->setEntidadeFraca(true);
        }
    }
}

void MainWindow::botoesManipClicked(int id)
{
    botoesManip->button(id)->setChecked(false);

    if(id == 0 || id == 1)
    {
        QList<QGraphicsItem *> itens = sceneConceitual->selectedItems();
        for ( int i=0; i<itens.size(); i++ )
        {
            if(itens[i]->type() == Poligono::Type || itens[i]->type() == Atributo::Type || itens[i]->type() == Texto::Type)
            {
                bool parentInList = false;
                QGraphicsItem * parentItem = itens[i]->parentItem();
                while(parentItem != NULL)
                {
                    if(itens.contains(parentItem))
                    {
                        parentInList = true;
                        break;
                    }
                    parentItem = parentItem->parentItem();
                }
                if(!parentInList)
                    itens[i]->setScale(itens[i]->scale() + ((id == 0) ? 0.1 : -0.1));

                //Move item para atualizar linha, caso exista
                itens[i]->setPos(itens[i]->x()+0.1, itens[i]->y()+0.1);
                itens[i]->setPos(itens[i]->x()-0.1, itens[i]->y()-0.1);
            }
        }
    }
    else if(id == 2 || id == 3)
    {
        organizarEntidades(id - 2);
    }
}

void MainWindow::organizarEntidades(int direcao) // 0 = Hor, 1 = Vert
{
    QList<QGraphicsItem*> itemsSelecionados = sceneConceitual->selectedItems();
    QGraphicsItem* item;
    QGraphicsItem* lastItem = NULL;
    QGraphicsItem* firstItem = NULL;

    for(int i = 0; i < itemsSelecionados.size(); i++)
    {
        if(itemsSelecionados.at(i)->type() != Poligono::Type)
        {
            itemsSelecionados.removeAt(i);
            i--; // reflete mudanca de tamanho do vetor
        }
    }

    if(!itemsSelecionados.empty())
    {
        lastItem = firstItem = itemsSelecionados.first();
        if(firstItem != NULL)
        {
            foreach(item, itemsSelecionados)
            {
                if(firstItem == item)
                    continue;

                if(lastItem != NULL)
                {
                    if(direcao == 0)
                        item->setPos(item->pos() + item->mapFromScene(QPointF(lastItem->scenePos().x(), lastItem->scenePos().y()  + lastItem->boundingRect().height() + 100)));
                    else
                        item->setPos(item->pos() + item->mapFromScene(QPointF(lastItem->scenePos().x() + lastItem->boundingRect().width() + 150, lastItem->scenePos().y())));
                }
                lastItem = item;
            }
        }
    }
}

void MainWindow::desfazer()
{
    pilhaDeAcoes->desfazer();
}

void MainWindow::refazer()
{
    pilhaDeAcoes->refazer();
}

void MainWindow::novaJanela()
{
    if (!sceneConceitual)
        criarScene();
    else
    {
        MainWindow *novaJanela = new MainWindow;
        novaJanela->show();
    }
}

void MainWindow::salvarArquivo(const QString nomeArquivo)
{
    /*MAPA:
      QString |_entidade_| OU |_relacionamento_| OU |_generalizacao_especializacao_|  = inicio de um poligono;
        - Posição x do poligono (float);
        - Posição y do poligono (float);
        - Escala do poligono (float);
        - Nome do poligono (QString);
        - Posição x do nome do poligono (float);
        - Posição y do nome do poligono (float);
        - Escala do nome do poligono (float);
      QString |_entidade_associativa_|
        - Posição x da entidade (float)
        - Posição y da entidade (float)
        - Escala da entidade (float)
        - Posição x do relacionamento (float)
        - Posição y do relacionamento (float)
        - Escala do relacionamento (float)
        - Nome do poligono (QString)
        - Posição x do nome do poligono (float)
        - Posição y do nome do poligono (float)
        - Escala do nome do poligono (float)
      QString |_atributo_| = inicio de um atributo;
      QString |_cardinalidade_| = inicio de uma cardinalidade;
      QString |_ligacao_| = inicio de uma ligacao;

      PS:. Cada QString serializada marca o início de um objeto para ser realocado no diagrama.*/

    QFile file(nomeArquivo);

    if ( !file.open(QIODevice::WriteOnly) )
    {
        QMessageBox::warning(this, trUtf8("Erro"),
                             trUtf8("Erro ao salvar arquivo"));
        return;
    }

    QDataStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    //Retorna todos os objetos do diagrama para serializá-los.
    QList<QGraphicsItem *> obj = sceneConceitual->items();
    for ( int i=0; i<obj.size(); i++ )
    {
        if(obj.at(i)->type() == Poligono::Type)
        {
            Poligono * castPoligono = qgraphicsitem_cast<Poligono *>(obj[i]);

            if ( castPoligono->getTipo() == Poligono::Tipo(0) )
            {
                QString nome = "|_entidade_|";
                out << nome << (float)castPoligono->x() << (float)castPoligono->y() << (float)castPoligono->scale();
                QList<QGraphicsItem *> txt = castPoligono->childItems();
                Texto *t = NULL;
                for ( int j=0; j<txt.size(); j++ )
                    if ( txt[j]->type() == Texto::Type )
                    {
                        t = qgraphicsitem_cast<Texto *>(txt[j]);
                        break;
                    }
                if(t)
                    out << t->toPlainText() << (float)t->x() << (float)t->y() << (float)t->scale();
            }

            else if ( castPoligono->getTipo() == Poligono::Tipo(1) )
            {
                QString nome = "|_relacionamento_|";
                out << nome << (float)castPoligono->x() << (float)castPoligono->y() << (float)castPoligono->scale();
                QList<QGraphicsItem *> txt = castPoligono->childItems();
                QList<Texto *> t;
                for ( int j=0; j<txt.size(); j++ )
                    if ( txt[j]->type() == Texto::Type )
                        t.push_back(qgraphicsitem_cast<Texto *>(txt[j]));
                out << t[0]->toPlainText() << (float)t[0]->x() << (float)t[0]->y() << (float)t[0]->scale();

                if ( t.size() > 1 )
                {
                    nome = "|_com_auto_relacionamento_|";
                    out << nome;
                    out << t[1]->toPlainText() << (float)t[1]->x() << (float)t[1]->y() << (float)t[1]->scale();
                    out << t[2]->toPlainText() << (float)t[2]->x() << (float)t[2]->y() << (float)t[2]->scale();
                }
                else
                {
                    nome = "|_sem_auto_relacionamento_|";
                    out << nome;
                }
            }

            else if ( castPoligono->getTipo() == Poligono::Tipo(2) )
            {
                QString nome = "|_generalizacao_especializacao_|";
                out << nome << (float)castPoligono->x() << (float)castPoligono->y() << (float)castPoligono->scale();
                QList<QGraphicsItem *> txt = castPoligono->childItems();
                Texto *t = NULL;
                for ( int j=0; j<txt.size(); j++ )
                    if ( txt[j]->type() == Texto::Type )
                    {
                        t = qgraphicsitem_cast<Texto *>(txt[j]);
                        break;
                    }
                if(t)
                    out << t->toPlainText() << (float)t->x() << (float)t->y() << (float)t->scale();
            }

            else if ( castPoligono->getTipo() == Poligono::Tipo(3) )
            {
                if ( castPoligono->getPoligonoPai() > 0 )
                {
                    QString nome = "|_entidade_associativa_|";
                    out << nome << castPoligono->getPoligonoPai()->x() << castPoligono->getPoligonoPai()->y() << castPoligono->getPoligonoPai()->scale();
                    QList<QGraphicsItem *> txt = castPoligono->childItems();
                    Texto *t = NULL;
                    for ( int j=0; j<txt.size(); j++ )
                        if ( txt[j]->type() == Texto::Type )
                        {
                            t = qgraphicsitem_cast<Texto *>(txt[j]);
                            break;
                        }
                    if(t)
                        out << t->toPlainText();
                }
            }
        }

        else if (obj.at(i)->type() == Atributo::Type)
        {
            Atributo * castAtributo2 = qgraphicsitem_cast<Atributo *>(obj[i]);

            if ( castAtributo2->getTipo() == Atributo::atributo )
            {
                QString nome = "|_atributo_|";
                out << nome << (float)castAtributo2->scenePos().x() << (float)castAtributo2->scenePos().y() << (float)castAtributo2->scale();
                QList<QGraphicsItem *> txt = castAtributo2->childItems();
                Texto *t = NULL;
                for ( int j=0; j<txt.size(); j++ )
                    if ( txt[j]->type() == Texto::Type )
                    {
                        t = qgraphicsitem_cast<Texto *>(txt[j]);
                        break;
                    }
                if(t)
                    out << t->toPlainText() << (float)t->x() << (float)t->y() << (float)t->scale();
            }

            else if ( castAtributo2->getTipo() == Atributo::atributo_identif )
            {
                QString nome = "|_atributo_identificador_|";
                out << nome << (float)castAtributo2->scenePos().x() << (float)castAtributo2->scenePos().y() << (float)castAtributo2->scale();
                QList<QGraphicsItem *> txt = castAtributo2->childItems();
                Texto *t = NULL;
                for ( int j=0; j<txt.size(); j++ )
                    if ( txt[j]->type() == Texto::Type )
                    {
                        t = qgraphicsitem_cast<Texto *>(txt[j]);
                        break;
                    }
                if(t)
                    out << t->toPlainText() << (float)t->x() << (float)t->y() << (float)t->scale();
            }
        }
    }

    for ( int i=0; i<obj.size(); i++ )
    {
        if(obj.at(i)->type() == Ligacao::Type)
        {
            Ligacao * castLigacao = qgraphicsitem_cast<Ligacao *>(obj[i]);

            if ( castLigacao->getCardinalidades_Associadas().size() > 0 )
            {
                QString nome = "|_linha_com_cardinalidade_|";
                out << nome;

                if ( castLigacao->getPoligonosAssociadoOfType<Poligono::entidade>().size() > 0 )
                    out << (float)castLigacao->getPoligonosAssociadoOfType<Poligono::entidade>().at(0)->x()
                        << (float)castLigacao->getPoligonosAssociadoOfType<Poligono::entidade>().at(0)->y();
                if ( castLigacao->getPoligonosAssociadoOfType<Poligono::relacionamento>().size() > 0 )
                    out << (float)castLigacao->getPoligonosAssociadoOfType<Poligono::relacionamento>().at(0)->x()
                        << (float)castLigacao->getPoligonosAssociadoOfType<Poligono::relacionamento>().at(0)->y();
                if ( castLigacao->getPoligonosAssociadoOfType<Poligono::ent_associativa>().size() > 0 )
                    out << (float)castLigacao->getPoligonosAssociadoOfType<Poligono::ent_associativa>().at(0)->x()
                        << (float)castLigacao->getPoligonosAssociadoOfType<Poligono::ent_associativa>().at(0)->y();


                out << castLigacao->getEntidadeFracaAtiva() << castLigacao->getCardinalidades_Associadas().at(0)->getCardinalidadeAtual();
            }

            else if ( castLigacao->getCardinalidades_Associadas().size() == 0 )
            {
                QString nome = "|_linha_sem_cardinalidade_|";
                out << nome;

                if ( castLigacao->getAtributoAssociado().size() > 0 )
                {
                    nome = "|_com_atributo_|";
                    out << nome;
                    out << (float)castLigacao->getAtributoAssociado().at(0)->scenePos().x() << (float)castLigacao->getAtributoAssociado().at(0)->scenePos().y();
                    if ( castLigacao->getPoligonosAssociadoOfType<Poligono::entidade>().size() > 0 )
                        out << (float)castLigacao->getPoligonosAssociadoOfType<Poligono::entidade>().at(0)->x()
                            << (float)castLigacao->getPoligonosAssociadoOfType<Poligono::entidade>().at(0)->y();
                    if ( castLigacao->getPoligonosAssociadoOfType<Poligono::relacionamento>().size() > 0 )
                        out << (float)castLigacao->getPoligonosAssociadoOfType<Poligono::relacionamento>().at(0)->x()
                            << (float)castLigacao->getPoligonosAssociadoOfType<Poligono::relacionamento>().at(0)->y();
                    if ( castLigacao->getPoligonosAssociadoOfType<Poligono::ent_associativa>().size() > 0 )
                        out << (float)castLigacao->getPoligonosAssociadoOfType<Poligono::ent_associativa>().at(0)->x()
                            << (float)castLigacao->getPoligonosAssociadoOfType<Poligono::ent_associativa>().at(0)->y();
                }
                else
                {
                    nome = "|_sem_atributo_|";
                    out << nome;
                    out << (float)castLigacao->getPoligonosAssociadoOfType<Poligono::gen_esp>().at(0)->x()
                        << (float)castLigacao->getPoligonosAssociadoOfType<Poligono::gen_esp>().at(0)->y();
                    if ( castLigacao->getPoligonosAssociadoOfType<Poligono::entidade>().size() > 0 )
                        out << (float)castLigacao->getPoligonosAssociadoOfType<Poligono::entidade>().at(0)->x()
                            << (float)castLigacao->getPoligonosAssociadoOfType<Poligono::entidade>().at(0)->y();
                }

                out << castLigacao->getEntidadeFracaAtiva();
            }
        }
    }

    QList<QGraphicsItem *> obj2 = sceneLogico->items();

    for ( int i=0; i<obj2.size(); i++ )
    {
        if ( obj2[i]->type() == Tabela::Type )
        {
            Tabela *tab = qgraphicsitem_cast<Tabela *>(obj2[i]);
            QString temp = "|_tabela_logica_|";

            out << temp;
            out << tab->getTitulo();
            out << (float)tab->x() << (float)tab->y();

            for ( int j=0; j<tab->getListaAtributo().size(); j++ )
                out << tab->getListaAtributo().at(j)->toPlainText();

            temp = "|_fim_tabela_logica_|";
            out << temp;
        }
    }

    for ( int i=0; i<obj2.size(); i++ )
    {
        if ( obj2[i]->type() == Ligacao::Type )
        {
            Ligacao *lg = qgraphicsitem_cast<Ligacao *>(obj2[i]);
            QString temp = "|_linha_tabela_logica_|";
            out << temp;
            out << lg->getTabelasAssociadas().first->getTitulo() << lg->getTabelasAssociadas().second->getTitulo();
        }
    }

    pilhaDeAcoes->setUnchaged();
    setArquivoAtual(nomeArquivo);
    setArquivoAtualTitulo(diminuirNome(nomeArquivo));
    QApplication::restoreOverrideCursor();

    statusBar()->showMessage(trUtf8("Arquivo salvo com sucesso!"), 2000);
}

void MainWindow::salvar()
{
    if ( nomeArquivoAtual.isEmpty() )
        salvarComo();
    else
        salvarArquivo(nomeArquivoAtual);
}

void MainWindow::abrirArquivo(const QString nomeArquivo)
{
    QFile file(nomeArquivo);

    if ( !file.open( QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this, trUtf8("Erro"),
                             trUtf8("Erro ao abrir arquivo"));
        return;
    }

    if ( sceneConceitual )
        deletarScene();

    QDataStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    criarScene();
    resetWindowState();

    Poligono *abrirPoligono;
    Atributo *abrirAtributo;
    Texto *abrirTexto;
    Texto *abrirAutoRelacionamento1;
    Texto *abrirAutoRelacionamento2;
    Cardinalidade *abrirCardinalidade;

    while(!in.atEnd())
    {
        QString head;
        in >> head;
        if ( head == "|_entidade_|" )
        {
            abrirPoligono = new Poligono(Poligono::entidade,true,0,sceneConceitual);
            abrirTexto = new Texto();
            abrirTexto->setTextInteractionFlags(Qt::TextEditorInteraction);
            sceneConceitual->addItem(abrirTexto);
            abrirTexto->setFocus();
            abrirTexto->setParentItem(abrirPoligono);
            float x, y, s;
            in >> x >> y >> s;
            abrirPoligono->setPos(x,y);
            abrirPoligono->setScale(s);
            QString t;
            in >> t >> x >> y >> s;
            abrirTexto->setPlainText(t);
            abrirTexto->setPos(x,y);
            abrirTexto->setScale(s);
        }

        else if ( head == "|_relacionamento_|" )
        {
            abrirPoligono = new Poligono(Poligono::relacionamento,true,0,sceneConceitual);
            abrirTexto = new Texto();
            abrirTexto->setTextInteractionFlags(Qt::TextEditorInteraction);
            sceneConceitual->addItem(abrirTexto);
            abrirTexto->setFocus();
            abrirTexto->setParentItem(abrirPoligono);
            float x, y, s;
            in >> x >> y >> s;
            abrirPoligono->setPos(x,y);
            abrirPoligono->setScale(s);
            QString t;
            in >> t >> x >> y >> s;
            abrirTexto->setPlainText(t);
            abrirTexto->setPos(x,y);
            abrirTexto->setScale(s);
            in >> t;
            if ( t == "|_com_auto_relacionamento_|" )
            {
                QString nome1, nome2;
                float x1, y1, s1, x2, y2, s2;
                in >> nome1 >> x1 >> y1 >> s1;
                in >> nome2 >> x2 >> y2 >> s2;
                abrirAutoRelacionamento1 = new Texto();
                abrirAutoRelacionamento2 = new Texto();
                abrirAutoRelacionamento1->setTextInteractionFlags(Qt::TextEditorInteraction);
                abrirAutoRelacionamento2->setTextInteractionFlags(Qt::TextEditorInteraction);
                sceneConceitual->addItem(abrirAutoRelacionamento1);
                sceneConceitual->addItem(abrirAutoRelacionamento2);
                abrirAutoRelacionamento1->setParentItem(abrirPoligono);
                abrirAutoRelacionamento2->setParentItem(abrirPoligono);
                abrirAutoRelacionamento1->setPos(x1, y1);
                abrirAutoRelacionamento1->setScale(s1);
                abrirAutoRelacionamento1->setPlainText(nome1);
                abrirAutoRelacionamento2->setPos(x2, y2);
                abrirAutoRelacionamento2->setScale(s2);
                abrirAutoRelacionamento2->setPlainText(nome2);
            }
        }

        else if ( head == "|_generalizacao_especializacao_|" )
        {
            abrirPoligono = new Poligono(Poligono::gen_esp,true,0,sceneConceitual);
            abrirTexto = new Texto();
            abrirTexto->setFlag(QGraphicsItem::ItemIsMovable, false);
            abrirTexto->setFlag(QGraphicsItem::ItemIsSelectable, false);
            abrirTexto->setCursor(Qt::PointingHandCursor);
            sceneConceitual->addItem(abrirTexto);
            abrirTexto->setFocus();
            abrirTexto->setParentItem(abrirPoligono);
            abrirTexto->setGenEspAtiva(true);
            float x, y, s;
            in >> x >> y >> s;
            abrirPoligono->setPos(x,y);
            abrirPoligono->setScale(s);
            QString t;
            in >> t >> x >> y >> s;
            abrirTexto->setPos(x,y);
            abrirTexto->setScale(s);
            abrirTexto->setTipoGenEsp(t);
            abrirTexto->setPlainText(abrirTexto->getTipoGenEsp());
        }

        else if ( head == "|_entidade_associativa_|" )
        {
            float x,y,s;
            QString txt;
            in >> x >> y >> s >> txt;

            QPointF posicao;
            posicao.setX(x);
            posicao.setY(y);
            AcaoCriarPoligono *acao = new AcaoCriarPoligono(sceneConceitual, Poligono::ent_associativa, txt, posicao);
            acao->fazerAcao();
            acao->getPoligono()->setScale(s);
        }

        else if ( head == "|_atributo_|" )
        {
            abrirAtributo = new Atributo(Atributo::atributo,0,sceneConceitual);
            abrirTexto = new Texto();
            sceneConceitual->addItem(abrirTexto);
            abrirTexto->setFocus();
            abrirTexto->setParentItem(abrirAtributo);
            float x, y, s;
            in >> x >> y >> s;
            abrirAtributo->setPos(x,y);
            abrirAtributo->setScale(s);
            QString t;
            in >> t >> x >> y >> s;
            abrirTexto->setPlainText(t);
            abrirTexto->setPos(x,y);
            abrirTexto->setScale(s);
        }

        else if ( head == "|_atributo_identificador_|" )
        {
            abrirAtributo = new Atributo(Atributo::atributo_identif,0,sceneConceitual);
            abrirTexto = new Texto();
            sceneConceitual->addItem(abrirTexto);
            abrirTexto->setFocus();
            abrirTexto->setParentItem(abrirAtributo);
            float x, y, s;
            in >> x >> y >> s;
            abrirAtributo->setPos(x,y);
            abrirAtributo->setScale(s);
            QString t;
            in >> t >> x >> y >> s;
            abrirTexto->setPlainText(t);
            abrirTexto->setPos(x,y);
            abrirTexto->setScale(s);
        }

        else if ( head == "|_linha_com_cardinalidade_|" )
        {
            float x1, y1, x2, y2;
            in >> x1 >> y1 >> x2 >> y2;
            QGraphicsItem *item[2];
            int count = 0;

            QList<QGraphicsItem *> obj = sceneConceitual->items();
            for ( int i=0; i<obj.size(); i++ )
            {
                if(obj.at(i)->type() == Poligono::Type)
                {
                    if (( (float)obj[i]->x() == x1 ) && ( (float)obj[i]->y() == y1 ))
                        item[count++] = obj[i];
                    if (( (float)obj[i]->x() == x2 ) && ( (float)obj[i]->y() == y2 ))
                        item[count++] = obj[i];
                }
            }

            Ligacao *ligacao = new Ligacao( item[0], item[1] );
            ligacao->setZValue(-1000.0);
            sceneConceitual->addItem(ligacao);

            bool entidadeFraca;
            QString card;
            in >> entidadeFraca >> card;

            Poligono * castPoligono = qgraphicsitem_cast<Poligono *>(item[0]);
            Poligono * castPoligono2 = qgraphicsitem_cast<Poligono *>(item[1]);

            if (( castPoligono->getTipo() == Poligono::ent_associativa ) || ( castPoligono2->getTipo() == Poligono::ent_associativa ))
                if(( castPoligono->getTipo() == Poligono::entidade ) || ( castPoligono2->getTipo() == Poligono::entidade ))
                    ligacao->setZValue(-250);

            abrirCardinalidade = new Cardinalidade(castPoligono, castPoligono2, ligacao);
            abrirCardinalidade->setCursor(Qt::PointingHandCursor);
            abrirCardinalidade->setCardinalidadeAtual(card);

            castPoligono->addCardinalidadesAssociadas(abrirCardinalidade);
            castPoligono2->addCardinalidadesAssociadas(abrirCardinalidade);
            ligacao->addCardinalidadeAssociada(abrirCardinalidade);
            ligacao->setEntidadeFracaAtiva(entidadeFraca);

            if ( castPoligono->getTipo() == Poligono::entidade )
                castPoligono->setEntidadeFraca(entidadeFraca);
            if ( castPoligono2->getTipo() == Poligono::entidade )
                castPoligono2->setEntidadeFraca(entidadeFraca);

            abrirCardinalidade->addPoligonoAssociado(castPoligono);
            abrirCardinalidade->addPoligonoAssociado(castPoligono2);

            sceneConceitual->addItem(abrirCardinalidade);

            item[0]->setPos(item[0]->x()+0.1, item[0]->y()+0.1);
            item[0]->setPos(item[0]->x()-0.1, item[0]->y()-0.1);
        }

        else if ( head == "|_linha_sem_cardinalidade_|" )
        {
            QString nome;
            in >> nome;
            float x1, y1, x2, y2;
            in >> x1 >> y1 >> x2 >> y2;
            QGraphicsItem *item[2];
            int count = 0;

            if ( nome == "|_com_atributo_|" )
            {
                QList<QGraphicsItem *> obj = sceneConceitual->items();
                for ( int i=0; i<obj.size(); i++ )
                {
                    if((obj.at(i)->type() == Poligono::Type) || (obj.at(i)->type() == Atributo::Type))
                    {
                        if (( (float)obj[i]->scenePos().x() == x1 ) && ( (float)obj[i]->scenePos().y() == y1 ))
                            item[count++] = obj[i];
                        if (( (float)obj[i]->scenePos().x() == x2 ) && ( (float)obj[i]->scenePos().y() == y2 ))
                            item[count++] = obj[i];
                    }
                }
            }
            else
            {
                QList<QGraphicsItem *> obj = sceneConceitual->items();
                for ( int i=0; i<obj.size(); i++ )
                {
                    if(obj.at(i)->type() == Poligono::Type)
                    {
                        if (( (float)obj[i]->x() == x1 ) && ( (float)obj[i]->y() == y1 ))
                            item[count++] = obj[i];
                        if (( (float)obj[i]->x() == x2 ) && ( (float)obj[i]->y() == y2 ))
                            item[count++] = obj[i];
                    }
                }
            }

            Ligacao *ligacao = new Ligacao( item[0], item[1] );
            ligacao->setZValue(-1000.0);
            sceneConceitual->addItem(ligacao);

            bool entidadeFraca;
            in >> entidadeFraca;
            ligacao->setEntidadeFracaAtiva(entidadeFraca);

            item[0]->setPos(item[0]->x()+0.1, item[0]->y()+0.1);
            item[0]->setPos(item[0]->x()-0.1, item[0]->y()-0.1);
        }

        else if ( head == "|_tabela_logica_|" )
        {
            QString nome;
            float x, y;

            in >> nome;
            in >> x >> y;

            tab1 = new Tabela(nome, NULL, sceneConceitual, sceneLogico);
            tab1->setPos(x,y);

            in >> nome;
            while ( nome != "|_fim_tabela_logica_|" )
            {
                bool pk,fk,nulo;

                pk = (nome.contains("[PK]", Qt::CaseSensitive)) ? true : false;
                fk = (nome.contains("[FK]", Qt::CaseSensitive)) ? true : false;
                nulo = (nome.contains("NOT NULL", Qt::CaseSensitive)) ? false : true;

                tab1->addAtributo(formataAtributo(nome), pk, fk, nulo);
                in >> nome;
            }

            tabelasLogico.push_back(tab1);
        }

        else if ( head == "|_linha_tabela_logica_|" )
        {
            QString nome1, nome2;
            Tabela *tabA, *tabB;

            in >> nome1 >> nome2;
            for ( int w=0; w<tabelasLogico.size(); w++ )
            {
                if ( tabelasLogico[w]->getTitulo() == nome1 )
                    tabA = tabelasLogico[w];
                else if ( tabelasLogico[w]->getTitulo() == nome2 )
                    tabB = tabelasLogico[w];
            }

            AcaoCriarLigacao * acao1 = new AcaoCriarLigacao(sceneLogico, tabA, tabB);
            if ( acao1->getLigacao() )
            {
                pilhaDeAcoes->addAcao(acao1);
                acao1->fazerAcao();
            }
        }
    }

    QApplication::restoreOverrideCursor();

    QString nome = diminuirNome(nomeArquivo);
    setArquivoAtualTitulo(nome);
    setNomeArquivoAtual(nomeArquivo);

    setArquivoAtual(nomeArquivo);
    pilhaDeAcoes->setUnchaged();
    statusBar()->showMessage(trUtf8("Arquivo aberto com sucesso!"), 2000);
}

void MainWindow::abrirArquivoRecente()
{
    if(!questionarSalvar())
        return;
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        abrirArquivo(action->data().toString());
}

void MainWindow::abrir()
{
    if(!questionarSalvar())
        return;
    QString nomeArquivo = QFileDialog::getOpenFileName(this, trUtf8("Abrir..."), "",
                                                       trUtf8("BahiaDBM (*.bdm)"));
    if (!nomeArquivo.isEmpty())
        abrirArquivo(nomeArquivo);
}

void MainWindow::salvarComo()
{
    QString nomeArquivo = QFileDialog::getSaveFileName(this,trUtf8("Save File"),
                                                       "meudiagrama.bdm",
                                                       trUtf8("BahiaDBM (*.bdm)"));
    if (nomeArquivo.isEmpty())
        return;

    salvarArquivo(nomeArquivo);
}

void MainWindow::botoesMLClicked(int id)
{
    QList<QAbstractButton *> botoes = botoesML->buttons();
    foreach ( QAbstractButton *botao, botoes )
        if ( botoesML->button(id) != botao )
            botao->setChecked(false);

    if ( id == 0 ) //Mouse
        sceneConceitual->setTipoER(Diagrama::TipoER(7));
    else if ( id == 1 ) //Linha
        sceneConceitual->setTipoER(Diagrama::TipoER(6));
    else //Remover
        deletarSelecionados();
}

void MainWindow::deletarSelecionados() {
    if(sceneConceitual->selectedItems().size() <= 0)
        return;

    AcaoDeletar * deletar = new AcaoDeletar(this->sceneConceitual, this->sceneConceitual->selectedItems());
    pilhaDeAcoes->addAcao(deletar, true);
}

void MainWindow::sobre()
{
    QMessageBox::about(this, trUtf8("Sobre BahiaDBM"), trUtf8("Bahia Database Modeler"));
}

void MainWindow::createActions()
{
    novaAction = new QAction(trUtf8("&Novo"), this);
    novaAction->setShortcuts(QKeySequence::New);
    novaAction->setStatusTip(trUtf8("Abrir uma nova janela"));
    connect(novaAction, SIGNAL(triggered()), this, SLOT(novaJanela()));

    abrirAction = new QAction(trUtf8("&Abrir..."), this);
    abrirAction->setShortcut(QKeySequence::Open);
    abrirAction->setStatusTip("Abrir um arquivo .bdm");
    connect(abrirAction, SIGNAL(triggered()), this, SLOT(abrir()));

    salvarAction = new QAction(trUtf8("&Salvar"), this);
    salvarAction->setShortcut(QKeySequence::Save);
    salvarAction->setStatusTip("Salvar alterações do projeto atual");
    connect(salvarAction, SIGNAL(triggered()), this, SLOT(salvar()));

    salvarComoAction = new QAction(trUtf8("&Salvar como..."), this);
    salvarComoAction->setShortcut(QKeySequence::SaveAs);
    salvarComoAction->setStatusTip("Salvar projeto atual");
    connect(salvarComoAction, SIGNAL(triggered()), this, SLOT(salvarComo()));

    fecharDiagramaAtual = new QAction(trUtf8("&Fechar Diagrama"), this);
    fecharDiagramaAtual->setShortcut(trUtf8("Ctrl+D"));
    fecharDiagramaAtual->setStatusTip("Fechar diagrama atual");
    connect(fecharDiagramaAtual, SIGNAL(triggered()), this, SLOT(fecharDiagrama()));

    for ( int i=0; i<MAXARQRECENTES; i++ )
    {
        arquivosRecentes[i] = new QAction(this);
        arquivosRecentes[i]->setVisible(false);
        connect(arquivosRecentes[i], SIGNAL(triggered()), this, SLOT(abrirArquivoRecente()));
    }

    sairAction = new QAction(trUtf8("&Sair"), this);
    sairAction->setShortcut(QKeySequence::Quit);
    sairAction->setStatusTip(trUtf8("Sair do BahiaDBM"));
    connect(sairAction, SIGNAL(triggered()), this, SLOT(close()));

    desfazerAction = new QAction(trUtf8("&Desfazer"), this);
    desfazerAction->setShortcut(trUtf8("Ctrl+Z"));
    desfazerAction->setStatusTip(trUtf8("Desfazer"));
    connect(desfazerAction, SIGNAL(triggered()), this, SLOT(desfazer()));

    refazerAction = new QAction(trUtf8("&Refazer"), this);
    refazerAction->setShortcut(trUtf8("Ctrl+R"));
    refazerAction->setStatusTip(trUtf8("Refazer"));
    connect(refazerAction, SIGNAL(triggered()), this, SLOT(refazer()));

    ajudaAction = new QAction(trUtf8("&Sobre"), this);
    ajudaAction->setShortcut(trUtf8("Ctrl+H"));
    ajudaAction->setStatusTip(trUtf8("Sobre BahiaDBM"));
    connect(ajudaAction, SIGNAL(triggered()), this, SLOT(sobre()));

    modeloLogico = new QAction(trUtf8("&Modelo Lógico"), this);
    modeloLogico->setStatusTip(trUtf8("Gerar Modelo Lógico"));
    connect(modeloLogico, SIGNAL(triggered()), this, SLOT(gerarModeloLogico()));

    modeloFisico = new QAction(trUtf8("&Modelo Físico"), this);
    modeloFisico->setStatusTip(trUtf8("Gerar Modelo Físico"));
}

void MainWindow::createMenu()
{
    arquivoMenu = menuBar()->addMenu(trUtf8("&Arquivo"));
    arquivoMenu->addAction(novaAction);
    arquivoMenu->addAction(abrirAction);
    arquivoMenu->addAction(salvarAction);
    arquivoMenu->addAction(salvarComoAction);
    arquivoMenu->addSeparator();
    arquivoMenu->addAction(fecharDiagramaAtual);
    arquivoMenu->addSeparator();
    for ( int i=0; i<MAXARQRECENTES; i++ )
        arquivoMenu->addAction(arquivosRecentes[i]);
    arquivoMenu->addSeparator();
    arquivoMenu->addAction(sairAction);
    atualizarAcaoArquivosRecentes();

    editarMenu = menuBar()->addMenu(trUtf8("&Editar"));
    editarMenu->addAction(desfazerAction);
    editarMenu->addAction(refazerAction);

    gerarMenu = menuBar()->addMenu(trUtf8("&Gerar"));
    gerarMenu->addAction(modeloLogico);
    gerarMenu->addAction(modeloFisico);

    ajudaMenu = menuBar()->addMenu(trUtf8("&Ajuda"));
    ajudaMenu->addAction(ajudaAction);
}

void MainWindow::createToolBar()
{
    //Cria botões que selecionam as figuras geométricas
    entidade = new QToolButton();
    relacionamento = new QToolButton();
    gen_esp = new QToolButton();
    entid_associativa = new QToolButton();
    atributo = new QToolButton();
    atributo_identif = new QToolButton();
    entidadeFraca = new QToolButton();
    entidade->setCheckable(true);
    relacionamento->setCheckable(true);
    gen_esp->setCheckable(true);
    entid_associativa->setCheckable(true);
    atributo->setCheckable(true);
    atributo_identif->setCheckable(true);
    atributo->setToolTip("Atributo");
    entidade->setIcon(QIcon(":/imagens/entidade.png"));
    entidade->setStatusTip(trUtf8("Entidade"));
    entidade->setToolTip("Entidade");
    relacionamento->setIcon(QIcon(":/imagens/relacionamento.png"));
    relacionamento->setStatusTip(trUtf8("Relacionamento"));
    relacionamento->setToolTip("Relacionamento");
    gen_esp->setIcon(QIcon(":/imagens/gen_especializacao.png"));
    gen_esp->setStatusTip(trUtf8("Generalização/Especialização"));
    gen_esp->setToolTip(trUtf8("Generalização Especialização"));
    entid_associativa->setIcon(QIcon(":/imagens/ent_associativa.png"));
    entid_associativa->setStatusTip(trUtf8("Entidade Associativa"));
    entid_associativa->setToolTip(trUtf8("Entidade Associativa"));
    atributo->setIcon(QIcon(":/imagens/atributo.png"));
    atributo->setStatusTip(trUtf8("Atributo"));
    atributo_identif->setIcon(QIcon(":/imagens/atributo_identif.png"));
    atributo_identif->setStatusTip(trUtf8("Atributo Identificador"));
    atributo_identif->setToolTip(trUtf8("Atributo Identificador"));
    entidadeFraca->setText("EF");
    entidadeFraca->setStatusTip(trUtf8("Entidade Fraca"));
    entidadeFraca->setToolTip(trUtf8("Entidade Fraca"));


    botoesER = new QButtonGroup(this);
    botoesER->setExclusive(false);
    botoesER->addButton(entidade,0);
    botoesER->addButton(relacionamento,1);
    botoesER->addButton(gen_esp,2);
    botoesER->addButton(entid_associativa,3);
    botoesER->addButton(atributo,4);
    botoesER->addButton(atributo_identif,5);
    botoesER->addButton(entidadeFraca, 6);
    connect(botoesER, SIGNAL(buttonClicked(int)),
            this, SLOT(botoesERClicked(int)));

    formas = addToolBar(trUtf8("&Formas"));
    formas->addWidget(entidade);
    formas->addWidget(relacionamento);
    formas->addWidget(gen_esp);
    formas->addWidget(entid_associativa);
    formas->addWidget(atributo);
    formas->addWidget(atributo_identif);
    formas->addWidget(entidadeFraca);

    //Cria botões de manipulação das figuras geométricas
    ampliar = new QToolButton();
    ampliar->setCheckable(true);
    ampliar->setStatusTip(trUtf8("Ampliar"));
    ampliar->setIcon(QIcon(":/imagens/ampliar.png"));
    ampliar->setToolTip(trUtf8("Ampliar"));

    reduzir = new QToolButton();
    reduzir->setCheckable(true);
    reduzir->setStatusTip(trUtf8("Reduzir"));
    reduzir->setIcon(QIcon(":/imagens/reduzir.png"));
    reduzir->setToolTip(trUtf8("Reduzir"));

    //    organizarHor = new QToolButton();
    //    organizarHor->setCheckable(true);
    //    organizarHor->setStatusTip(tr("Organizar Horizontalmente"));
    //    organizarHor->setText(tr("Org. Horiz."));
    //    //organizarHor->setIcon(QIcon(":/imagens/reduzir.png"));

    //    organizarVer = new QToolButton();
    //    organizarVer->setCheckable(true);
    //    organizarVer->setStatusTip(tr("Organizar Verticalmente"));
    //    organizarVer->setText(tr("Org. Vert."));
    //    //organizarVer->setIcon(QIcon(":/imagens/reduzir.png"));

    botoesManip = new QButtonGroup(this);
    botoesManip->setExclusive(false);
    botoesManip->addButton(ampliar,0);
    botoesManip->addButton(reduzir,1);
    //    botoesManip->addButton(organizarHor,3);
    //    botoesManip->addButton(organizarVer,2);
    connect(botoesManip, SIGNAL(buttonClicked(int)),
            this, SLOT(botoesManipClicked(int)));

    manipulacoes = addToolBar(trUtf8("&Manipulações"));
    manipulacoes->addWidget(ampliar);
    manipulacoes->addWidget(reduzir);
    //    manipulacoes->addWidget(organizarHor);
    //    manipulacoes->addWidget(organizarVer);

    //Cria botões(linha ou mouse) que permite escolher entre ligar ou manipular figuras
    mouse = new QToolButton();
    linhas = new QToolButton();
    deletar = new QToolButton();
    mouse->setCheckable(true);
    mouse->setChecked(true);
    mouse->setToolTip("Mouse");
    linhas->setCheckable(true);
    deletar->setCheckable(false);
    mouse->setStatusTip(trUtf8("Mouse"));
    linhas->setStatusTip(trUtf8("Linha"));
    linhas->setToolTip(trUtf8("Linha"));
    deletar->setStatusTip(trUtf8("Deletar"));
    deletar->setShortcut(Qt::Key_Delete);
    deletar->setToolTip(trUtf8("Deletar"));
    mouse->setIcon(QIcon(":/imagens/mouse.png"));
    linhas->setIcon(QIcon(":/imagens/linha.png"));
    deletar->setIcon(QIcon(":/imagens/deletar.png"));

    botoesML = new QButtonGroup(this);
    botoesML->setExclusive(false);
    botoesML->addButton(mouse,0);
    botoesML->addButton(linhas,1);
    botoesML->addButton(deletar,2);
    connect(botoesML, SIGNAL(buttonClicked(int)),
            this, SLOT(botoesMLClicked(int)));

    mouseLinha = addToolBar(trUtf8("&MouseLinha"));
    mouseLinha->addWidget(mouse);
    mouseLinha->addWidget(linhas);
    mouseLinha->addWidget(deletar);

    //Criando toolbar de exibicao(zoom, fit-to-all)
    exibicaoToolBar = addToolBar(trUtf8("&Zoom"));

    scroolZoom = new QSpinBox(exibicaoToolBar);
    scroolZoom->setFixedWidth(60);
    scroolZoom->setMinimum(10);
    scroolZoom->setMaximum(500);
    scroolZoom->setValue(100);
    scroolZoom->setToolTip(trUtf8("Zoom"));

    barraZoom = new QSlider(exibicaoToolBar);
    barraZoom->setFixedWidth(200);
    barraZoom->setMinimum(10);
    barraZoom->setMaximum(500);
    barraZoom->setValue(100);
    barraZoom->setToolTip(trUtf8("Zoom"));
    barraZoom->setOrientation(Qt::Horizontal);


    connect(barraZoom, SIGNAL(valueChanged(int)), scroolZoom, SLOT(setValue(int)));
    connect(scroolZoom, SIGNAL(valueChanged(int)), barraZoom, SLOT(setValue(int)));

    exibicaoToolBar->addWidget(scroolZoom);
    label = new QLabel("   ");
    exibicaoToolBar->addWidget(label);
    exibicaoToolBar->addWidget(barraZoom);
}

void MainWindow::setArquivoAtual(const QString nome)
{
    nomeArquivoAtual = nome;
    setWindowFilePath(nomeArquivoAtual);

    QSettings settings;
    QStringList files = settings.value("listaArquivosRecentes").toStringList();
    files.removeAll(nome);
    files.prepend(nome);
    while (files.size() > MAXARQRECENTES)
        files.removeLast();

    settings.setValue("listaArquivosRecentes", files);

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->atualizarAcaoArquivosRecentes();
    }
}

void MainWindow::atualizarAcaoArquivosRecentes()
{
    QSettings settings;
    QStringList files = settings.value("listaArquivosRecentes").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MAXARQRECENTES);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(diminuirNome(files[i]));
        arquivosRecentes[i]->setText(text);
        arquivosRecentes[i]->setData(files[i]);
        arquivosRecentes[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MAXARQRECENTES; ++j)
        arquivosRecentes[j]->setVisible(false);
}

QString MainWindow::diminuirNome(const QString nomeCompleto)
{
    return QFileInfo(nomeCompleto).fileName();
}

void MainWindow::setArquivoAtualTitulo(const QString nome)
{
    setWindowTitle(nome+" - BahiaDBM");
}

bool MainWindow::questionarSalvar(){
    if(pilhaDeAcoes->hasChanged()) {
        int res = QMessageBox::question(this, "Deseja salvar?", "Diagrama foi alterado, deseja salva-lo?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);

        if(res == QMessageBox::Yes)
            salvar();
        else if(res == QMessageBox::Cancel)
            return false; // Nao fecha Diagrama
    }
    return true;
}

void MainWindow::fecharDiagrama()
{
    if (sceneConceitual) {
        if(!questionarSalvar())
            return;
        deletarScene();
    }
}

void MainWindow::houveModificacao()
{
    if(!windowTitle().startsWith("(*) "))
        setWindowTitle( "(*) " + windowTitle());
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    if(!questionarSalvar()) {
        event->ignore();
        return;
    }
    QMainWindow::closeEvent(event);
}

void MainWindow::abaAlterada(int index)
{
    //Aba "Conceitual"
    if ( index == 0 )
    {
        if ( sceneConceitual != NULL )
        {
            formas->setHidden(false);
            formas->setEnabled(true);
            mouseLinha->actions().at(1)->setVisible(true);
            mouseLinha->actions().at(2)->setVisible(true);
        }
    }

    //Aba "Logico"
    else if ( index == 1 )
    {
        if ( sceneLogico != NULL )
        {
            formas->setHidden(true);
            formas->setEnabled(false);
            mouseLinha->actions().at(1)->setVisible(false);
            mouseLinha->actions().at(2)->setVisible(false);
        }
    }

    //Aba "Fisico"
    else if ( index == 2 )
    {
        if ( sceneFisico != NULL )
        {
            formas->setHidden(true);
            formas->setEnabled(false);
            mouseLinha->actions().at(1)->setVisible(false);
            mouseLinha->actions().at(2)->setVisible(false);
        }
    }
}

//Formata atributo
QString MainWindow::formataAtributo(QString nome)
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

//Verifica se atributo informado já existe na tabela lógica especificada.
bool MainWindow::procuraAtributo(Tabela *tab, QString nome)
{
    QList<Texto *> atr = tab->getListaAtributo();
    for ( int i=0; i<atr.size(); i++ )
        if ( formataAtributo(atr[i]->toPlainText()) == nome )
            return true;

    return false;
}

void MainWindow::insereAtributoTabela(Atributo::Tipo tipoAtributo, Poligono *entidade, Tabela *tab, bool pk, bool fk, bool nulo)
{
    if ( entidade != NULL )
    {
        for ( int j=0; j<entidade->getAtributosAssociados().size(); j++ )
        {
            Atributo *atr = entidade->getAtributosAssociados().at(j);
            if (( atr->getTipo() == tipoAtributo ) && ( atr->childItems().size() == 1 ) && ( atr->childItems().at(0)->type() == Texto::Type ))
            {
                castTexto = qgraphicsitem_cast<Texto *>(atr->childItems().at(0));
                if ( !procuraAtributo(tab, castTexto->toPlainText()) )
                    tab->addAtributo(castTexto->toPlainText(), pk, fk, nulo);
            }
        }
    }
}

//Busca nome do poligono, setado como filho.
QString MainWindow::buscaNome(Poligono *p)
{
    if ( p != NULL )
    {
        QString nome = NULL;
        for ( int j=0; j<p->childItems().size(); j++ )
            if ( p->childItems().at(j)->type() == Texto::Type )
            {
                castTexto = qgraphicsitem_cast<Texto *>(p->childItems().at(j));
                nome = castTexto->toPlainText();
                break;
            }

        return nome;
    }
    else
        return NULL;
}

//Caixa de diálogo com o usuário na transformação conceitual/lógico quando uma generalização/especialização é encontrada.
int MainWindow::dialogGenEsp(QString generalizacao)
{
    QMessageBox msgbox(QMessageBox::Question, trUtf8("Generalização/Especialização"),
                       trUtf8("Generalização/Especialização encontrada ("+generalizacao.toUtf8()+"). O que deseja fazer?\n\nOpção A: Criar uma única tabela\nOpção B: Criar várias tabelas\n"),
                       0, this);

    msgbox.addButton(trUtf8("Opção A"), QMessageBox::AcceptRole);
    msgbox.addButton(trUtf8("Opção B"), QMessageBox::RejectRole);

    int ret = msgbox.exec();

    if ( ret == QMessageBox::AcceptRole )
        return 1;
    else
        return 2;
}

//Verifica se tabela informada já existe no diagrama lógico.
Tabela *MainWindow::procuraTabela(QString nomeTab)
{
    for ( int i=0; i<tabelasLogico.size(); i++ )
        if ( tabelasLogico[i]->getTitulo() == nomeTab )
            return tabelasLogico[i];

    return NULL;
}

void MainWindow::gerarModeloLogico()
{
    //Limpa a área de trabalho do lógico antes de adicionar algo.
    QList<QGraphicsItem *> lixo = sceneLogico->items();
    for ( int i=0; i<lixo.size(); i++ )
        delete lixo[i];

    tabelasLogico.clear();

    const int espacoEntreTabelas = 50;

    QList<QGraphicsItem *> itens = sceneConceitual->items();
    int inicio=2120;
    qreal alturaMax;
    QPointF pos;
    pos.setX(2120); pos.setY(2397);

    for ( int i=0; i<itens.size(); i++ )
    {
        if ( itens[i]->type() == Poligono::Type )
        {
            cast_poligono = qgraphicsitem_cast<Poligono *>(itens[i]);
            if ((( cast_poligono->getTipo() == Poligono::relacionamento ) || ( cast_poligono->getTipo() == Poligono::ent_associativa )) && ( cast_poligono->getCardinalidadesAssociadas().size() >= 2 ))
            {
                QString cardMax1 = cast_poligono->getCardinalidadesAssociadas().at(0)->getCardinalidadeAtual().at(4);
                QString cardMin1 = cast_poligono->getCardinalidadesAssociadas().at(0)->getCardinalidadeAtual().at(0);
                QString cardMax2 = cast_poligono->getCardinalidadesAssociadas().at(1)->getCardinalidadeAtual().at(4);
                QString cardMin2 = cast_poligono->getCardinalidadesAssociadas().at(1)->getCardinalidadeAtual().at(0);

                //Cria modelo lógico a partir de um relacionamento N:N
                if (( cardMax1 == "N" ) && ( cardMax2 == "N" ))
                {
                    QList<Poligono *> entidade = cast_poligono->getPoligonosAssociadoOfType<Poligono::entidade>();
                    QList<Poligono *> entidade2 = cast_poligono->getPoligonosAssociadoOfType<Poligono::ent_associativa>();

                    if ( (( entidade.size() == 2 ) || (( entidade.size() == 1 ) && ( entidade2.size() == 1 ))) || ((entidade.size() == 1) && (entidade[0]->getAutoRelacionamento())) )
                    {
                        Poligono *ent1 = entidade[0];
                        Poligono *ent2 = NULL;
                        if ( !entidade[0]->getAutoRelacionamento() )
                            ent2 = (entidade.size() == 2) ? entidade[1] : entidade2[0];

                        //Busca nomes associados de relacionamento e entidades.
                        QString nomeRelacionamento = buscaNome(cast_poligono);
                        QString nomeEnt1 = buscaNome(ent1);
                        QString nomeEnt2 = buscaNome(ent2);

                        alturaMax = 0;

                        //Tabela 1
                        pos.setX(inicio);
                        Tabela *aux = procuraTabela(nomeEnt1);
                        if ( aux == NULL )
                        {
                            tab1 = new Tabela(nomeEnt1, NULL, sceneConceitual, sceneLogico);
                            tab1->setPos(pos);
                            tabelasLogico.push_back(tab1);
                        }
                        else
                            tab1 = aux;

                        //Adiciona na tabela os atributos identificadores de ent1
                        insereAtributoTabela(Atributo::atributo_identif, ent1, tab1, true, false, false);

                        //Adiciona na tabela os atributos de ent1
                        insereAtributoTabela(Atributo::atributo, ent1, tab1, false, false, true);

                        alturaMax = ( tab1->rect().height() > alturaMax ) ? tab1->rect().height() : alturaMax;

                        //Tabela 2
                        pos.setX(pos.x()+(tab1->rect().width()+espacoEntreTabelas));
                        aux = procuraTabela(nomeRelacionamento);
                        if ( aux == NULL )
                        {
                            tab2 = new Tabela(nomeRelacionamento, NULL, sceneConceitual, sceneLogico);
                            tab2->setPos(pos);
                            tabelasLogico.push_back(tab2);
                        }
                        else
                            tab2 = aux;

                        //Adiciona na tabela os atributos identificadores de ent1
                        insereAtributoTabela(Atributo::atributo_identif, ent1, tab2, true, true, false);

                        //Adiciona na tabela os atributos identificadores de ent2
                        insereAtributoTabela(Atributo::atributo_identif, ent2, tab2, true, true, false);

                        //Adiciona na tabela os atributos do relacionamento
                        if ( cast_poligono->getTipo() != Poligono::ent_associativa )
                            insereAtributoTabela(Atributo::atributo, cast_poligono, tab2, false, false, true);

                        alturaMax = ( tab2->rect().height() > alturaMax ) ? tab2->rect().height() : alturaMax;

                        //Tabela 3
                        if ( ent2 != NULL )
                        {
                            pos.setX(pos.x()+(tab2->rect().width()+espacoEntreTabelas));
                            Tabela *aux = procuraTabela(nomeEnt2);
                            if ( aux == NULL )
                            {
                                tab3 = new Tabela(nomeEnt2, NULL, sceneConceitual, sceneLogico);
                                tab3->setPos(pos);
                                tabelasLogico.push_back(tab3);
                            }
                            else
                                tab3 = aux;

                            //Adiciona na tabela os atributos identificadores de ent2
                            insereAtributoTabela(Atributo::atributo_identif, ent2, tab3, true, false, false);

                            //Procura e adiciona na tabela os atributos de ent2
                            insereAtributoTabela(Atributo::atributo, ent2, tab3, false, false, true);

                            alturaMax = ( tab3->rect().height() > alturaMax ) ? tab3->rect().height() : alturaMax;

                            AcaoCriarLigacao * acao2 = new AcaoCriarLigacao(sceneLogico, tab2, tab3);
                            if ( acao2->getLigacao() )
                            {
                                pilhaDeAcoes->addAcao(acao2);
                                acao2->fazerAcao();
                            }
                        }

                        AcaoCriarLigacao * acao1 = new AcaoCriarLigacao(sceneLogico, tab1, tab2);
                        if ( acao1->getLigacao() )
                        {
                            pilhaDeAcoes->addAcao(acao1);
                            acao1->fazerAcao();
                        }

                        //Atualiza posição da coordenada Y
                        pos.setY(pos.y()+(alturaMax+espacoEntreTabelas));
                    }
                }

                //Cria modelo lógico a partir de um relacionamento 1:N ou N:1
                else if ((( cardMax1 == "1" ) && ( cardMax2 == "N" )) || (( cardMax1 == "N" ) && ( cardMax2 == "1" )))
                {
                    QList<Poligono *> entidade = cast_poligono->getPoligonosAssociadoOfType<Poligono::entidade>();
                    QList<Poligono *> entidade2 = cast_poligono->getPoligonosAssociadoOfType<Poligono::ent_associativa>();

                    if ((( entidade.size() == 2 ) || (( entidade.size() == 1 ) && ( entidade2.size() == 1 ))) || ((entidade.size() == 1) && (entidade[0]->getAutoRelacionamento())) )
                    {
                        Poligono *ent1, *ent2;
                        if ( entidade.size() == 2 )
                        {
                            ent1 = (entidade[0]->getCardinalidadesAssociadas().at(0)->getCardinalidadeAtual().at(4) == 'N') ? entidade[0] : entidade[1];
                            ent2 = (entidade[0]->getCardinalidadesAssociadas().at(0)->getCardinalidadeAtual().at(4) == 'N') ? entidade[1] : entidade[0];
                        }
                        else if ( entidade[0]->getAutoRelacionamento() )
                        {
                            ent1 = entidade[0];
                            ent2 = NULL;
                        }
                        else
                        {
                            ent1 = (entidade[0]->getCardinalidadesAssociadas().at(0)->getCardinalidadeAtual().at(4) == 'N') ? entidade[0] : entidade2[0];
                            ent2 = (entidade[0]->getCardinalidadesAssociadas().at(0)->getCardinalidadeAtual().at(4) == 'N') ? entidade2[0] : entidade[0];
                        }

                        //Busca nomes associados de entidades.
                        QString nomeEnt1 = buscaNome(ent1);
                        QString nomeEnt2 = buscaNome(ent2);

                        alturaMax = 0;

                        //Tabela 1
                        pos.setX(inicio);
                        Tabela *aux = procuraTabela(nomeEnt1);
                        if ( aux == NULL )
                        {
                            tab1 = new Tabela(nomeEnt1, NULL, sceneConceitual, sceneLogico);
                            tab1->setPos(pos);
                            tabelasLogico.push_back(tab1);
                        }
                        else
                            tab1 = aux;

                        //Adiciona na tabela os atributos identificadores de ent2 caso seja uma entidade fraca
                        if ( ent1->getEntidadeFraca() )
                            insereAtributoTabela(Atributo::atributo_identif, ent2, tab1, true, true, false);

                        //Adiciona na tabela os atributos identificadores de ent1
                        insereAtributoTabela(Atributo::atributo_identif, ent1, tab1, true, false, false);

                        //Adiciona na tabela os atributos identificadores de ent2 caso seja uma entidade forte
                        if ( !ent1->getEntidadeFraca() )
                            insereAtributoTabela(Atributo::atributo_identif, ent2, tab1, false, true, false);

                        //Adiciona na tabela os atributos do relacionamento
                        if ( cast_poligono->getTipo() != Poligono::ent_associativa )
                            insereAtributoTabela(Atributo::atributo, cast_poligono, tab1, false, false, true);

                        //Adiciona na tabela os atributos de ent1
                        insereAtributoTabela(Atributo::atributo, ent1, tab1, false, false, true);

                        alturaMax = ( tab1->rect().height() > alturaMax ) ? tab1->rect().height() : alturaMax;

                        //Tabela 2
                        if ( ent2 != NULL )
                        {
                            pos.setX(pos.x()+(tab1->rect().width()+espacoEntreTabelas));
                            Tabela *aux = procuraTabela(nomeEnt2);
                            if ( aux == NULL )
                            {
                                tab2 = new Tabela(nomeEnt2, NULL, sceneConceitual, sceneLogico);
                                tab2->setPos(pos);
                                tabelasLogico.push_back(tab2);
                            }
                            else
                                tab2 = aux;

                            //Adiciona na tabela os atributos identificadores de ent2
                            insereAtributoTabela(Atributo::atributo_identif, ent2, tab2, true, false, false);

                            //Adiciona na tabela os atributos de ent2
                            insereAtributoTabela(Atributo::atributo, ent2, tab2, false, false, true);

                            alturaMax = ( tab2->rect().height() > alturaMax ) ? tab2->rect().height() : alturaMax;

                            AcaoCriarLigacao * acao1 = new AcaoCriarLigacao(sceneLogico, tab1, tab2);
                            if ( acao1->getLigacao() )
                            {
                                pilhaDeAcoes->addAcao(acao1);
                                acao1->fazerAcao();
                            }
                        }

                        //Atualiza posição da coordenada Y
                        pos.setY(pos.y()+(alturaMax+espacoEntreTabelas));
                    }
                }

                //Cria modelo lógico a partir de um relacionamento 1:1, onde as cardinalidade mínimas são iguais.
                //Cria modelo lógico a partir de um relacionamento 1:1, onde as cardinalidade mínimas são diferentes.
                else if (( cardMax1 == "1" ) && ( cardMax2 == "1" ))
                {
                    QList<Poligono *> entidade = cast_poligono->getPoligonosAssociadoOfType<Poligono::entidade>();
                    QList<Poligono *> entidade2 = cast_poligono->getPoligonosAssociadoOfType<Poligono::ent_associativa>();

                    if ((( entidade.size() == 2 ) || (( entidade.size() == 1 ) && ( entidade2.size() == 1 ))) || ((entidade.size() == 1) && (entidade[0]->getAutoRelacionamento())) )
                    {
                        Poligono *ent1;
                        Poligono *ent2;

                        if ( entidade.size() == 2 )
                        {
                            if (( entidade[0]->getCardinalidadesAssociadas().at(0)->getCardinalidadeAtual().at(0) == '0' ) || ( entidade[1]->getCardinalidadesAssociadas().at(0)->getCardinalidadeAtual().at(0) == '0' ))
                            {
                                ent1 = (entidade[0]->getCardinalidadesAssociadas().at(0)->getCardinalidadeAtual().at(0) == '0') ? entidade[0] : entidade[1];
                                ent2 = (entidade[0]->getCardinalidadesAssociadas().at(0)->getCardinalidadeAtual().at(0) == '0') ? entidade[1] : entidade[0];
                            }
                            else
                            {
                                ent1 = entidade[0];
                                ent2 = entidade[1];
                            }
                        }
                        else if ( entidade[0]->getAutoRelacionamento() )
                        {
                            ent1 = entidade[0];
                            ent2 = NULL;
                        }
                        else
                        {
                            if (( entidade[0]->getCardinalidadesAssociadas().at(0)->getCardinalidadeAtual().at(0) == '0' ) || ( entidade2[0]->getCardinalidadesAssociadas().at(0)->getCardinalidadeAtual().at(0) == '0' ))
                            {
                                ent1 = (entidade[0]->getCardinalidadesAssociadas().at(0)->getCardinalidadeAtual().at(0) == '0') ? entidade[0] : entidade2[0];
                                ent2 = (entidade[0]->getCardinalidadesAssociadas().at(0)->getCardinalidadeAtual().at(0) == '0') ? entidade2[0] : entidade[0];
                            }
                            else
                            {
                                ent1 = entidade[0];
                                ent2 = entidade2[0];
                            }
                        }

                        //Busca nomes associados de entidades.
                        QString nomeEnt1 = buscaNome(ent1);
                        QString nomeEnt2 = buscaNome(ent2);

                        alturaMax = 0;

                        //Tabela 1
                        //Antes de criar uma tabela, verifica no diagrama se esta já foi criada em outra situação.
                        pos.setX(inicio);
                        Tabela *aux = procuraTabela(nomeEnt1);
                        if ( aux == NULL )
                        {
                            tab1 = new Tabela(nomeEnt1, NULL, sceneConceitual, sceneLogico);
                            tab1->setPos(pos);
                            tabelasLogico.push_back(tab1);
                        }
                        else
                            tab1 = aux;

                        //Adiciona na tabela os atributos identificadores de ent1
                        insereAtributoTabela(Atributo::atributo_identif, ent1, tab1, true, false, false);

                        //Adiciona na tabela os atributos identificadores de ent2
                        insereAtributoTabela(Atributo::atributo_identif, ent2, tab1, false, true, false);

                        //Adiciona na tabela os atributos do relacionamento
                        if ( cast_poligono->getTipo() != Poligono::ent_associativa )
                            insereAtributoTabela(Atributo::atributo, cast_poligono, tab1, false, false, true);

                        //Adiciona na tabela os atributos de ent1
                        insereAtributoTabela(Atributo::atributo, ent1, tab1, false, false, true);

                        alturaMax = ( tab1->rect().height() > alturaMax ) ? tab1->rect().height() : alturaMax;

                        //Tabela 2
                        //ent2 será NULL quando for um auto-relacionamento
                        if ( ent2 != NULL )
                        {
                            //Antes de criar uma tabela, verifica no diagrama se esta já foi criada em outra situação.
                            pos.setX(pos.x()+(tab1->rect().width()+espacoEntreTabelas));
                            Tabela *aux = procuraTabela(nomeEnt2);
                            if ( aux == NULL )
                            {
                                tab2 = new Tabela(nomeEnt2, NULL, sceneConceitual, sceneLogico);
                                tab2->setPos(pos);
                                tabelasLogico.push_back(tab2);
                            }
                            else
                                tab2 = aux;

                            //Adiciona na tabela os atributos identificadores de ent2
                            insereAtributoTabela(Atributo::atributo_identif, ent2, tab2, true, false, false);

                            //Adiciona na tabela os atributos de ent2
                            insereAtributoTabela(Atributo::atributo, ent2, tab2, false, false, true);

                            alturaMax = ( tab2->rect().height() > alturaMax ) ? tab2->rect().height() : alturaMax;

                            AcaoCriarLigacao * acao1 = new AcaoCriarLigacao(sceneLogico, tab1, tab2);
                            if ( acao1->getLigacao() )
                            {
                                pilhaDeAcoes->addAcao(acao1);
                                acao1->fazerAcao();
                            }
                        }

                        //Atualiza posição da coordenada Y
                        pos.setY(pos.y()+(alturaMax+espacoEntreTabelas));
                    }
                }
            }

            else if (( cast_poligono->getTipo() == Poligono::entidade ) && ( !cast_poligono->getConectado() ))
            {
                //Busca nomes associados de entidades.
                QString nomeEnt = buscaNome(cast_poligono);

                alturaMax = 0;

                //Tabela 1
                pos.setX(inicio);
                Tabela *aux = procuraTabela(nomeEnt);
                if ( aux == NULL )
                {
                    tab1 = new Tabela(nomeEnt, NULL, sceneConceitual, sceneLogico);
                    tab1->setPos(pos);
                    tabelasLogico.push_back(tab1);
                }
                else
                    tab1 = aux;

                //Adiciona na tabela os atributos identificadores da entidade
                insereAtributoTabela(Atributo::atributo_identif, cast_poligono, tab1, true, false, false);

                //Adiciona na tabela os atributos da entidade
                insereAtributoTabela(Atributo::atributo, cast_poligono, tab1, false, false, true);

                alturaMax = ( tab1->rect().height() > alturaMax ) ? tab1->rect().height() : alturaMax;

                //Atualiza posição da coordenada Y
                pos.setY(pos.y()+(alturaMax+espacoEntreTabelas));
            }
            else if ( cast_poligono->getTipo() == Poligono::gen_esp )
            {
                QList<Poligono *> entidade = cast_poligono->getPoligonosAssociadoOfType<Poligono::entidade>();

                //Procura a entidade genérica e especializada por meio do atributo identificador.
                Poligono *generalizacao = NULL;
                QList< Poligono * > especializacao;

                for ( int i=0; i<entidade.size(); i++ )
                {
                    bool control=false;
                    QList<Atributo *> atr = entidade[i]->getAtributosAssociados();
                    for ( int j=0; j<atr.size(); j++ )
                    {
                        if ( atr[j]->getTipo() == Atributo::atributo_identif )
                        {
                            generalizacao = entidade[i];
                            control = true;
                            break;
                        }
                    }
                    if ( !control )
                        especializacao.push_back(entidade[i]);
                }

                QString nomeGeneralizacao = buscaNome(generalizacao);
                int tipo = dialogGenEsp(nomeGeneralizacao);

                //Tipo 1 = Cria uma única tabela
                if ( tipo == 1 )
                {
                    //Tabela 1
                    pos.setX(inicio);
                    Tabela *aux = procuraTabela(nomeGeneralizacao);
                    if ( aux == NULL )
                    {
                        tab1 = new Tabela(nomeGeneralizacao, NULL, sceneConceitual, sceneLogico);
                        tab1->setPos(pos);
                        tabelasLogico.push_back(tab1);
                    }
                    else
                        tab1 = aux;

                    alturaMax = 0;

                    //Adiciona na tabela os atributos identificadores da generalização
                    insereAtributoTabela(Atributo::atributo_identif, generalizacao, tab1, true, false, false);

                    //Adiciona na tabela os atributos da generalização
                    insereAtributoTabela(Atributo::atributo, generalizacao, tab1, false, false, true);

                    //Adiciona na tabela os atributos das especializações
                    for ( int i=0; i<especializacao.size(); i++ )
                        insereAtributoTabela(Atributo::atributo, especializacao[i], tab1, false, false, true);

                    alturaMax = ( tab1->rect().height() > alturaMax ) ? tab1->rect().height() : alturaMax;

                    //Atualiza posição da coordenada Y
                    pos.setY(pos.y()+(alturaMax+espacoEntreTabelas));
                }

                //Tipo 2 = Cria mais de uma tabela
                else
                {
                    //Tabela 1
                    pos.setX(inicio);
                    Tabela *aux = procuraTabela(nomeGeneralizacao);
                    if ( aux == NULL )
                    {
                        tab1 = new Tabela(nomeGeneralizacao, NULL, sceneConceitual, sceneLogico);
                        tab1->setPos(pos);
                        tabelasLogico.push_back(tab1);
                    }
                    else
                        tab1 = aux;

                    alturaMax = 0;

                    //Adiciona na tabela os atributos identificadores da generalização
                    insereAtributoTabela(Atributo::atributo_identif, generalizacao, tab1, true, false, false);

                    //Adiciona na tabela os atributos da generalização
                    insereAtributoTabela(Atributo::atributo, generalizacao, tab1, false, false, true);

                    alturaMax = ( tab1->rect().height() > alturaMax ) ? tab1->rect().height() : alturaMax;

                    //Atualiza posição da coordenada Y
                    pos.setY(pos.y()+(alturaMax+espacoEntreTabelas));

                    //Tabelas da especialização
                    pos.setX(inicio);
                    for ( int i=0; i<especializacao.size(); i++ )
                    {
                        Tabela *aux = procuraTabela(buscaNome(especializacao[i]));
                        if ( aux == NULL )
                        {
                            tabEsp = new Tabela(buscaNome(especializacao[i]), NULL, sceneConceitual, sceneLogico);
                            tabEsp->setPos(pos);
                            tabelasLogico.push_back(tabEsp);
                        }
                        else
                            tabEsp = aux;

                        //Adiciona na tabela especializada o(s) atributo(s) identificador(es) da generalização
                        insereAtributoTabela(Atributo::atributo_identif, generalizacao, tabEsp, true, true, false);

                        //Adiciona na tabela os atributos da especialização
                        insereAtributoTabela(Atributo::atributo, especializacao[i], tabEsp, false, false, true);

                        alturaMax = ( tabEsp->rect().height() > alturaMax ) ? tabEsp->rect().height() : alturaMax;

                        pos.setX(pos.x()+(tabEsp->rect().width()+espacoEntreTabelas));

                        AcaoCriarLigacao *acao1 = new AcaoCriarLigacao(sceneLogico, tab1, tabEsp);
                        if ( acao1->getLigacao() )
                        {
                            pilhaDeAcoes->addAcao(acao1);
                            acao1->fazerAcao();
                        }
                    }

                    //Atualiza posição da coordenada Y
                    pos.setY(pos.y()+(alturaMax+espacoEntreTabelas));
                }
            }
        }
    }
}
