#include <stdio.h>
#include <stdlib.h>
#include "branch_and_bound.h"

int** alocarCopiaMatriz(int tam, int** origem) {
    int** copia = malloc(tam * sizeof(int*));
    if (!copia) { perror("malloc"); exit(EXIT_FAILURE); }
    for (int i = 0; i < tam; i++) {
        copia[i] = malloc(tam * sizeof(int));
        if (!copia[i]) { perror("malloc"); exit(EXIT_FAILURE); }
        for (int j = 0; j < tam; j++) copia[i][j] = origem[i][j];
    }
    return copia;
}

int reduzirMatriz(int tam, int** m) {
    int custo = 0;
    // Linhas
    for (int i = 0; i < tam; i++) {
        int menor = INF_COST, zero = 0;
        for (int j = 0; j < tam; j++) {
            if (m[i][j] == 0) { 
                zero = 1; 
                break; 
            }
            if (m[i][j] < menor && m[i][j] != INF_COST) {
                menor = m[i][j];
            }
        }
        if (!zero && menor != INF_COST) {
            custo += menor;
            for (int j = 0; j < tam; j++) {
                if (m[i][j] != INF_COST) {
                    m[i][j] -= menor;
                }
            }
        }
    }
    // Colunas
    for (int j = 0; j < tam; j++) {
        int menor = INF_COST, zero = 0;
        for (int i = 0; i < tam; i++) {
            if (m[i][j] == 0) { 
                zero = 1; 
                break; 
            }
            if (m[i][j] < menor && m[i][j] != INF_COST) {
                menor = m[i][j];
            }
        }
        if (!zero && menor != INF_COST) {
            custo += menor;
            for (int i = 0; i < tam; i++) {
                if (m[i][j] != INF_COST) {
                     m[i][j] -= menor;
                }
            }
        }
    }
    return custo;
}

int caminhoAB(int A, int B, int tam, int** m) {
    for (int i = 0; i < tam; i++) {
        m[A][i] = INF_COST;
        m[i][B] = INF_COST;
    }
    m[B][A] = INF_COST;
    int extra = reduzirMatriz(tam, m);
    return extra;
}

int jaVisitado(int cidade, int* caminho, int nivel) {
    for (int i = 0; i <= nivel; i++) if (caminho[i] == cidade) return 1;
    return 0;
}

No* criarNo(int valorNo, int custo, int nivel, int* caminho, int tam, int** matrizBase) {
    No* no = malloc(sizeof(No));
    if (!no) { perror("malloc"); exit(EXIT_FAILURE); }
    no->valorNo = valorNo;
    no->custo = custo;
    no->nivel = nivel;
    no->caminho = malloc((nivel+1) * sizeof(int));
    for (int i = 0; i <= nivel; i++) no->caminho[i] = caminho[i];
    no->matriz = alocarCopiaMatriz(tam, matrizBase);
    no->numFilhos = 0;
    no->filhos = NULL;
    return no;
}

void liberarArvore(No* no, int tam) {
    if (!no) return;
    for (int i = 0; i < no->numFilhos; i++) liberarArvore(no->filhos[i], tam);
    free(no->filhos);
    free(no->caminho);
    for (int i = 0; i < tam; i++) free(no->matriz[i]);
    free(no->matriz);
    free(no);
}

void branchBound(No* no, int tam, int* melhorCusto, int* melhorCaminho) {
    if (no->nivel == tam - 1) {
        int ultimo = no->valorNo;
        int custoRetorno = no->matriz[ultimo][0]; //cidade inicial retorno

        if (custoRetorno == INF_COST) { //caso o retorno seja infinito, logo nao e um ciclo valido
            return;
        }
        
        int total = no->custo + custoRetorno; //cuto total do ciclo completo, soma o custo acumulado desde o ultimo no ate o custo de retorno a cidade
        
        if (total < *melhorCusto) {
            *melhorCusto = total;
            for (int i = 0; i <= no->nivel; i++) melhorCaminho[i] = no->caminho[i];
            melhorCaminho[tam] = 0;
        }
        return;
    }
    for (int j = 0; j < tam; j++) {
        if (!jaVisitado(j, no->caminho, no->nivel)) {

            if (no->matriz[no->valorNo][j] == INF_COST) { //pula ramificacao caso a dita aresta seja infinita
                continue;
            }

            int** copia = alocarCopiaMatriz(tam, no->matriz);

            int custoAresta = no->matriz[no->valorNo][j]; //vai obter o custo real da aresta atual (no->valorNo) para a proxima cidade (j)

            int extra = caminhoAB(no->valorNo, j, tam, copia);

            int novoCusto = no->custo + custoAresta + extra; //soma acumulada do pai, o custo real da percorrida aresta junto com o custo de reducao adicional da matriz
            if (novoCusto < *melhorCusto) {
                int* novoCam = malloc((no->nivel+2) * sizeof(int));
                for (int k = 0; k <= no->nivel; k++) novoCam[k] = no->caminho[k];
                novoCam[no->nivel+1] = j;
                No* filho = criarNo(j, novoCusto, no->nivel+1, novoCam, tam, copia);
                no->filhos = realloc(no->filhos, (no->numFilhos+1) * sizeof(No*));
                no->filhos[no->numFilhos++] = filho;
                free(novoCam);
                branchBound(filho, tam, melhorCusto, melhorCaminho);
            }
            for (int x = 0; x < tam; x++) free(copia[x]);
            free(copia);
        }
    }
}

void imprimirResultado(int* caminho, char** cidades, int tam) {
    printf("Melhor rota: ");
    for (int i = 0; i <= tam; i++){
        printf("%s ", cidades[caminho[i]]);
        if(i != tam){
            printf("-> ");
        }
    } 
    printf("\n");
}