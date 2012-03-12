#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define MAXARQRECENTES 5

#include "ligacao.h"
#include "diagrama.h"
#include "diagramaview.h"
#include "acoespilha.h"
#include "cardinalidade.h"
#include "poligono.h"
#include "texto.h"

#include <QMainWindow>
#include <QtGui>

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

    AcoesPilha * pilhaDeAcoes;

    Diagrama *scene;
    DiagramaView *view;
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

    QMenu *arquivoMenu;
    QMenu *editarMenu;
    QMenu *ajudaMenu;

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

    QToolBar * exibicaoToolBar;
    QSpinBox * scroolZoom;

    QString nomeArquivoAtual;
};

#endif // MAINWINDOW_H
