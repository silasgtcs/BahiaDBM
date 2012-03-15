#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    pilhaDeAcoes = new AcoesPilha();
    connect(pilhaDeAcoes, SIGNAL(changed()), this, SLOT(houveModificacao()));

    criarScene();
    createActions();
    createMenu();
    createToolBar();
}

MainWindow::~MainWindow()
{
}

void MainWindow::criarScene()
{
    scene = new Diagrama(this, pilhaDeAcoes);
    pilhaDeAcoes->setUnchaged();
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(scene, SIGNAL(itemInserido()),
            this, SLOT(itemInserido()));
    scene->setTipoER(Diagrama::TipoER(7));
    view = new DiagramaView(scene);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle("BahiaDBM");
}

void MainWindow::deletarScene()
{
    pilhaDeAcoes->limpar();
    pilhaDeAcoes->setUnchaged();
    delete scene;
    delete view;
    scene = NULL;
    view = NULL;
    setArquivoAtualTitulo("");
}

void MainWindow::itemInserido()
{
    botoesER->button(int(scene->getTipoER()))->setChecked(false);
    scene->setTipoER(Diagrama::TipoER(7)); //Tipo(7) = nenhum
    botoesML->button(0)->setChecked(true);
    botoesML->button(1)->setChecked(false);
}

void MainWindow::botoesERClicked(int id)
{
    QList<QAbstractButton *> botoes = botoesER->buttons();
    foreach ( QAbstractButton *botao, botoes )
        if ( botoesER->button(id) != botao )
            botao->setChecked(false);

    scene->setTipoER(Diagrama::TipoER(id));

    /*Inicialmente verifica se botão clicado é o "EF". Caso afirmativo, verifica se existe alguma linha selecionada, e então aumenta
      a sua espessura.*/
    if ( id == 6 )
    {
        scene->setTipoER(Diagrama::TipoER(7));

        QList<QGraphicsItem *> entidade = scene->selectedItems();

        if ((entidade.length() > 0) && ( entidade[0]->type() == Ligacao::Type ))
        {
            Ligacao * castLinha = qgraphicsitem_cast<Ligacao *>(entidade[0]);
            castLinha->setEntidadeFracaAtiva(true);
        }
    }
}

void MainWindow::botoesManipClicked(int id)
{
    botoesManip->button(id)->setChecked(false);

    if(id == 0 || id == 1)
    {
        QList<QGraphicsItem *> itens = scene->selectedItems();
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
    QList<QGraphicsItem*> itemsSelecionados = scene->selectedItems();
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
    MainWindow *novaJanela = new MainWindow;
    novaJanela->show();
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
        QMessageBox::warning(this, tr("Erro"),
                             tr("Erro ao salvar arquivo"));
        return;
    }

    QDataStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    //Retorna todos os objetos do diagrama para serializá-los.
    QList<QGraphicsItem *> obj = scene->items();
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

    pilhaDeAcoes->setUnchaged();
    setArquivoAtual(nomeArquivo);
    setArquivoAtualTitulo(diminuirNome(nomeArquivo));
    QApplication::restoreOverrideCursor();

    statusBar()->showMessage(tr("Arquivo salvo com sucesso!"), 2000);
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
        QMessageBox::warning(this, tr("Erro"),
                             tr("Erro ao abrir arquivo"));
        return;
    }

    if ( scene )
        deletarScene();

    QDataStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    criarScene();

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
            abrirPoligono = new Poligono(Poligono::entidade,true,0,scene);
            abrirTexto = new Texto();
            abrirTexto->setTextInteractionFlags(Qt::TextEditorInteraction);
            scene->addItem(abrirTexto);
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
            abrirPoligono = new Poligono(Poligono::relacionamento,true,0,scene);
            abrirTexto = new Texto();
            abrirTexto->setTextInteractionFlags(Qt::TextEditorInteraction);
            scene->addItem(abrirTexto);
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
                scene->addItem(abrirAutoRelacionamento1);
                scene->addItem(abrirAutoRelacionamento2);
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
            abrirPoligono = new Poligono(Poligono::gen_esp,true,0,scene);
            abrirTexto = new Texto();
            abrirTexto->setFlag(QGraphicsItem::ItemIsMovable, false);
            abrirTexto->setFlag(QGraphicsItem::ItemIsSelectable, false);
            abrirTexto->setCursor(Qt::PointingHandCursor);
            scene->addItem(abrirTexto);
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
            AcaoCriarPoligono *acao = new AcaoCriarPoligono(scene, Poligono::ent_associativa, txt, posicao);
            acao->fazerAcao();
            acao->getPoligono()->setScale(s);
        }

        else if ( head == "|_atributo_|" )
        {
            abrirAtributo = new Atributo(Atributo::atributo,0,scene);
            abrirTexto = new Texto();
            scene->addItem(abrirTexto);
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
            abrirAtributo = new Atributo(Atributo::atributo_identif,0,scene);
            abrirTexto = new Texto();
            scene->addItem(abrirTexto);
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

            QList<QGraphicsItem *> obj = scene->items();
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
            scene->addItem(ligacao);

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

            abrirCardinalidade->addPoligonoAssociado(castPoligono);
            abrirCardinalidade->addPoligonoAssociado(castPoligono2);

            scene->addItem(abrirCardinalidade);

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
                QList<QGraphicsItem *> obj = scene->items();
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
                QList<QGraphicsItem *> obj = scene->items();
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
            scene->addItem(ligacao);

            bool entidadeFraca;
            in >> entidadeFraca;
            ligacao->setEntidadeFracaAtiva(entidadeFraca);

            item[0]->setPos(item[0]->x()+0.1, item[0]->y()+0.1);
            item[0]->setPos(item[0]->x()-0.1, item[0]->y()-0.1);
        }
    }

    QApplication::restoreOverrideCursor();

    QString nome = diminuirNome(nomeArquivo);
    setArquivoAtualTitulo(nome);
    setNomeArquivoAtual(nomeArquivo);

    setArquivoAtual(nomeArquivo);
    pilhaDeAcoes->setUnchaged();
    statusBar()->showMessage(tr("Arquivo aberto com sucesso!"), 2000);
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
    QString nomeArquivo = QFileDialog::getOpenFileName(this, tr("Abrir..."), "",
                                                       tr("BahiaDBM (*.bdm)"));
    if (!nomeArquivo.isEmpty())
        abrirArquivo(nomeArquivo);
}

void MainWindow::salvarComo()
{
    QString nomeArquivo = QFileDialog::getSaveFileName(this,tr("Save File"),
                                                       "meudiagrama.bdm",
                                                       tr("BahiaDBM (*.bdm)"));
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
        scene->setTipoER(Diagrama::TipoER(7));
    else if ( id == 1 ) //Linha
        scene->setTipoER(Diagrama::TipoER(6));
    else //Remover
        deletarSelecionados();
}

void MainWindow::deletarSelecionados() {
    if(scene->selectedItems().size() <= 0)
        return;

    AcaoDeletar * deletar = new AcaoDeletar(this->scene, this->scene->selectedItems());
    pilhaDeAcoes->addAcao(deletar, true);
}

void MainWindow::sobre()
{
    QMessageBox::about(this, tr("Sobre BahiaDBM"), tr("Bahia Database Modeler"));
}

void MainWindow::createActions()
{
    novaAction = new QAction(tr("&Novo"), this);
    novaAction->setShortcuts(QKeySequence::New);
    novaAction->setStatusTip(tr("Abrir uma nova janela"));
    connect(novaAction, SIGNAL(triggered()), this, SLOT(novaJanela()));

    abrirAction = new QAction(tr("&Abrir..."), this);
    abrirAction->setShortcut(QKeySequence::Open);
    abrirAction->setStatusTip("Abrir um arquivo .bdm");
    connect(abrirAction, SIGNAL(triggered()), this, SLOT(abrir()));

    salvarAction = new QAction(tr("&Salvar"), this);
    salvarAction->setShortcut(QKeySequence::Save);
    salvarAction->setStatusTip("Salvar alterações do projeto atual");
    connect(salvarAction, SIGNAL(triggered()), this, SLOT(salvar()));

    salvarComoAction = new QAction(tr("&Salvar como..."), this);
    salvarComoAction->setShortcut(QKeySequence::SaveAs);
    salvarComoAction->setStatusTip("Salvar projeto atual");
    connect(salvarComoAction, SIGNAL(triggered()), this, SLOT(salvarComo()));

    fecharDiagramaAtual = new QAction(tr("&Fechar Diagrama"), this);
    fecharDiagramaAtual->setShortcut(tr("Ctrl+D"));
    fecharDiagramaAtual->setStatusTip("Fechar diagrama atual");
    connect(fecharDiagramaAtual, SIGNAL(triggered()), this, SLOT(fecharDiagrama()));

    for ( int i=0; i<MAXARQRECENTES; i++ )
    {
        arquivosRecentes[i] = new QAction(this);
        arquivosRecentes[i]->setVisible(false);
        connect(arquivosRecentes[i], SIGNAL(triggered()), this, SLOT(abrirArquivoRecente()));
    }

    sairAction = new QAction(tr("&Sair"), this);
    sairAction->setShortcut(QKeySequence::Quit);
    sairAction->setStatusTip(tr("Sair do BahiaDBM"));
    connect(sairAction, SIGNAL(triggered()), this, SLOT(close()));

    desfazerAction = new QAction(tr("&Desfazer"), this);
    desfazerAction->setShortcut(tr("Ctrl+Z"));
    desfazerAction->setStatusTip(tr("Desfazer"));
    connect(desfazerAction, SIGNAL(triggered()), this, SLOT(desfazer()));

    refazerAction = new QAction(tr("&Refazer"), this);
    refazerAction->setShortcut(tr("Ctrl+R"));
    refazerAction->setStatusTip(tr("Refazer"));
    connect(refazerAction, SIGNAL(triggered()), this, SLOT(refazer()));

    ajudaAction = new QAction(tr("&Sobre"), this);
    ajudaAction->setShortcut(tr("Ctrl+H"));
    ajudaAction->setStatusTip(tr("Sobre BahiaDBM"));
    connect(ajudaAction, SIGNAL(triggered()), this, SLOT(sobre()));
}

void MainWindow::createMenu()
{
    arquivoMenu = menuBar()->addMenu(tr("&Arquivo"));
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

    editarMenu = menuBar()->addMenu(tr("&Editar"));
    editarMenu->addAction(desfazerAction);
    editarMenu->addAction(refazerAction);

    ajudaMenu = menuBar()->addMenu(tr("&Ajuda"));
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
    entidade->setIcon(QIcon(":/imagens/entidade.png"));
    entidade->setStatusTip(tr("Entidade"));
    relacionamento->setIcon(QIcon(":/imagens/relacionamento.png"));
    relacionamento->setStatusTip(tr("Relacionamento"));
    gen_esp->setIcon(QIcon(":/imagens/gen_especializacao.png"));
    gen_esp->setStatusTip(tr("Generalizacao/Especializacao"));
    entid_associativa->setIcon(QIcon(":/imagens/ent_associativa.png"));
    entid_associativa->setStatusTip(tr("Entidade Associativa"));
    atributo->setIcon(QIcon(":/imagens/atributo.png"));
    atributo->setStatusTip(tr("Atributo"));
    atributo_identif->setIcon(QIcon(":/imagens/atributo_identif.png"));
    atributo_identif->setStatusTip(tr("Atributo Identificador"));
    entidadeFraca->setText("EF");
    entidadeFraca->setStatusTip(tr("Entidade Fraca"));


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

    formas = addToolBar(tr("&Formas"));
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
    ampliar->setStatusTip(tr("Ampliar"));
    ampliar->setIcon(QIcon(":/imagens/ampliar.png"));

    reduzir = new QToolButton();
    reduzir->setCheckable(true);
    reduzir->setStatusTip(tr("Reduzir"));
    reduzir->setIcon(QIcon(":/imagens/reduzir.png"));

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

    manipulacoes = addToolBar(tr("&Manipulacoes"));
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
    linhas->setCheckable(true);
    deletar->setCheckable(false);
    mouse->setStatusTip(tr("Mouse"));
    linhas->setStatusTip(tr("Linha"));
    deletar->setStatusTip(tr("Deletar"));
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

    mouseLinha = addToolBar(tr("&MouseLinha"));
    mouseLinha->addWidget(mouse);
    mouseLinha->addWidget(linhas);
    mouseLinha->addWidget(deletar);

    //Criando toolbar de exibicao(zoom, fit-to-all)
    exibicaoToolBar = addToolBar(tr("&Exibicao"));

    scroolZoom = new QSpinBox(exibicaoToolBar);
    scroolZoom->setFixedWidth(50);
    scroolZoom->setMaximum(500);
    scroolZoom->setMinimum(10);
    scroolZoom->setValue(100);
    connect(scroolZoom, SIGNAL(valueChanged(int)), view, SLOT(setarZoom(int)));

    //Associa scrool ao graphicView(assim wheel do mouse pode ser redirecionado para o scroll)
    view->setScrool(scroolZoom);

    exibicaoToolBar->addWidget(scroolZoom);
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
    if (scene) {
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
