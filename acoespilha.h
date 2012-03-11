/*Tanto o arquivo acoes.h quando acoespilha.h são para controlar a opção desfazer/refazer da ferramenta.*/

#ifndef ACOESPILHA_H
#define ACOESPILHA_H

#include "acao.h"
#include "acoes/acao_mover.h"
#include "acoes/acao_criar_poligono.h"
#include "acoes/acao_criar_atributo.h"
#include "acoes/acao_criar_ligacao.h"
#include "acoes/acao_deletar.h"
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
    static AcoesPilha * instanciaUnica() // Singleton
    {
        static AcoesPilha instance;
        return &instance;
    }

    Acao * getTopCommand();
    void addAcao(Acao * command);
    void addAcao(Acao * command, bool run);
    bool desfazer();
    bool refazer();
    void limpar();
};

#endif // ACOESPILHA_H
