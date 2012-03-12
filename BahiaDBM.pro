#-------------------------------------------------
#
# Project created by QtCreator 2011-07-05T14:28:03
#
#-------------------------------------------------

QT       += core gui

TARGET = BahiaDBM
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    diagrama.cpp \
    poligono.cpp \
    ligacao.cpp \
    atributo.cpp \
    texto.cpp \
    cardinalidade.cpp \
    diagramaview.cpp \
    dialog.cpp \
    acoespilha.cpp \
    acoes/acao_mover.cpp \
    acoes/acao_criar_poligono.cpp \
    acoes/acao_criar_atributo.cpp \
    acoes/acao_criar_ligacao.cpp \
    acoes/acao_deletar.cpp

HEADERS  += mainwindow.h \
    diagrama.h \
    atributo.h \
    ligacao.h \
    poligono.h \
    texto.h \
    cardinalidade.h \
    diagramaview.h \
    dialog.h \
    acoespilha.h \
    acao.h \
    acoes/acao_mover.h \
    acoes/acao_criar_poligono.h \
    acoes/acao_criar_atributo.h \
    acoes/acao_criar_ligacao.h \
    acoes/acao_deletar.h \
    objetoremovivel.h

FORMS    +=

OTHER_FILES += \
    imagens/mouse.png \
    imagens/linha.png \
    imagens/ampliar.png \
    imagens/reduzir.png \
    imagens/atributo.png \
    imagens/atributo_identif.png \
    imagens/ent_associativa.png \
    imagens/gen_especializacao.png \
    imagens/relacionamento.png \
    imagens/deletar.png

RESOURCES += \
    resources.qrc





















