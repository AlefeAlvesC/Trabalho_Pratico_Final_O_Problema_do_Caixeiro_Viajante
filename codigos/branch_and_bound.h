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

//Função que realiza uma copia do conteúdo da matriz passada por parâmentro
//Ela recebe o tamanho da matriz e a matriz de origem
int** alocarCopiaMatriz(int tam, int** origem);

//Função para realizar a redução de uma matriz
//Ela recebe o tamanho da matriz e a matriz de custos
int reduzirMatriz(int tam, int** m);

//Função que calcula o custo adicional ao remover arestas entre duas cidades
//Ela recebe as cidades A e B, o tamanho da matriz e a matriz de custos
int caminhoAB(int A, int B, int tam, int** m);

//Função para criar e retornar um novo nó da árvore de decisões
//Ela recebe o valor do nó, custo acumulado, nível atual, caminho percorrido, tamanho da matriz e a matriz base
No* criarNo(int valorNo, int custo, int nivel, int* caminho, int tam, int** matrizBase);

//Função para liberar a memória alocada a árvore de decisões
//Ela recebe o nó atual e o tamanho da matriz
void liberarArvore(No* no, int tam);

//Função principal responsável por implementar o algoritmo Branch and Bound
//Ela recebe o nó atual, o tamanho da matriz, o melhor custo encontrado até agora e o melhor caminho encontrado
void branchBound(No* no, int tam, int* melhorCusto, int* melhorCaminho);

//Função para verificar se uma cidade já foi visitada no caminho atual
//Ela recebe a cidade, o caminho atual e o nível atual do nó
int jaVisitado(int cidade, int* caminho, int nivel);

//Função para imprimir o resultado final do melhor caminho encontrado
//Ela recebe o caminho, as cidades e o tamanho do caminho
void imprimirResultado(int* caminho, char** cidades, int tam);

#endif // BRANCH_AND_BOUND_H