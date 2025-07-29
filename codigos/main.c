#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "branch_and_bound.h"

//Variáveis globais referentes aos nomes das cidades, a matriz de adjacência e o tamanho da matriz
char **nome_cidades;
int **matriz;
int tam;

//Função para criar uma matriz de adjacência inicializada com INT_MAX
//Ela recebe o tamanho da matriz e retorna um ponteiro para a matriz alocada
int** criar_matriz(int tam) {
    int** matriz;
    matriz = (int**) malloc(tam * sizeof(int*));
    
    for(int i = 0; i < tam; i++) {
        matriz[i] = malloc(tam * sizeof(int));
        for(int j = 0; j < tam; j++) {
            matriz[i][j] = INT_MAX;
        }
    }
    return matriz;
}

//Função para alocar memória para os nomes das cidades
//Ela recebe o tamanho da matriz e aloca memória para cada nome de cidade
void alocar_nome_cidades(int tam) {
    nome_cidades = (char**) malloc(tam * sizeof(char*));
    for(int i = 0; i < tam; i++) {
        nome_cidades[i] = malloc(40 * sizeof(char));
    }
}

//Função para popular a matriz de adjacência com os custos entre as cidades
//Ela recebe a matriz e o tamanho da matriz, e solicita ao usuário os custos entre as cidades
void popular_matriz_adjacencia(int **matriz, int tam) {
    for(int i = 0; i < tam; i++) {
        for(int j = 0; j < tam; j++) {
            if(i == j) continue;
            if(matriz[i][j] == INT_MAX) {
                printf("Informe o custo/distancia de %s para %s: \n", nome_cidades[i], nome_cidades[j]);
                scanf("%d", &matriz[i][j]);
                matriz[j][i] = matriz[i][j]; 
            }
        }
    }
}

//Função para liberar a memória alocada para a matriz de adjacência
//Ela recebe a matriz e o tamanho da matriz
void liberar_matriz(int** matriz, int tam) {
    for(int i = 0; i < tam; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

//Função para liberar a memória alocada para os nomes das cidades
//Ela recebe o tamanho da matriz e libera cada nome de cidade
void liberar_nome_cidades(int tam) {
    for(int i = 0; i < tam; i++) {
        free(nome_cidades[i]);
    }
    free(nome_cidades);
}

//Função para limpar o buffer de entrada
//Ela lê até encontrar um '\n' ou EOF, descartando os caracteres
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//Função para tratamento de entrada de inteiros
//Ela recebe um prompt, um valor mínimo e máximo, e retorna um inteiro válido dentro desse intervalo
//Se a entrada for inválida, solicita novamente até receber um valor válido
int lerInteiro(const char *prompt, int min, int max) {
    char input[50];
    int valor;
    
    while (1) {
        printf("%s", prompt);
        fgets(input, sizeof(input), stdin);
        
        if (sscanf(input, "%d", &valor) == 1) {
            if (valor >= min && valor <= max) {
                return valor;
            }
            printf("Valor deve estar entre %d e %d. Tente novamente.\n", min, max);
        } else {
            printf("Entrada invalida. Digite um numero inteiro. Tente novamente.\n");
        }
    }
}

//Função para tratamento de entrada de nomes de cidades
//Ela recebe um ponteiro para o nome da cidade e um prompt, e solicita ao usuário o nome
//Se o nome for vazio, solicita novamente até receber um nome válido
void lerNomeCidade(char *nome, const char *prompt) {
    while (1) {
        printf("%s", prompt);
        fgets(nome, 40, stdin);
        nome[strcspn(nome, "\n")] = '\0';
        
        if (strlen(nome) > 0) {
            return;
        }
        printf("Nome da cidade nao pode ser vazio. Tente novamente.\n");
    }
}

//Função para exibir o menu principal do sistema
//Ela apresenta as opções disponíveis e executa a ação correspondente à opção escolhida pelo usuário
void menuPrincipal() {
    int opcao;
    
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Adicionar cidade\n");
        printf("2. Definir distancias entre cidades\n");
        printf("3. Ver distancia entre duas cidades\n");
        printf("4. Listar todas as cidades\n");
        printf("5. Calcular rota otimizada (Branch and Bound)\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();
        
        switch(opcao) {
            //Opcao 1: Adicionar cidade
            //Permite ao usuário adicionar uma nova cidade à matriz de adjacência
            case 1: {
                if (tam >= 20) { // Limite máximo do branch_and_bound.h
                    printf("Limite maximo de cidades atingido!\n");
                    break;
                }

                int novoTam = tam + 1;

                int** tempMatriz = realloc(matriz, novoTam * sizeof(int*)); //realocacao do array de ponteiros de linha
                if(!tempMatriz) {
                    perror("Realloc para linhas da matriz");
                    exit(EXIT_FAILURE);
                }
                matriz = tempMatriz;

                for (int i = 0; i < tam; i++) { //realocacao e inicializacao das linhas existentes com a nova coluna
                    int* tempLinha = realloc(matriz[i], novoTam * sizeof(int));
                    if(!tempLinha) {
                        perror("Realloc para colunas da matriz");
                        exit(EXIT_FAILURE);
                    }
                    matriz[i] = tempLinha;
                    matriz[i][novoTam - 1] = INT_MAX;
                }

                matriz[novoTam - 1] = malloc(novoTam * sizeof(int)); //aloca e inicializa a nova linha
                if(!matriz[novoTam - 1]){
                    perror("Realloc para nova linha da matriz");
                    exit(EXIT_FAILURE);
                }
                for (int j = 0; j < novoTam; j++) {
                    matriz[novoTam - 1][j] = (j == novoTam - 1) ? 0 : INT_MAX;
                }

                char **tempNomeCidades = realloc(nome_cidades, novoTam * sizeof(char*)); //atualizacao dos nome das cidades
                if (!tempNomeCidades) {
                    perror("realloc para nomes das cidades");
                    exit(EXIT_FAILURE);
                }
                nome_cidades = tempNomeCidades;
                nome_cidades[novoTam - 1] = malloc(40 * sizeof(char));
                if (!nome_cidades[novoTam - 1]) {
                    perror("malloc para novo nome de cidade");
                    exit(EXIT_FAILURE);
                }

                lerNomeCidade(nome_cidades[novoTam-1], "Informe o nome da nova cidade: ");
                tam = novoTam;
                break;
            }
            
            //Opcao 2: Definir distâncias entre cidades
            //Permite ao usuário definir as distâncias entre as cidades já cadastradas
            case 2: {
                if (tam < 2) {
                    printf("E necessario ter pelo menos 2 cidades!\n");
                    break;
                }
                popular_matriz_adjacencia(matriz, tam);
                break;
            }
                
            case 3: {
                if (tam < 2) {
                    printf("E necessario ter pelo menos 2 cidades!\n");
                    break;
                }
                printf("\n=== CIDADES DISPONIVEIS ===\n");
                for (int i = 0; i < tam; i++) {
                    printf("%d. %s\n", i, nome_cidades[i]);
                }
                int cidade1 = lerInteiro("\nDigite o ID da primeira cidade: ", 0, tam-1);
                int cidade2 = lerInteiro("Digite o ID da segunda cidade: ", 0, tam-1);
                
                if (matriz[cidade1][cidade2] == INT_MAX) {
                    printf("Nao ha rota cadastrada entre %s e %s\n", nome_cidades[cidade1], nome_cidades[cidade2]);
                } else {
                    printf("Distancia entre %s e %s: %d\n", nome_cidades[cidade1], nome_cidades[cidade2], matriz[cidade1][cidade2]);
                }
                break;
            }
            
            //Opcao 4: Listar todas as cidades
            //Exibe uma lista de todas as cidades cadastradas no sistema
            case 4: {
                printf("\n=== LISTA DE CIDADES ===\n");
                for (int i = 0; i < tam; i++) {
                    printf("%d. %s\n", i, nome_cidades[i]);
                }
                break;
            }
            
            //Opcao 5: Calcular rota otimizada (Branch and Bound)
            //Executa o algoritmo Branch and Bound para encontrar a rota mais curta entre todas as cidades
            case 5: {
                if (tam < 2) {
                    printf("E necessario ter pelo menos 2 cidades!\n");
                    break;
                }
                
                int** base = malloc(tam * sizeof(int*));
                for (int i = 0; i < tam; i++) {
                    base[i] = malloc(tam * sizeof(int));
                    for (int j = 0; j < tam; j++) base[i][j] = matriz[i][j];
                }
                
                int custo0 = reduzirMatriz(tam, base);
                int caminho0[1] = {0};
                No* raiz = criarNo(0, custo0, 0, caminho0, tam, base);
                int melhorCusto = INT_MAX;
                int melhorCaminho[tam + 1];
                
                branchBound(raiz, tam, &melhorCusto, melhorCaminho);
                if (melhorCusto == INT_MAX) {
                    printf("\nNao foi possivel encontrar um ciclo completo com custo finito.\n");
                    printf("Verifique se todas as cidades estao conectadas e possuem custo validos.\n");
                } else {
                    printf("\nCusto minimo: %d\n", melhorCusto);
                    imprimirResultado(melhorCaminho, nome_cidades, tam);
                }
                
                liberarArvore(raiz, tam);
                for (int i = 0; i < tam; i++) free(base[i]);
                free(base);
                break;
            }
            
            //Opcao 6: Sair
            //Encerra o programa
            case 6:
                printf("Encerrando o programa...\n");
                break;
                
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 6);
}

int main() {
    printf("SISTEMA DE GERENCIAMENTO DE ROTAS - CAIXEIRO VIAJANTE\n\n");
    
    //Recebe o tamanho inicial da matriz de adjacência
    //O usuário deve informar a quantidade de cidades que deseja cadastrar inicialmente
    printf("Informe a quantidade inicial de cidades: \n");
    tam = lerInteiro("", 1, 20);
    
    // Alocar memória para a matriz de adjacência e os nomes das cidades
    matriz = criar_matriz(tam);
    alocar_nome_cidades(tam);
    
    // Ler os nomes das cidades
    for(int i = 0; i < tam; i++) {
        char prompt[50];
        sprintf(prompt, "Informe o nome da cidade %d: ", i+1);
        lerNomeCidade(nome_cidades[i], prompt);
    }
    
    
    // Menu integrado
    menuPrincipal();
    
    // Liberar memória
    liberar_matriz(matriz, tam);
    liberar_nome_cidades(tam);
    
    return 0;
}