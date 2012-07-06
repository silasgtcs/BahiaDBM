#include "dialog.h"

Dialog::Dialog(QWidget *parent) : QWidget(parent)
{
}

QString Dialog::mostrarCardinalidade()
{
    QStringList tipos;
    tipos << tr("1 : 1") << tr("1 : N") << tr("0 : 1") << ("0 : N");

    bool ok;
    QString tipo = QInputDialog::getItem(this, tr("Cardinalidade"),
                                         tr("Tipos:"), tipos, 0, false, &ok);

    if (ok && !tipo.isEmpty())
        return tipo;
    else
        return NULL;
}

QString Dialog::mostrarTipoGenEsp()
{
    QStringList tipos;
    tipos << tr("xt") << tr("ct") << tr("xp") << tr("cp");

    bool ok;
    QString tipo = QInputDialog::getItem(this, trUtf8("Tipo Generalização/Especialização"),
                                         trUtf8("Tipos:"), tipos, 0, false, &ok);

    if (ok && !tipo.isEmpty())
        return tipo;
    else
        return NULL;
}

void Dialog::botaoCancelar()
{
    cancelar=true;
    dlg->close();
}

QPair<QString, bool> Dialog::alterarNomeLogico(QString nomeAtual, bool alteraRestricao, bool nulo)
{
    dlg = new QDialog(this);
    cancelar=false;

    label = new QLabel(tr("Novo Nome:"));
    lineEdit = new QLineEdit;
    lineEdit->setText(nomeAtual);
    label->setBuddy(lineEdit);

    notNullRadio = new QRadioButton(tr("NOT NULL"));
    nullRadio = new QRadioButton(tr("NULL"));

    nulo ? nullRadio->setChecked(true) : notNullRadio->setChecked(true);

    QPushButton *okButton = new QPushButton(tr("Ok"));
    QPushButton *cancelButton = new QPushButton(tr("Cancelar"));

    connect(okButton, SIGNAL(clicked()), this, SLOT(botaoOk()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(botaoCancelar()));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Vertical);
    buttonBox->addButton(okButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(lineEdit);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    if ( alteraRestricao )
    {
        leftLayout->addWidget(notNullRadio);
        leftLayout->addWidget(nullRadio);
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(leftLayout, 0, 0);
    mainLayout->addWidget(buttonBox, 0, 1);
    mainLayout->setRowStretch(2, 1);

    dlg->setLayout(mainLayout);
    dlg->setWindowTitle(tr("Alterar"));
    dlg->exec();

    if ( cancelar )
    {
        QString temp=NULL;
        return qMakePair(temp,false);
    }
    else
        return qMakePair(lineEdit->displayText(), nullRadio->isChecked());
}
