# Problema do Caixeiro Viajante - Algoritmo Branch and Bound

Este projeto implementa uma solução para o **Problema do Caixeiro Viajante (TSP)** usando o algoritmo **Branch and Bound** em linguagem C. O objetivo é encontrar o caminho de menor custo/distância para visitar todas as cidades uma única vez e retornar à cidade de origem.

---

## 📌 Algoritmo Utilizado

O algoritmo **Branch and Bound** trabalha com poda de estados não promissores para reduzir o espaço de busca de soluções. Ele utiliza os seguintes passos principais:

1. **Redução de Matriz**: Inicialmente reduz a matriz de custos para obter uma estimativa inferior do custo.
2. **Exploração de Estados**: Para cada cidade, calcula o custo de continuar por ela e compara com o melhor custo atual.
3. **Poda**: Se o custo atual for maior que o melhor encontrado, o ramo é descartado.
4. **Recursão**: A função segue recursivamente até visitar todas as cidades.

---

## 📂 Estrutura do Projeto

```
Trabalho_Pratico_Final_O_Problema_do_Caixeiro_Viajante-main/
│
├── codigos/
│   ├── main.c                 # Interação com o usuário e montagem da matriz de distâncias
│   ├── branch_and_bound.c     # Implementação do algoritmo Branch and Bound
│   ├── branch_and_bound.h     # Interface do algoritmo
│
└── README.md                  # Documentação do projeto
```

---

## 🧪 Como Compilar

Para compilar o projeto, abra um terminal na pasta `implementacao/` e execute o seguinte comando:

```bash
gcc main.c branch_and_bound.c -o tsp
```

Isso criará um executável chamado `tsp`.

---

## ▶️ Como Executar

1. Execute o programa:

```bash
./tsp
```

2. Informe a **quantidade de cidades**.
3. Digite o **nome de cada cidade**.
4. Para cada par de cidades, informe o custo/distância entre elas.

O programa exibirá:

- O **menor caminho encontrado**.
- O **custo total** da viagem.

---

## 🧠 Exemplo de Execução

```
Quantas cidades deseja informar? 3

Informe o nome da cidade 1: A
Informe o nome da cidade 2: B
Informe o nome da cidade 3: C

Informe o custo/distância de A para B: 10
Informe o custo/distância de A para C: 15
Informe o custo/distância de B para C: 20
...

Menor caminho: A -> B -> C -> A
Custo total: 45
```

---

## 🛠️ Requisitos

- GCC ou outro compilador C
- Sistema operacional Unix-like (Linux, macOS) ou Windows com terminal compatível

---

## 👨‍💻 Autoria

Este projeto foi desenvolvido como trabalho prático final para a disciplina de Estrutura de Dados II.
Discentes:

- [Andersson Silva Pereira](https://github.com/Moab76)
- [Álefe Alves](https://github.com/AlefeAlvesC)
- [José Carvalho](https://github.com/JcVison)
- [Matheus Rebouças](https://github.com/Math77077)

---

## 📃 Licença

Este projeto é de uso acadêmico. Para outros usos, consulte os autores.
