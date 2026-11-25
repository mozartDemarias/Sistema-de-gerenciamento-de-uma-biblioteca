/*
 * livros.c
 * Gerenciamento dinamico do acervo de livros: alocacao com malloc/realloc,
 * persistencia em CSV e binario (.bin), listagem, busca sequencial e pesquisa binaria.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/livros.h"
#include "../include/ordenacao.h"
#include "../include/utils.h"

/*
 * Garante espaco no vetor dinamico de livros.
 * Dobra a capacidade via realloc usando ponteiro temporario para preservar o bloco original em caso de falha.
 */
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

/* Carrega os livros de livros.csv expandindo o vetor conforme necessario. */
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

        /* Ignora a primeira linha caso seja o cabecalho textual do CSV. */
        if (*total == 0 && (linha[0] < '0' || linha[0] > '9')) continue;

        LIVRO temp = {0};
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

/*
 * Carrega livros de livros.bin no formato [int total][total * sizeof(LIVRO)].
 * Aloca exatamente a capacidade necessaria com malloc e libera o ponteiro anterior apenas apos sucesso.
 */
int carregar_livros_bin(LIVRO **livros, int *total, int *capacidade) {
    FILE *arq = fopen("../data/livros.bin", "rb");
    if (arq == NULL) {
        arq = fopen("data/livros.bin", "rb");
        if (arq == NULL) {
            return 0;
        }
    }

    int qtd = 0;
    if (fread(&qtd, sizeof(int), 1, arq) != 1 || qtd < 0) {
        fclose(arq);
        return 0;
    }

    int nova_cap = (qtd > 0) ? qtd : CAPACIDADE_INICIAL_LIVROS;
    LIVRO *novo = (LIVRO *)malloc((size_t)nova_cap * sizeof(LIVRO));
    if (novo == NULL) {
        printf("Erro critico: Falha ao alocar memoria ao carregar livros.bin!\n");
        fclose(arq);
        return 0;
    }

    if (qtd > 0) {
        if (fread(novo, sizeof(LIVRO), (size_t)qtd, arq) != (size_t)qtd) {
            free(novo);
            fclose(arq);
            return 0;
        }
    }

    fclose(arq);
    free(*livros);
    *livros = novo;
    *total = qtd;
    *capacidade = nova_cap;
    return 1;
}

/* Persiste o inteiro 'total' seguido do bloco contiguo de structs LIVRO em livros.bin. */
void salvar_livros_bin(LIVRO *livros, int total) {
    FILE *arq = fopen("../data/livros.bin", "wb");
    if (arq == NULL) {
        arq = fopen("data/livros.bin", "wb");
        if (arq == NULL) {
            printf("Erro ao salvar livros.bin!\n");
            return;
        }
    }

    if (fwrite(&total, sizeof(int), 1, arq) != 1) {
        printf("Erro ao gravar cabecalho de livros.bin!\n");
        fclose(arq);
        return;
    }

    if (total > 0 && livros != NULL) {
        if (fwrite(livros, sizeof(LIVRO), (size_t)total, arq) != (size_t)total) {
            printf("Erro ao gravar registros em livros.bin!\n");
        }
    }

    fclose(arq);
}

void cadastrar_livro(LIVRO **livros, int *total, int *capacidade) {
    LIVRO novo = {0};
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
    salvar_livros_bin(*livros, *total);
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

/* Pesquisa binaria manual O(log n) por codigo apos ordenacao previa do acervo. */
void buscar_livro_binaria_codigo(LIVRO *livros, int total) {
    if (livros == NULL || total <= 0) {
        printf("Nenhum livro cadastrado.\n");
        return;
    }

    int codigo;
    printf("Digite o codigo do livro para busca binaria:\n");
    if (scanf("%d", &codigo) != 1) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        printf("Erro: Codigo invalido.\n");
        return;
    }

    /* Garante que o acervo esteja ordenado por codigo antes da pesquisa binaria. */
    insertion_sort_livros_codigo(livros, total);

    int inicio = 0;
    int fim = total - 1;
    int comparacoes = 0;

    while (inicio <= fim) {
        /* Calcula o ponto medio evitando possivel overflow em (inicio + fim). */
        int meio = inicio + (fim - inicio) / 2;
        comparacoes++;

        if (livros[meio].codigo == codigo) {
            printf("Encontrado (%d comparacoes) -> Cod: %d | Titulo: %s | Autor: %s | Ano: %d | Qtd: %d\n",
                   comparacoes, livros[meio].codigo, livros[meio].titulo, livros[meio].autor,
                   livros[meio].ano, livros[meio].quantidade);
            return;
        }
        if (livros[meio].codigo < codigo) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    printf("Erro: Livro com codigo %d nao encontrado.\n", codigo);
}
