#ifndef TELA_H
#define TELA_H

#include "Jogo.h"

void imprimirMenuPrincipal();
void imprimirMenuNovoJogo(int idioma);
void imprimirMenuContinuarJogo(Jogo *jogo, int qntJogosSalvos);
void imprimirMenuIdioma(Jogo *jogo);
void imprimirArquivo(Jogo *jogo, int numArquivo);
void imprimirMenu(char *nome);
void imprimirOpcao(char *nome, int valor, int idioma);
void imprimirComando();
void imprimirFinal();
void imprimirLinha();
void imprimirEspaco();

#endif