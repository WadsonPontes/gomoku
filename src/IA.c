#include "headers/IA.h"
#include "headers/Util.h"
#include "headers/Regra2.h"
#include "headers/Regra1.h"
#include <stdio.h>

void pedirJogadaIA(Jogo *jogo, int *lin, int *col) {
	Jogada jogadas[500];
	int n = 0;

	for (int i = 0; i < jogo->goban.dimensao; i++) {
		for (int j = 0; j < jogo->goban.dimensao; j++) {
			if (jogo->goban.matriz[i][j] == -1 && !verificarFormacao3x3(jogo->goban, i, j, jogo->proximoJogador)) {
				jogadas[n].lin = i;
				jogadas[n].col = j;
				jogadas[n].prioridade = calcularPrioridade(jogo, i, j);
				n++;
			}
		}
	}

	selecionarMelhorJogada(jogadas, n, lin, col);
}

int calcularPrioridade(Jogo *jogo, int i, int j) {
	Jogador jogador = jogo->jogador1.peca == jogo->proximoJogador ? jogo->jogador1 : jogo->jogador2;

	return   calcularCaptura(jogo, jogador, i, j)
	       + calcularCapturado(jogo, jogador, i, j)
	       + calcularSequencia(jogo, jogador, i, j)
	       + calcularBloqueio(jogo, jogador, i, j);
}

int calcularCaptura(Jogo *jogo, Jogador jogador, int i, int j) {
	return   calcularCapDirecao(jogo, jogador, i, j,  0,  1)
	       + calcularCapDirecao(jogo, jogador, i, j,  0, -1)
	       + calcularCapDirecao(jogo, jogador, i, j,  1,  0)
	       + calcularCapDirecao(jogo, jogador, i, j, -1,  0)
	       + calcularCapDirecao(jogo, jogador, i, j,  1,  1)
	       + calcularCapDirecao(jogo, jogador, i, j, -1, -1)
	       + calcularCapDirecao(jogo, jogador, i, j, -1,  1)
	       + calcularCapDirecao(jogo, jogador, i, j,  1, -1);
}

int calcularCapDirecao(Jogo *jogo, Jogador jogador, int i, int j, int di, int dj) {
	int existe =    i + 3*di >= 0
                 && i + 3*di < jogo->goban.dimensao
                 && j + 3*dj >= 0
                 && j + 3*dj < jogo->goban.dimensao
                 && jogo->goban.matriz[i +   di][j +   dj] == 1-jogador.peca
                 && jogo->goban.matriz[i + 2*di][j + 2*dj] == 1-jogador.peca
                 && jogo->goban.matriz[i + 3*di][j + 3*dj] == jogador.peca;

    if (existe && jogador.capturas == 4) {
    	return 1500;
    }
    if (existe) {
    	return   55
    	       + calcularBloqueio(jogo, jogador, i + di, j + dj)
    	       + calcularBloqueio(jogo, jogador, i + 2*di, j + 2*dj);
    }
    return 0;
}

int calcularCapturado(Jogo *jogo, Jogador jogador, int i, int j) {
	Jogador inimigo = jogo->jogador1.peca == jogo->proximoJogador ? jogo->jogador2 : jogo->jogador1;

	return - 1
	       + calcularCaptura(jogo, inimigo, i, j)
	       + calcularCapturadoDir(jogo, jogador, inimigo, i, j,  0,  1)
	       + calcularCapturadoDir(jogo, jogador, inimigo, i, j,  0, -1)
	       + calcularCapturadoDir(jogo, jogador, inimigo, i, j,  1,  0)
	       + calcularCapturadoDir(jogo, jogador, inimigo, i, j, -1,  0)
	       + calcularCapturadoDir(jogo, jogador, inimigo, i, j,  1,  1)
	       + calcularCapturadoDir(jogo, jogador, inimigo, i, j, -1, -1)
	       + calcularCapturadoDir(jogo, jogador, inimigo, i, j, -1,  1)
	       + calcularCapturadoDir(jogo, jogador, inimigo, i, j,  1, -1);
}

int calcularCapturadoDir(Jogo *jogo, Jogador jogador, Jogador inimigo, int i, int j, int di, int dj) {
	int existe =     i + 2*di >= 0
                  && i + 2*di < jogo->goban.dimensao
                  && j + 2*dj >= 0
                  && j + 2*dj < jogo->goban.dimensao
                  && i -   di >= 0
                  && i -   di < jogo->goban.dimensao
                  && j -   dj >= 0
                  && j -   dj < jogo->goban.dimensao
                  && !verificarFormacao3x3(jogo->goban, i - di, j - dj, inimigo.peca)
                  && jogo->goban.matriz[i -   di][j -   dj] == -1
                  && jogo->goban.matriz[i +   di][j +   dj] == jogador.peca
                  && jogo->goban.matriz[i + 2*di][j + 2*dj] == 1-jogador.peca;

	if (existe && inimigo.capturas == 4) {
    	return -1500;
    }
    if (existe) {
    	return - 55
    	       - calcularSequencia(jogo, jogador, i, j)
    	       - calcularSequencia(jogo, jogador, i + di, j + dj);
    }

    existe =     i - 2*di >= 0
              && i - 2*di < jogo->goban.dimensao
              && j - 2*dj >= 0
              && j - 2*dj < jogo->goban.dimensao
              && i +   di >= 0
              && i +   di < jogo->goban.dimensao
              && j +   dj >= 0
              && j +   dj < jogo->goban.dimensao
              && jogo->goban.matriz[i - 2*di][j - 2*dj] == 1-jogador.peca
              && jogo->goban.matriz[i -   di][j -   dj] == jogador.peca
              && jogo->goban.matriz[i +   di][j +   dj] == -1
              && !verificarFormacao3x3(jogo->goban, i + di, j + dj, inimigo.peca);

    if (existe && inimigo.capturas == 4) {
    	return -1500;
    }
    if (existe) {
    	return - 55
    	       - calcularSequencia(jogo, jogador, i, j)
    	       - calcularSequencia(jogo, jogador, i - di, j - dj);
    }

    return 0;
}

int calcularSequencia(Jogo *jogo, Jogador jogador, int i, int j) {
	return   calcularSeqDirecao(jogo->goban, i, j, 0, 1, jogador.peca)
	       + calcularSeqDirecao(jogo->goban, i, j, 1, 0, jogador.peca)
	       + calcularSeqDirecao(jogo->goban, i, j, 1, 1, jogador.peca)
	       + calcularSeqDirecao(jogo->goban, i, j,-1, 1, jogador.peca);
}

int calcularBloqueio(Jogo *jogo, Jogador jogador, int i, int j) {
	return   1
	       + calcularSeqDirecao(jogo->goban, i, j, 0, 1, 1-jogador.peca)
	       + calcularSeqDirecao(jogo->goban, i, j, 1, 0, 1-jogador.peca)
	       + calcularSeqDirecao(jogo->goban, i, j, 1, 1, 1-jogador.peca)
	       + calcularSeqDirecao(jogo->goban, i, j,-1, 1, 1-jogador.peca);
}

int calcularSeqDirecao(Tabuleiro goban, int i, int j, int di, int dj, Peca peca) {
	Casa casa = {i, j};
	int n1 = 1;
	int n2 = 1;
	int real = 0;
	int n;
	int max1;
	int max2;
	int inicio;
	int final;
	int atual;

	while (verificarSeSeqContinua(goban, casa, di, dj, n1, peca)) {
		n1++;
	}

	max1 = n1;

	while (verificarSeqMax(goban, casa, di, dj, max1, peca)) {
		max1++;
	}

	while (verificarSeSeqContinua(goban, casa, -di, -dj, n2, peca)) {
		n2++;
	}

	max2 = n2;

	while (verificarSeqMax(goban, casa, -di, -dj, max2, peca)) {
		max2++;
	}

	if (max1 + max2 - 1 < 5) {
		return 0; // Sequência falsa, não tem como ganhar com ela
	}

	n = n1 + n2 - 1;

	if (n > 4) {
		return 350;
	}

	if (n == 4) {
		return 100;
	}

	inicio =  n1 - 1 - 4 > -max2 + 1 ?  n1 - 1 - 4 : -max2 + 1;
	final  = -n2 + 1 + 4 <  max1 - 1 ? -n2 + 1     :  max1 - 1 - 4;

	for (; inicio <= final; inicio++) {
		atual = 0;

		for (int k = inicio; k < inicio + 5; k++) {
			if (goban.matriz[i + di*k][j + dj*k] == peca) {
				atual++;
			}
		}

		if (atual > real) {
			real = atual;
		}
	}

	if (real == 4) {
		return 100;
	}

	if (real == 3) {
		return 35;
	}

	if (real == 2) {
		return 10;
	}

	return 0;
}

int verificarSeqMax(Tabuleiro goban, Casa casa, int di, int dj, int n, Peca peca) {
	return ( 
		         casa.lin + di*n >= 0
		      && casa.lin + di*n < goban.dimensao
		      && casa.col + dj*n >= 0
		      && casa.col + dj*n < goban.dimensao
		   )
		   &&
		   (
		   	     goban.matriz[casa.lin + di*n][casa.col + dj*n] == peca
	          ||
	          (
	       	        goban.matriz[casa.lin + di*n][casa.col + dj*n] == -1
	       	     && !verificarFormacao3x3(goban, casa.lin + di*n, casa.col + dj*n, peca)
	          )
	       );
}

void selecionarMelhorJogada(Jogada *jogadas, int n, int *lin, int *col) {
	Jogada melhor;
	int max;
	int i = 1;
	jogadas = insertionSort(jogadas, n);

	while (i < n && jogadas[i].prioridade == jogadas[0].prioridade) {
		i++;
	}

	melhor = jogadas[sortearNumero(0, i-1)];

	*lin = melhor.lin;
	*col = melhor.col;
}

Jogada* insertionSort(Jogada *jogadas, int n) {
	Jogada aux;
	int pivo;

	for (int i = 1; i < n; i++) {
		pivo = i;

		while (pivo > 0 && jogadas[pivo].prioridade > jogadas[pivo - 1].prioridade) {
			aux = jogadas[pivo];
			jogadas[pivo] = jogadas[pivo - 1];
			jogadas[pivo - 1] = aux;
			pivo--;
		}
	}

	return jogadas;
}