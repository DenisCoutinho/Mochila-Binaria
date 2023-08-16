/*----------------------------------------------------------------------------------------------------------------------------------/
 *             Unifal - Universidade Federal de Alfenas
 *                BACHARELADO EM CIENCIA DA COMPUTACAO
 * Trabalho..: Trabalho prático 2 - Paradigmas de Projeto de Algoritmos
 * Disciplina: AED's III
 * Professor.: Iago Augusto de Carvalho
 * Alunos.....: Denis Mendes Coutinho, Gabriel Micael Henrique, Giovana Nogueira, Rodrigo Cabral Diniz, Vinicius Barbosa da Silveira
 * Data......: 04/05/2023
 *----------------------------------------------------------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef struct
{
    unsigned int peso;
    int beneficio;
} type_item;

void forca_bruta(type_item *items, int n_items, int capacidad)
{
    clock_t start_time = clock();
    int max_beneficio = 0;
    unsigned int max_peso = 0;
    int *max_combinacao = (int *)calloc(n_items, sizeof(int));
    int i, j, max = 1 << n_items;
    for (i = 0; i < max; i++)
    {
        unsigned int peso = 0;
        int beneficio = 0;
        for (j = 0; j < n_items; j++)
        {
            if ((i >> j) & 1)
            {
                peso += items[j].peso;
                beneficio += items[j].beneficio;
            }
        }
        if (peso <= capacidad && beneficio > max_beneficio)
        {
            max_beneficio = beneficio;
            max_peso = peso;
            for (j = 0; j < n_items; j++)
            {
                max_combinacao[j] = (i >> j) & 1;
            }
        }
    }
    printf("\nForca bruta:\n");
    printf("Beneficio maximo: %d, Peso total: %u\n", max_beneficio, max_peso);
    printf("Itens escolhidos: ");
    for (i = 0; i < n_items; i++)
    {
        if (max_combinacao[i])
        {
            printf("%d ", i + 1);
        }
    }
    printf("\nTempo de execucao: %f segundos\n\n", ((double)(clock() - start_time)) / CLOCKS_PER_SEC);
    free(max_combinacao);
}

void guloso(type_item *items, int n_items, int capacidad)
{
    int i, j;
    float *coeficientes = (float *)calloc(n_items, sizeof(float));
    int *escolhidos = (int *)calloc(n_items, sizeof(int));
    for (i = 0; i < n_items; i++)
    {
        coeficientes[i] = (float)items[i].beneficio / (float)items[i].peso;
    }
    clock_t inicio = clock();
    for (i = 0; i < n_items; i++)
    {
        int max_index = 0;
        for (j = 1; j < n_items; j++)
        {
            if (coeficientes[j] > coeficientes[max_index])
            {
                max_index = j;
            }
        }
        escolhidos[max_index] = 1;
        coeficientes[max_index] = -1.0;
        capacidad -= items[max_index].peso;
        if (capacidad < 0)
        {
            break;
        }
    }
    clock_t fim = clock();
    double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    int beneficio = 0;
    unsigned int peso = 0;
    printf("\nGuloso:\n");
    printf("Itens escolhidos: ");
    for (i = 0; i < n_items; i++)
    {
        if (escolhidos[i])
        {
            printf("%d ", i + 1);
            beneficio += items[i].beneficio;
            peso += items[i].peso;
        }
    }
    printf("\nBeneficio maximo: %d, Peso total: %u\n", beneficio, peso);
    printf("Tempo de execucao: %.4lf segundos\n\n", tempo_gasto);
    free(coeficientes);
    free(escolhidos);
}

int comparar_itens(const void *a, const void *b)
{
    type_item *item_a = (type_item *)a;
    type_item *item_b = (type_item *)b;

    double razao_a = (double)item_a->beneficio / (double)item_a->peso;
    double razao_b = (double)item_b->beneficio / (double)item_b->peso;

    if (razao_a < razao_b)
    {
        return 1;
    }
    else if (razao_a > razao_b)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int melhor_combinacao(type_item *items, int n_items, int capacidad)
{
    if (n_items == 0 || capacidad == 0)
    {
        return 0;
    }

    if (items[0].peso > capacidad)
    {
        return melhor_combinacao(items + 1, n_items - 1, capacidad);
    }

    int com_primeiro = items[0].beneficio + melhor_combinacao(items + 1, n_items - 1, capacidad - items[0].peso);
    int sem_primeiro = melhor_combinacao(items + 1, n_items - 1, capacidad);

    return (com_primeiro > sem_primeiro) ? com_primeiro : sem_primeiro;
}

void divisao_conquista(type_item *items, int n_items, int capacidad)
{
    clock_t inicio, fim;
    inicio = clock();

    qsort(items, n_items, sizeof(type_item), comparar_itens);
    int melhor_beneficio = melhor_combinacao(items, n_items, capacidad);

    fim = clock();
    double tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\nMelhor beneficio usando divisao e conquista: %d\n", melhor_beneficio);
    printf("Tempo de execucao: %.6f segundos\n\n", tempo_execucao);
}

void imprimir_items(type_item *items, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("Peso: %d, Beneficio: %d\n", items[i].peso, items[i].beneficio);
    }
}

type_item *ler_items(char *filename, int *n_items, int *capacidad)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        puts("Não foi possível abrir o arquivo");
        exit(0);
    }

    fscanf(fp, "%d %d", n_items, capacidad);
    printf("\nNumero de elementos: %d, Capacidade: %d\n", *n_items, *capacidad);

    type_item *items = (type_item *)malloc(*n_items * sizeof(type_item));

    for (int i = 0; i < *n_items; i++)
    {
        fscanf(fp, "%d %d", &items[i].peso, &items[i].beneficio);
    }

    fclose(fp);

    return items;
}

int main(int argc, char *argv[])
{
    int n_items, capacidad;

    if (argc != 3)
    {
        printf("Uso: %s <nome do arquivo de entrada> <algoritmo>\n", argv[0]);
        return 1;
    }

    type_item *items = ler_items(argv[1], &n_items, &capacidad);
    imprimir_items(items, n_items);

    if (argv[2][0] == '1')
    {
        forca_bruta(items, n_items, capacidad);
    }
    else if (argv[2][0] == '2')
    {
        guloso(items, n_items, capacidad);
    }
    else if (argv[2][0] == '3')
    {
        divisao_conquista(items, n_items, capacidad);
    }
    else
    {
        printf("\nEscolha um algoritmo entre 1 e 3\n\n");
        return 1;
    }

    free(items);
    return 0;
}