#include <stdio.h>
#include <stdlib.h>
#include "branch_and_bound.h" //Para INT_MAX, que representa o "infinito"

//Interações com o usuário
char **nome_cidades;

int** criar_matriz(int tam){
    int** matriz;
    matriz = (int**) malloc(tam * sizeof(int*));
    
    for(int i = 0; i < tam; i++){
        matriz[i] = malloc(tam * sizeof(int));
        for(int j = 0; j < tam; j++){
            matriz[i][j] = INT_MAX;
        }
    }

    return matriz;
}

void alocar_nome_cidades(int tam){
    nome_cidades = (char**) malloc(tam * sizeof(char*));
        
    for(int i = 0; i < tam; i++){
        nome_cidades[i] = malloc(40 * sizeof(char));
    }
} 

void popular_matriz_princial(int **matriz, int tam){
    for(int i = 0; i < tam; i ++){
        for(int j = 0; j < tam; j++){
            
            if(i == j){ 
                continue;
            }

            if(matriz[i][j] == INT_MAX){
                printf("Informe o custo/distancia de %s para %s: \n", nome_cidades[i], nome_cidades[j]);
                scanf("%d", &matriz[i][j]);
                matriz[j][i] = matriz[i][j]; 
            }
        }
    }
}

int liberar_matriz(int** matriz, int tam){
    for(int i = 0; i < tam; i++){
        free(matriz[i]);
    }
    free(matriz);
}

int liberar_nome_cidades(int tam){
    for(int i = 0; i < tam; i++){
        free(nome_cidades[i]);
    }
    free(nome_cidades);
}

void limpar_buffer() {
    //printf("\nPressione enter tecla para continuar...\n");
    //getchar();
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(){
    int tam;
    int **matriz;//Matriz principal
    printf("Infome a quantidade de cidades: \n");
    scanf("%d", &tam);

    matriz = criar_matriz(tam);
    alocar_nome_cidades(tam);

    printf("Informe o nome da cidade principal:\n");
    scanf(" %100[^\n]", nome_cidades[0]);

    for(int i = 1; i < tam; i++){
        printf("Informe o nome da próxima cidade:\n");
        scanf(" %100[^\n]", nome_cidades[i]);
    }
    
    popular_matriz_princial(matriz, tam);

    int** base = malloc(tam * sizeof(int*));
    for (int i = 0; i < tam; i++) {
        base[i] = malloc(tam * sizeof(int));
        for (int j = 0; j < tam; j++) base[i][j] = matriz[i][j];
    }
    int custo0 = reduzirMatriz(tam, base);
    int caminho0[1] = {0};
    No* raiz = criarNo(0, custo0, 0, caminho0, tam, base);
    int melhorCusto = INF_COST;
    int melhorCaminho[ tam + 1];
    branchBound(raiz, tam, &melhorCusto, melhorCaminho);
    printf("\nCusto mínimo: %d\n", melhorCusto);
    imprimirResultado(melhorCaminho, nome_cidades, tam);
    liberarArvore(raiz, tam);
    for (int i = 0; i < tam; i++) free(base[i]);
    free(base);

    liberar_matriz(matriz, tam);
    liberar_nome_cidades(tam);

    return 0;
}