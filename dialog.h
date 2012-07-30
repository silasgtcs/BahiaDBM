/*Utilizada para criar a janela de opções para que seja modificado os tipos de generalização/especialização
ou os tipos de cardinalidade.*/

#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class Dialog : public QWidget
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    QString mostrarCardinalidade();
    QString mostrarTipoGenEsp();
    QPair<QString, bool> alterarNomeLogico(QString nomeAtual, QString tipo, bool alteraRestricao, bool nulo, bool tabFisica);

private slots:
    void botaoOk() { dlg->close(); } ;
    void botaoCancelar();

private:
    QDialog *dlg;
    QLabel *label;
    QLineEdit *lineEdit;
    QRadioButton *notNullRadio;
    QRadioButton *nullRadio;
    QComboBox *selecionarTipo;
    bool cancelar;
};

#endif // DIALOG_H
