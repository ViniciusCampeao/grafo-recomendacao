/*
Nome: Vinícius Campeão Fernandes
RA: 2465396

Resumo:
 * Vertices: C1, C2, C3, C4 (clientes) e P1, P2, P3, P4 (produtos).
 * Arestas: C1-P1, C1-P2, C2-P1, C2-P3, C3-P3, C3-P4, C4-P4.
 * Busca em profundidade (DFS): recursiva, com backtracking.
 * Busca em largura (BFS): usa uma fila.

 * Como compilar: gcc -std=c99 -Wall -Wextra -pedantic -o grafo grafo.c
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define N 8
#define TAM_LINHA 64

static const char *NOMES[N] = {"C1", "C2", "C3", "C4", "P1", "P2", "P3", "P4"};
static int adj[N][N];

typedef struct {
    int visitado[N];
    int ordem[N];      /* v na ordem em que foram visitados */
    int qtd_visitados;
    int arestas;       /* a para avancar a vertices novos */
    int caminho[N];    /* caminho origem p/ objetivo */
    int tam_caminho;
    int encontrado;
    int pilha[N];      /* caminho atual da recursao (DFS) */
    int topo;
} Busca;

static void montar_grafo(void) {
    static const int arestas[][2] = {
        {0, 4}, {0, 5}, {1, 4}, {1, 6}, {2, 6}, {2, 7}, {3, 7}
    };
    size_t i;
    for (i = 0; i < sizeof arestas / sizeof arestas[0]; i++) {
        adj[arestas[i][0]][arestas[i][1]] = 1;
        adj[arestas[i][1]][arestas[i][0]] = 1;
    }
}

static int indice_vertice(const char *texto) {
    char nome[TAM_LINHA];
    size_t i, n = strlen(texto);
    int v;
    for (i = 0; i < n && i < sizeof nome - 1; i++)
        nome[i] = (char)toupper((unsigned char)texto[i]);
    nome[i] = '\0';
    for (v = 0; v < N; v++)
        if (strcmp(nome, NOMES[v]) == 0)
            return v;
    return -1;
}

static int ler_linha(const char *prompt, char *buf, size_t tam) {
    size_t inicio = 0, n;
    printf("%s", prompt);
    fflush(stdout);
    if (fgets(buf, (int)tam, stdin) == NULL)
        return 0;
    while (buf[inicio] == ' ' || buf[inicio] == '\t')
        inicio++;
    memmove(buf, buf + inicio, strlen(buf + inicio) + 1);
    n = strlen(buf);
    while (n > 0 && (buf[n - 1] == '\n' || buf[n - 1] == '\r' ||
                     buf[n - 1] == ' ' || buf[n - 1] == '\t'))
        buf[--n] = '\0';
    return 1;
}

static void zerar_busca(Busca *b) {
    memset(b, 0, sizeof *b);
}

static void imprimir_sequencia(const int *seq, int n) {
    int i;
    if (n == 0) {
        printf("nenhum");
        return;
    }
    for (i = 0; i < n; i++) {
        if (i > 0)
            printf(" -> ");
        printf("%s", NOMES[seq[i]]);
    }
}


static int dfs(int u, int destino, Busca *b) {
    int v;
    b->visitado[u] = 1;
    b->ordem[b->qtd_visitados++] = u;
    b->pilha[b->topo++] = u;

    if (u == destino)
        return 1;

    for (v = 0; v < N; v++) {
        if (adj[u][v] && !b->visitado[v]) {
            b->arestas++;
            if (dfs(v, destino, b))
                return 1;
        }
    }

    b->topo--;  
    return 0;
}

static void executar_dfs(int origem, int destino, Busca *b) {
    int i;
    zerar_busca(b);
    b->encontrado = dfs(origem, destino, b);
    if (b->encontrado) {
        for (i = 0; i < b->topo; i++)
            b->caminho[i] = b->pilha[i];
        b->tam_caminho = b->topo;
    }
}

static void executar_bfs(int origem, int destino, Busca *b) {
    int fila[N], pai[N], ini = 0, fim = 0, u, v, i;
    int tmp[N], k = 0, x;

    zerar_busca(b);
    for (v = 0; v < N; v++)
        pai[v] = -1;

    b->visitado[origem] = 1;
    fila[fim++] = origem;

    while (ini < fim) {
        u = fila[ini++];
        b->ordem[b->qtd_visitados++] = u;

        if (u == destino) {
            b->encontrado = 1;
            break;
        }

        for (v = 0; v < N; v++) {
            if (adj[u][v] && !b->visitado[v]) {
                b->visitado[v] = 1;
                pai[v] = u;
                b->arestas++;
                fila[fim++] = v;
            }
        }
    }

    if (b->encontrado) {
        for (x = destino; x != -1; x = pai[x])
            tmp[k++] = x;
        b->tam_caminho = k;
        for (i = 0; i < k; i++)
            b->caminho[i] = tmp[k - 1 - i];
    }
}

static void imprimir_relatorio(const char *titulo, int origem, int destino, const Busca *b) {
    int marcados[N] = {0};
    int i, v, primeiro = 1;

    printf("\n===== %s =====\n", titulo);
    printf("Vertice de origem       : %s\n", NOMES[origem]);
    printf("Vertice objetivo        : %s\n",
           destino >= 0 ? NOMES[destino] : "nao informado (busca completa)");

    printf("Caminho percorrido      : ");
    imprimir_sequencia(b->ordem, b->qtd_visitados);
    printf("\n");

    for (i = 0; i < b->qtd_visitados; i++)
        marcados[b->ordem[i]] = 1;
    printf("Vertices visitados      : ");
    for (v = 0; v < N; v++) {
        if (marcados[v]) {
            if (!primeiro)
                printf(", ");
            printf("%s", NOMES[v]);
            primeiro = 0;
        }
    }
    printf("\n");

    printf("Quantidade de visitados : %d\n", b->qtd_visitados);
    printf("Arestas percorridas     : %d\n", b->arestas);

    printf("Caminho ate o objetivo  : ");
    if (destino < 0) {
        printf("nao se aplica\n");
    } else if (b->encontrado) {
        imprimir_sequencia(b->caminho, b->tam_caminho);
        printf("\n");
    } else {
        printf("nenhum\n");
    }

    printf("Objetivo encontrado     : ");
    if (destino < 0)
        printf("nao se aplica\n");
    else
        printf("%s\n", b->encontrado ? "SIM" : "NAO");
}

static void mostrar_lista(void) {
    int u, v, primeiro;
    printf("\nLista de adjacencia:\n");
    for (u = 0; u < N; u++) {
        printf("%s: ", NOMES[u]);
        primeiro = 1;
        for (v = 0; v < N; v++) {
            if (adj[u][v]) {
                if (!primeiro)
                    printf(", ");
                printf("%s", NOMES[v]);
                primeiro = 0;
            }
        }
        printf("\n");
    }
}

static int pedir_busca(int *origem, int *destino) {
    char linha[TAM_LINHA];

    if (!ler_linha("Vertice de origem (ex.: C1): ", linha, sizeof linha))
        return -1;
    *origem = indice_vertice(linha);
    if (*origem < 0) {
        printf("Vertice de origem invalido.\n");
        return 0;
    }

    if (!ler_linha("Vertice objetivo (ENTER = busca completa): ", linha, sizeof linha))
        return -1;
    if (linha[0] == '\0') {
        *destino = -1;
        return 1;
    }
    *destino = indice_vertice(linha);
    if (*destino < 0) {
        printf("Vertice objetivo invalido.\n");
        return 0;
    }
    return 1;
}

int main(void) {
    char linha[TAM_LINHA];
    int opcao, origem = 0, destino = -1, r;
    Busca b;

    montar_grafo();

    for (;;) {
        printf("\n========== MENU ==========\n");
        printf("1 - Busca em profundidade (DFS)\n");
        printf("2 - Busca em largura (BFS)\n");
        printf("3 - Mostrar lista de adjacencia\n");
        printf("0 - Sair\n");

        if (!ler_linha("Opcao: ", linha, sizeof linha))
            break;
        if (sscanf(linha, "%d", &opcao) != 1)
            opcao = -1;

        if (opcao == 1 || opcao == 2) {
            r = pedir_busca(&origem, &destino);
            if (r < 0)
                break;
            if (r == 0)
                continue;
            if (opcao == 1) {
                executar_dfs(origem, destino, &b);
                imprimir_relatorio("Busca em profundidade (DFS)", origem, destino, &b);
            } else {
                executar_bfs(origem, destino, &b);
                imprimir_relatorio("Busca em largura (BFS)", origem, destino, &b);
            }
        } else if (opcao == 3) {
            mostrar_lista();
        } else if (opcao == 0) {
            break;
        } else {
            printf("Opcao invalida.\n");
        }
    }

    printf("Encerrando.\n");
    return 0;
}