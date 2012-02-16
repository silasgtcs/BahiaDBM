#ifndef ACOESPILHA_H
#define ACOESPILHA_H

#include "acao.h"
#include "acoes/acao_mover.h"
#include "QList"

class AcoesPilha
{
private:
    QList<Acao *> pilhaDeAcoes;
    int posAtual;
    AcoesPilha();
    AcoesPilha(AcoesPilha const&);     // Nao implementar
    void operator=(AcoesPilha const&); // Nao implementar
public:
    static AcoesPilha * instanciaUnica()
    {
        static AcoesPilha instance;
        return &instance;
    }


    Acao * getTopCommand();
    void addAcao(Acao * command);
    bool desfazer();
    bool refazer();
};

#endif // ACOESPILHA_H
