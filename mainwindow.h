#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define MAXARQRECENTES 5

#include "qtextcodec.h"
#include "ligacao.h"
#include "diagrama.h"
#include "diagramaview.h"
#include "acoespilha.h"
#include "cardinalidade.h"
#include "poligono.h"
#include "texto.h"
#include "tabela.h"

#include <QMainWindow>
#include <QtGui>
#include <QMessageBox>

class Diagrama;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:

signals:

private slots:
    void sobre();
    void botoesERClicked(int id);
    void botoesManipClicked(int id);
    void botoesMLClicked(int id);
    void itemInserido();
    void desfazer();
    void refazer();
    void novaJanela();
    void abrir();
    void salvar();
    void salvarComo();
    void abrirArquivoRecente();
    void fecharDiagrama();
    void houveModificacao();
    void abaAlterada(int index);
    void voltaMouse();
    void gerarModeloLogico();
    void gerarModeloFisico();
protected:
    virtual void closeEvent(QCloseEvent *);

private:
    void createActions();
    void createMenu();
    void createToolBar();
    void organizarEntidades(int direcao);
    void salvarArquivo(const QString nomeArquivo);
    void abrirArquivo(const QString nomeArquivo);
    void deletarSelecionados();
    void setNomeArquivoAtual(QString nome) { this->nomeArquivoAtual = nome; }
    void setArquivoAtual(const QString nomeArquivo);
    void atualizarAcaoArquivosRecentes();
    void setArquivoAtualTitulo(const QString nome);
    QString diminuirNome(const QString nomeCompleto);
    void criarScene();
    void deletarScene();
    bool questionarSalvar();
    void resetWindowState();
    QString formataAtributo(QString nome);
    bool procuraAtributo(Tabela *tab, QString nome);
    void insereAtributoTabela( Atributo::Tipo tipoAtributo, Poligono *entidade, Tabela *tab, bool pk, bool fk, bool nulo );
    QString buscaNome( Poligono *p );
    int dialogGenEsp(QString generalizacao);
    Tabela *procuraTabela(QString nomeTab);

    AcoesPilha * pilhaDeAcoes;

    Texto *castTexto;

    Diagrama *sceneConceitual;
    Diagrama *sceneLogico;
    Diagrama *sceneFisico;

    DiagramaView *viewConceitual;
    DiagramaView *viewLogico;
    DiagramaView *viewFisico;

    Poligono *cast_poligono;

    QAction *sairAction;
    QAction *ajudaAction;
    QAction *desfazerAction;
    QAction *refazerAction;
    QAction *abrirAction;
    QAction *salvarComoAction;
    QAction *salvarAction;
    QAction *novaAction;
    QAction *arquivosRecentes[MAXARQRECENTES];
    QAction *fecharDiagramaAtual;
    QAction *modeloLogico;
    QAction *modeloFisico;

    QMenu *arquivoMenu;
    QMenu *editarMenu;
    QMenu *ajudaMenu;
    QMenu *gerarMenu;

    QToolBar *formas;
    QToolBar *manipulacoes;
    QToolBar *mouseLinha;

    QToolButton *entidade;
    QToolButton *relacionamento;
    QToolButton *gen_esp;
    QToolButton *entid_associativa;
    QToolButton *atributo;
    QToolButton *atributo_identif;
    QToolButton *ampliar;
    QToolButton *reduzir;
    QToolButton *mouse;
    QToolButton *linhas;
    QToolButton *deletar;
    QToolButton *entidadeFraca;
    QToolButton *organizarHor;
    QToolButton *organizarVer;

    QButtonGroup *botoesER;
    QButtonGroup *botoesManip;
    QButtonGroup *botoesML;    

    QToolBar *exibicaoToolBar;

    QSpinBox *scroolZoom;

    QSlider *barraZoom;

    QLabel *label;

    QString nomeArquivoAtual;

    QTabWidget *tabWidget;

    QGraphicsLineItem *nLinha;

    Tabela *tab1;
    Tabela *tab2;
    Tabela *tab3;
    Tabela *tabEsp;

    QList <Tabela *> tabelasLogico;
};

#endif // MAINWINDOW_H
