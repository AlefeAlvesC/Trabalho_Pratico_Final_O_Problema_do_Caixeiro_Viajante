#include <stdio.h>
#include <stdlib.h>
#include "branch_and_bound.h"

// Imprime matriz com "INF" para INF_COST
void imprimirMatriz(int tam, int** m) {
    printf("Matriz (nível):\n");
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            if (m[i][j] == INF_COST) printf("INF ");
            else printf("%3d ", m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

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
            if (m[i][j] == 0) { zero = 1; break; }
            if (m[i][j] < menor) menor = m[i][j];
        }
        if (!zero && menor != INF_COST) {
            custo += menor;
            for (int j = 0; j < tam; j++) if (m[i][j] != INF_COST) m[i][j] -= menor;
        }
    }
    // Colunas
    for (int j = 0; j < tam; j++) {
        int menor = INF_COST, zero = 0;
        for (int i = 0; i < tam; i++) {
            if (m[i][j] == 0) { zero = 1; break; }
            if (m[i][j] < menor) menor = m[i][j];
        }
        if (!zero && menor != INF_COST) {
            custo += menor;
            for (int i = 0; i < tam; i++) if (m[i][j] != INF_COST) m[i][j] -= menor;
        }
    }
    printf("Custo de redução: %d\n", custo);
    imprimirMatriz(tam, m);
    return custo;
}

int caminhoAB(int A, int B, int tam, int** m) {
    printf("Expandindo caminho %d -> %d\n", A, B);
    for (int i = 0; i < tam; i++) {
        m[A][i] = INF_COST;
        m[i][B] = INF_COST;
    }
    m[B][A] = INF_COST;
    imprimirMatriz(tam, m);
    int extra = reduzirMatriz(tam, m);
    printf("Custo extra caminho %d->%d: %d\n\n", A, B, extra);
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
    printf("Criado nó %d com custo %d no nível %d\n", valorNo, custo, nivel);
    imprimirMatriz(tam, no->matriz);
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
        int retorno = (no->matriz[ultimo][0] == INF_COST) ? INF_COST : no->matriz[ultimo][0];
        int total = (retorno == INF_COST) ? INF_COST : no->custo + retorno;
        printf("Fechando ciclo: custo total %d\n", total);
        if (total < *melhorCusto) {
            *melhorCusto = total;
            for (int i = 0; i <= no->nivel; i++) melhorCaminho[i] = no->caminho[i];
            melhorCaminho[tam] = 0;
        }
        return;
    }
    for (int j = 0; j < tam; j++) {
        if (!jaVisitado(j, no->caminho, no->nivel)) {
            int** copia = alocarCopiaMatriz(tam, no->matriz);
            int extra = caminhoAB(no->valorNo, j, tam, copia);
            int novoCusto = no->custo + extra;
            printf("Custo parcial até nó %d: %d\n", j, novoCusto);
            if (novoCusto < *melhorCusto) {
                int* novoCam = malloc((no->nivel+2) * sizeof(int));
                for (int k = 0; k <= no->nivel; k++) novoCam[k] = no->caminho[k];
                novoCam[no->nivel+1] = j;
                No* filho = criarNo(j, novoCusto, no->nivel+1, novoCam, tam, copia);
                no->filhos = realloc(no->filhos, (no->numFilhos+1) * sizeof(No*));
                no->filhos[no->numFilhos++] = filho;
                free(novoCam);
                branchBound(filho, tam, melhorCusto, melhorCaminho);
            } else {
                printf("Poda: custo %d >= melhorCusto %d\n\n", novoCusto, *melhorCusto);
            }
            for (int x = 0; x < tam; x++) free(copia[x]);
            free(copia);
        }
    }
}

void imprimirResultado(int* caminho, int tam) {
    printf("Melhor rota: ");
    for (int i = 0; i <= tam; i++) printf("%d ", caminho[i]);
    printf("\n");
}

int main(void) {
    int tam = 4;
    int exemplo[4][4] = {
        {INF_COST, 10, 15, 20},
        {10, INF_COST, 35, 25},
        {15, 35, INF_COST, 30},
        {20, 25, 30, INF_COST}
    };
    int** base = malloc(tam * sizeof(int*));
    for (int i = 0; i < tam; i++) {
        base[i] = malloc(tam * sizeof(int));
        for (int j = 0; j < tam; j++) base[i][j] = exemplo[i][j];
    }
    int custo0 = reduzirMatriz(tam, base);
    int caminho0[1] = {0};
    No* raiz = criarNo(0, custo0, 0, caminho0, tam, base);
    int melhorCusto = INF_COST;
    int melhorCaminho[ tam + 1];
    branchBound(raiz, tam, &melhorCusto, melhorCaminho);
    printf("\nCusto mínimo: %d\n", melhorCusto);
    imprimirResultado(melhorCaminho, tam);
    liberarArvore(raiz, tam);
    for (int i = 0; i < tam; i++) free(base[i]);
    free(base);
    return EXIT_SUCCESS;
}
