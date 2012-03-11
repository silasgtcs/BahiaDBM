/*Tanto o arquivo acoes.h quando acoespilha.h são utilizados para controlar a opção desfazer/refazer da ferramenta.*/

#ifndef ACOES_H
#define ACOES_H

#include <QObject>

class Acao : public QObject // Interface para as acoes
{
    Q_OBJECT

public:
    virtual ~Acao() {}

    virtual void fazerAcao() = 0;
    virtual void desfazerAcao() = 0;
    virtual bool podeDesfazer() = 0;
    virtual void dispose() = 0;
};

#endif // ACOES_H
