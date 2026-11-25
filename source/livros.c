#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/livros.h"
#include "../include/utils.h"

/* Garante espaco no vetor dinamico de livros, dobrando a capacidade quando necessario. */
static int garantir_capacidade_livros(LIVRO **livros, int total, int *capacidade) {
    if (*livros == NULL || *capacidade <= 0) {
        int cap_inicial = CAPACIDADE_INICIAL_LIVROS;
        LIVRO *novo = (LIVRO *)malloc((size_t)cap_inicial * sizeof(LIVRO));
        if (novo == NULL) {
            printf("Erro critico: Falha ao alocar memoria para livros!\n");
            return 0;
        }
        *livros = novo;
        *capacidade = cap_inicial;
    }

    if (total >= *capacidade) {
        int nova_cap = (*capacidade) * 2;
        LIVRO *temp = (LIVRO *)realloc(*livros, (size_t)nova_cap * sizeof(LIVRO));
        if (temp == NULL) {
            printf("Erro critico: Falha ao realocar memoria para livros!\n");
            return 0;
        }
        *livros = temp;
        *capacidade = nova_cap;
    }

    return 1;
}

void carregar_livros(LIVRO **livros, int *total, int *capacidade) {
    FILE *arq = fopen("../data/livros.csv", "r");
    *total = 0;
    if (arq == NULL) {
        arq = fopen("data/livros.csv", "r");
        if (arq == NULL) return;
    }

    char linha[512];
    while (fgets(linha, sizeof(linha), arq) != NULL) {
        limpar_enter(linha);
        if (linha[0] == '\0') continue;

        if (*total == 0 && (linha[0] < '0' || linha[0] > '9')) continue;

        LIVRO temp;
        char *token = strtok(linha, ";");
        if (!token) continue;
        temp.codigo = atoi(token);

        token = strtok(NULL, ";");
        if (!token) continue;
        strncpy(temp.titulo, token, MAX_TITULOS - 1);
        temp.titulo[MAX_TITULOS - 1] = '\0';

        token = strtok(NULL, ";");
        if (!token) continue;
        strncpy(temp.autor, token, MAX_AUTOR - 1);
        temp.autor[MAX_AUTOR - 1] = '\0';

        token = strtok(NULL, ";");
        if (!token) continue;
        temp.ano = atoi(token);

        token = strtok(NULL, ";");
        if (!token) continue;
        temp.quantidade = atoi(token);

        if (!garantir_capacidade_livros(livros, *total, capacidade)) {
            break;
        }

        (*livros)[*total] = temp;
        (*total)++;
    }
    fclose(arq);
}

void salvar_livros(LIVRO *livros, int total) {
    FILE *arq = fopen("../data/livros.csv", "w");
    if (arq == NULL) {
        arq = fopen("data/livros.csv", "w");
        if (arq == NULL) {
            printf("Erro ao salvar livros.csv!\n");
            return;
        }
    }

    fprintf(arq, "codigo;titulo;autor;ano;quantidade\n");
    for (int i = 0; i < total; i++) {
        fprintf(arq, "%d;%s;%s;%d;%d\n",
                livros[i].codigo,
                livros[i].titulo,
                livros[i].autor,
                livros[i].ano,
                livros[i].quantidade);
    }
    fclose(arq);
}

void cadastrar_livro(LIVRO **livros, int *total, int *capacidade) {
    LIVRO novo;
    printf("Digite o codigo do livro:\n");
    scanf("%d", &novo.codigo);
    getchar();

    for (int i = 0; i < *total; i++) {
        if ((*livros)[i].codigo == novo.codigo) {
            printf("Erro: Ja existe um livro com este codigo!\n");
            return;
        }
    }

    printf("Digite o titulo do livro:\n");
    fgets(novo.titulo, sizeof(novo.titulo), stdin);
    limpar_enter(novo.titulo);

    printf("Digite o nome do autor(a) do livro:\n");
    fgets(novo.autor, sizeof(novo.autor), stdin);
    limpar_enter(novo.autor);

    printf("Digite o ano do livro:\n");
    scanf("%d", &novo.ano);

    printf("Digite a quantidade:\n");
    scanf("%d", &novo.quantidade);

    if (!garantir_capacidade_livros(livros, *total, capacidade)) {
        return;
    }

    (*livros)[*total] = novo;
    (*total)++;
    salvar_livros(*livros, *total);
    printf("Livro cadastrado com sucesso!\n");
}

void listar_livros(LIVRO *livros, int total) {
    if (total == 0) {
        printf("Nenhum livro cadastrado.\n");
        return;
    }
    printf("\n--- LISTA DE LIVROS ---\n");
    for (int i = 0; i < total; i++) {
        printf("Cod: %d | Titulo: %s | Autor: %s | Ano: %d | Qtd: %d\n",
               livros[i].codigo, livros[i].titulo, livros[i].autor, livros[i].ano, livros[i].quantidade);
    }
}

void buscar_livro_titulo(LIVRO *livros, int total) {
    char termo[MAX_TITULOS];
    getchar();
    printf("Digite o titulo para busca:\n");
    fgets(termo, sizeof(termo), stdin);
    limpar_enter(termo);

    int encontrou = 0;
    for (int i = 0; i < total; i++) {
        if (contem_texto(livros[i].titulo, termo)) {
            printf("Cod: %d | Titulo: %s | Autor: %s | Ano: %d | Qtd: %d\n",
                   livros[i].codigo, livros[i].titulo, livros[i].autor, livros[i].ano, livros[i].quantidade);
            encontrou = 1;
        }
    }
    if (!encontrou) {
        printf("Erro: Livro nao encontrado.\n");
    }
}

void buscar_livro_autor(LIVRO *livros, int total) {
    char termo[MAX_AUTOR];
    getchar();
    printf("Digite o autor para busca:\n");
    fgets(termo, sizeof(termo), stdin);
    limpar_enter(termo);

    int encontrou = 0;
    for (int i = 0; i < total; i++) {
        if (contem_texto(livros[i].autor, termo)) {
            printf("Cod: %d | Titulo: %s | Autor: %s | Ano: %d | Qtd: %d\n",
                   livros[i].codigo, livros[i].titulo, livros[i].autor, livros[i].ano, livros[i].quantidade);
            encontrou = 1;
        }
    }
    if (!encontrou) {
        printf("Erro: Nenhum livro deste autor foi encontrado.\n");
    }
}
