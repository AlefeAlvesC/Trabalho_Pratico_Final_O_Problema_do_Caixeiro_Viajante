#include <stdio.h>
#include <stdlib.h>
#include "branch_and_bound.h"

/*
 * Aloca e copia uma matriz dinamicamente
 * Parâmetros:
 *   tam - tamanho da matriz (número de cidades)
 *   origem - matriz original a ser copiada
 * Retorno:
 *   Ponteiro para a nova matriz alocada e preenchida
 */
int** alocarCopiaMatriz(int tam, int** origem) {
    // Aloca vetor de ponteiros para as linhas
    int** copia = malloc(tam * sizeof(int*));
    if (!copia) { perror("malloc"); exit(EXIT_FAILURE); } // Verifica falha na alocação
    
    // Aloca cada linha da matriz
    for (int i = 0; i < tam; i++) {
        copia[i] = malloc(tam * sizeof(int));
        if (!copia[i]) { perror("malloc"); exit(EXIT_FAILURE); } // Verifica falha
        
        // Copia os valores da matriz original
        for (int j = 0; j < tam; j++) 
            copia[i][j] = origem[i][j];
    }
    return copia;
}

/*
 * Reduz a matriz subtraindo o menor valor de cada linha e coluna
 * Parâmetros:
 *   tam - tamanho da matriz
 *   m - matriz a ser reduzida (modificada in-place)
 * Retorno:
 *   Custo total da redução (soma dos menores valores)
 */
int reduzirMatriz(int tam, int** m) {
    int custo = 0;
    
    // Redução por linhas
    for (int i = 0; i < tam; i++) {
        int menor = INF_COST, zero = 0;
        
        // Encontra o menor valor na linha
        for (int j = 0; j < tam; j++) {
            if (m[i][j] == 0) { 
                zero = 1; 
                break;  // Já tem zero, não precisa reduzir
            }
            if (m[i][j] < menor && m[i][j] != INF_COST) {
                menor = m[i][j];
            }
        }
        
        // Reduz a linha se não tiver zero
        if (!zero && menor != INF_COST) {
            custo += menor;
            for (int j = 0; j < tam; j++) {
                if (m[i][j] != INF_COST) {
                    m[i][j] -= menor;
                }
            }
        }
    }
    
    // Redução por colunas (similar ao das linhas)
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

/*
 * Remove as arestas de A para todos e de todos para B
 * e impede o retorno de B para A
 * Parâmetros:
 *   A - cidade de origem
 *   B - cidade de destino
 *   tam - tamanho da matriz
 *   m - matriz a ser modificada
 * Retorno:
 *   Custo adicional da redução da matriz resultante
 */
int caminhoAB(int A, int B, int tam, int** m) {
    // Remove todas as arestas saindo de A
    for (int i = 0; i < tam; i++) {
        m[A][i] = INF_COST;
    }
    
    // Remove todas as arestas entrando em B
    for (int i = 0; i < tam; i++) {
        m[i][B] = INF_COST;
    }
    
    // Impede o retorno direto de B para A
    m[B][A] = INF_COST;
    
    // Reduz a matriz resultante
    int extra = reduzirMatriz(tam, m);
    return extra;
}

/*
 * Verifica se uma cidade já está no caminho atual
 * Parâmetros:
 *   cidade - cidade a verificar
 *   caminho - array com o caminho percorrido
 *   nivel - tamanho atual do caminho
 * Retorno:
 *   1 se a cidade já está no caminho, 0 caso contrário
 */
int jaVisitado(int cidade, int* caminho, int nivel) {
    for (int i = 0; i <= nivel; i++) 
        if (caminho[i] == cidade) 
            return 1;
    return 0;
}

/*
 * Cria um novo nó para a árvore de busca
 * Parâmetros:
 *   valorNo - cidade atual
 *   custo - custo acumulado
 *   nivel - profundidade na árvore
 *   caminho - caminho percorrido até o nó
 *   tam - tamanho da matriz
 *   matrizBase - matriz de custos
 * Retorno:
 *   Ponteiro para o novo nó alocado
 */
No* criarNo(int valorNo, int custo, int nivel, int* caminho, int tam, int** matrizBase) {
    No* no = malloc(sizeof(No));
    if (!no) { perror("malloc para no"); exit(EXIT_FAILURE); }
    
    no->valorNo = valorNo;
    no->custo = custo;
    no->nivel = nivel;
    
    // Aloca e copia o caminho
    no->caminho = malloc((nivel+1) * sizeof(int));
    if (!no->caminho) { perror("malloc para no->caminho"); exit(EXIT_FAILURE); }
    for (int i = 0; i <= nivel; i++) 
        no->caminho[i] = caminho[i];
    
    // Cria cópia da matriz
    no->matriz = alocarCopiaMatriz(tam, matrizBase);
    no->numFilhos = 0;
    no->filhos = NULL;
    
    return no;
}

/*
 * Libera a memória de uma árvore de busca recursivamente
 * Parâmetros:
 *   no - nó raiz da árvore/subárvore
 *   tam - tamanho da matriz (para liberar corretamente)
 */
void liberarArvore(No* no, int tam) {
    if (!no) return;
    
    // Libera os filhos recursivamente
    for (int i = 0; i < no->numFilhos; i++) 
        liberarArvore(no->filhos[i], tam);
    
    // Libera os arrays alocados
    free(no->filhos);
    free(no->caminho);
    
    // Libera a matriz
    for (int i = 0; i < tam; i++) 
        free(no->matriz[i]);
    free(no->matriz);
    
    // Libera o próprio nó
    free(no);
}

/*
 * Algoritmo principal Branch and Bound
 * Parâmetros:
 *   no - nó atual sendo processado
 *   tam - número de cidades
 *   melhorCusto - ponteiro para o melhor custo encontrado
 *   solucoes - estrutura para armazenar as soluções encontradas
 */
void branchBound(No* no, int tam, int* melhorCusto, Solucoes* solucoes) {
    // Caso base: chegou ao final do caminho
    if (no->nivel == tam - 1) {
        int ultimo = no->valorNo;
        int custoRetorno = no->matriz[ultimo][0]; // Custo para voltar ao início
        
        // Se não tem como voltar, descarta
        if (custoRetorno == INF_COST) return;
        
        // Calcula custo total do ciclo
        int total = no->custo + custoRetorno;
        
        // Encontrou um custo melhor
        if (total < *melhorCusto) {
            *melhorCusto = total;
            solucoes->numCaminhos = 0; // Reseta as soluções
        }
        
        // Adiciona à lista se for uma solução ótima
        if (total == *melhorCusto) {
            // verifica o realloc
            int** temp_caminhos = realloc(solucoes->caminhos, (solucoes->numCaminhos + 1) * sizeof(int*));
            if (!temp_caminhos) {
                perror("realloc para solucoes->caminhos");
                exit(EXIT_FAILURE);
            }
            solucoes->caminhos = temp_caminhos; //se deu certo

            solucoes->caminhos[solucoes->numCaminhos] = malloc((tam + 1) * sizeof(int));
            if (!solucoes->caminhos[solucoes->numCaminhos]) {
                perror("malloc para solucoes->caminhos[numCaminhos]");
                exit(EXIT_FAILURE);
            }
            
            // Copia o caminho
            for (int i = 0; i <= no->nivel; i++) {
                solucoes->caminhos[solucoes->numCaminhos][i] = no->caminho[i];
            }
            solucoes->caminhos[solucoes->numCaminhos][tam] = 0; // Completa o ciclo
            solucoes->numCaminhos++;
        }
        return;
    }
    
    // Para cada cidade possível
    for (int j = 0; j < tam; j++) {
        // Se não foi visitada e a aresta existe
        if (!jaVisitado(j, no->caminho, no->nivel) && no->matriz[no->valorNo][j] != INF_COST) {
            // Cria cópia da matriz para modificação
            int** copia = alocarCopiaMatriz(tam, no->matriz);
            
            // Calcula custos
            int custoAresta = no->matriz[no->valorNo][j];
            int extra = caminhoAB(no->valorNo, j, tam, copia);
            int novoCusto = no->custo + custoAresta + extra;

            // Cria novo caminho
            int* novoCam = malloc((no->nivel + 2) * sizeof(int));
            if (!novoCam) { perror("malloc"); exit(EXIT_FAILURE); }

            for (int k = 0; k <= no->nivel; k++) 
                novoCam[k] = no->caminho[k];
            novoCam[no->nivel + 1] = j;
            
            // Se for promissor (custo <= melhor conhecido)
            if (novoCusto <= *melhorCusto) {
                
                // Cria novo nó filho
                No* filho = criarNo(j, novoCusto, no->nivel + 1, novoCam, tam, copia);
                
                // Adiciona à lista de filhos
                no->filhos = realloc(no->filhos, (no->numFilhos + 1) * sizeof(No*));
                if (!no->filhos) { perror("realloc"); exit(EXIT_FAILURE); }
                no->filhos[no->numFilhos++] = filho;
                
                // Chamada recursiva
                branchBound(filho, tam, melhorCusto, solucoes);
            }

            free(novoCam);
            
            // Libera a cópia da matriz
            for (int x = 0; x < tam; x++) 
                free(copia[x]);
            free(copia);
        }
    }
}

/*
 * Imprime o resultado com todas as rotas ótimas encontradas
 * Parâmetros:
 *   solucoes - estrutura com as soluções
 *   cidades - nomes das cidades
 *   tam - número de cidades
 */
void imprimirResultado(Solucoes* solucoes, char** cidades, int tam) {
    printf("\n=== MELHORES ROTAS ENCONTRADAS ===\n");
    printf("Custo mínimo: %d\n", solucoes->custo);
    printf("Número de rotas ótimas: %d\n\n", solucoes->numCaminhos);

    // Imprime cada rota encontrada
    for (int i = 0; i < solucoes->numCaminhos; i++) {
        printf("Rota %d: ", i + 1);
        for (int j = 0; j <= tam; j++) {
            printf("%s", cidades[solucoes->caminhos[i][j]]);
            if (j != tam) printf(" -> ");
        }
        printf("\n");
    }
}
