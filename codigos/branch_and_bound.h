#ifndef BRANCH_AND_BOUND_H
#define BRANCH_AND_BOUND_H

#include <limits.h>

// Representa "infinito" em custos
#define INF_COST INT_MAX

// Estrutura de nó para Branch and Bound
typedef struct no {
    int valorNo;       // Vértice atual
    int custo;         // Custo acumulado
    int nivel;         // Profundidade do caminho
    int* caminho;      // Sequência de vértices visitados
    int** matriz;      // Matriz reduzida de custos
    int numFilhos;     // Quantidade de filhos
    struct no** filhos;// Vetor de ponteiros para filhos
} No;

// Protótipos de funções
int** alocarCopiaMatriz(int tam, int** origem);

int reduzirMatriz(int tam, int** m);

int caminhoAB(int A, int B, int tam, int** m);

No* criarNo(int valorNo, int custo, int nivel, int* caminho, int tam, int** matrizBase);

void liberarArvore(No* no, int tam);

void branchBound(No* no, int tam, int* melhorCusto, int* melhorCaminho);

int jaVisitado(int cidade, int* caminho, int nivel);

void imprimirResultado(int* caminho, char** cidades, int tam);

#endif // BRANCH_AND_BOUND_H