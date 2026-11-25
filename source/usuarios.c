/*
 * usuarios.c
 * Gerenciamento dinamico de usuarios: alocacao com malloc/realloc,
 * persistencia em CSV e binario (.bin), listagem, busca sequencial e pesquisa binaria.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/usuario.h"
#include "../include/ordenacao.h"
#include "../include/utils.h"

/*
 * Garante espaco no vetor dinamico de usuarios.
 * Dobra a capacidade via realloc usando ponteiro temporario e verifica NULL antes de atualizar.
 */
static int garantir_capacidade_usuarios(USUARIO **usuarios, int total, int *capacidade) {
    if (*usuarios == NULL || *capacidade <= 0) {
        int cap_inicial = CAPACIDADE_INICIAL_USUARIOS;
        USUARIO *novo = (USUARIO *)malloc((size_t)cap_inicial * sizeof(USUARIO));
        if (novo == NULL) {
            printf("Erro critico: Falha ao alocar memoria para usuarios!\n");
            return 0;
        }
        *usuarios = novo;
        *capacidade = cap_inicial;
    }

    if (total >= *capacidade) {
        int nova_cap = (*capacidade) * 2;
        USUARIO *temp = (USUARIO *)realloc(*usuarios, (size_t)nova_cap * sizeof(USUARIO));
        if (temp == NULL) {
            printf("Erro critico: Falha ao realocar memoria para usuarios!\n");
            return 0;
        }
        *usuarios = temp;
        *capacidade = nova_cap;
    }

    return 1;
}

/* Carrega os usuarios de usuarios.csv expandindo o vetor conforme necessario. */
void carregar_usuarios(USUARIO **usuarios, int *total, int *capacidade) {
    FILE *arq = fopen("../data/usuarios.csv", "r");
    *total = 0;
    if (arq == NULL) {
        arq = fopen("data/usuarios.csv", "r");
        if (arq == NULL) return;
    }

    char linha[512];
    while (fgets(linha, sizeof(linha), arq) != NULL) {
        limpar_enter(linha);
        if (linha[0] == '\0') continue;
        if (*total == 0 && (linha[0] < '0' || linha[0] > '9')) continue;

        USUARIO temp = {0};
        char *token = strtok(linha, ";");
        if (!token) continue;
        temp.matricula = atoi(token);

        token = strtok(NULL, ";");
        if (!token) continue;
        strncpy(temp.nome, token, MAX_NOME - 1);
        temp.nome[MAX_NOME - 1] = '\0';

        token = strtok(NULL, ";");
        if (!token) continue;
        strncpy(temp.curso, token, MAX_CURSO - 1);
        temp.curso[MAX_CURSO - 1] = '\0';

        if (!garantir_capacidade_usuarios(usuarios, *total, capacidade)) {
            break;
        }

        (*usuarios)[*total] = temp;
        (*total)++;
    }
    fclose(arq);
}

void salvar_usuarios(USUARIO *usuarios, int total) {
    FILE *arq = fopen("../data/usuarios.csv", "w");
    if (arq == NULL) {
        arq = fopen("data/usuarios.csv", "w");
        if (arq == NULL) {
            printf("Erro ao salvar usuarios.csv!\n");
            return;
        }
    }

    fprintf(arq, "matricula;nome;curso\n");
    for (int i = 0; i < total; i++) {
        fprintf(arq, "%d;%s;%s\n",
                usuarios[i].matricula,
                usuarios[i].nome,
                usuarios[i].curso);
    }
    fclose(arq);
}

/*
 * Carrega usuarios de usuarios.bin no formato [int total][total * sizeof(USUARIO)].
 * Aloca dinamicamente com malloc e substitui o ponteiro anterior somente apos leitura completa.
 */
int carregar_usuarios_bin(USUARIO **usuarios, int *total, int *capacidade) {
    FILE *arq = fopen("../data/usuarios.bin", "rb");
    if (arq == NULL) {
        arq = fopen("data/usuarios.bin", "rb");
        if (arq == NULL) {
            return 0;
        }
    }

    int qtd = 0;
    if (fread(&qtd, sizeof(int), 1, arq) != 1 || qtd < 0) {
        fclose(arq);
        return 0;
    }

    int nova_cap = (qtd > 0) ? qtd : CAPACIDADE_INICIAL_USUARIOS;
    USUARIO *novo = (USUARIO *)malloc((size_t)nova_cap * sizeof(USUARIO));
    if (novo == NULL) {
        printf("Erro critico: Falha ao alocar memoria ao carregar usuarios.bin!\n");
        fclose(arq);
        return 0;
    }

    if (qtd > 0) {
        if (fread(novo, sizeof(USUARIO), (size_t)qtd, arq) != (size_t)qtd) {
            free(novo);
            fclose(arq);
            return 0;
        }
    }

    fclose(arq);
    free(*usuarios);
    *usuarios = novo;
    *total = qtd;
    *capacidade = nova_cap;
    return 1;
}

/* Persiste o inteiro 'total' seguido do bloco contiguo de structs USUARIO em usuarios.bin. */
void salvar_usuarios_bin(USUARIO *usuarios, int total) {
    FILE *arq = fopen("../data/usuarios.bin", "wb");
    if (arq == NULL) {
        arq = fopen("data/usuarios.bin", "wb");
        if (arq == NULL) {
            printf("Erro ao salvar usuarios.bin!\n");
            return;
        }
    }

    if (fwrite(&total, sizeof(int), 1, arq) != 1) {
        printf("Erro ao gravar cabecalho de usuarios.bin!\n");
        fclose(arq);
        return;
    }

    if (total > 0 && usuarios != NULL) {
        if (fwrite(usuarios, sizeof(USUARIO), (size_t)total, arq) != (size_t)total) {
            printf("Erro ao gravar registros em usuarios.bin!\n");
        }
    }

    fclose(arq);
}

void cadastrar_usuario(USUARIO **usuarios, int *total, int *capacidade) {
    USUARIO novo = {0};
    printf("Digite o numero de matricula:\n");
    scanf("%d", &novo.matricula);
    getchar();

    for (int i = 0; i < *total; i++) {
        if ((*usuarios)[i].matricula == novo.matricula) {
            printf("Erro: Matricula ja cadastrada!\n");
            return;
        }
    }

    printf("Digite o nome de usuario:\n");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    limpar_enter(novo.nome);

    printf("Digite o curso:\n");
    fgets(novo.curso, sizeof(novo.curso), stdin);
    limpar_enter(novo.curso);

    if (!garantir_capacidade_usuarios(usuarios, *total, capacidade)) {
        return;
    }

    (*usuarios)[*total] = novo;
    (*total)++;
    salvar_usuarios(*usuarios, *total);
    salvar_usuarios_bin(*usuarios, *total);
    printf("Usuario cadastrado com sucesso!\n");
}

void listar_usuarios(USUARIO *usuarios, int total) {
    if (total == 0) {
        printf("Nenhum usuario cadastrado.\n");
        return;
    }
    printf("\n--- LISTA DE USUARIOS ---\n");
    for (int i = 0; i < total; i++) {
        printf("Matricula: %d | Nome: %s | Curso: %s\n",
               usuarios[i].matricula, usuarios[i].nome, usuarios[i].curso);
    }
}

void buscar_usuario_matricula(USUARIO *usuarios, int total) {
    int mat;
    printf("Digite a matricula para busca:\n");
    scanf("%d", &mat);

    for (int i = 0; i < total; i++) {
        if (usuarios[i].matricula == mat) {
            printf("Encontrado -> Matricula: %d | Nome: %s | Curso: %s\n",
                   usuarios[i].matricula, usuarios[i].nome, usuarios[i].curso);
            return;
        }
    }
    printf("Erro: Usuario com matricula %d nao encontrado.\n", mat);
}

/* Pesquisa binaria manual O(log n) por matricula apos ordenacao previa dos usuarios. */
void buscar_usuario_binaria_matricula(USUARIO *usuarios, int total) {
    if (usuarios == NULL || total <= 0) {
        printf("Nenhum usuario cadastrado.\n");
        return;
    }

    int mat;
    printf("Digite a matricula para busca binaria:\n");
    if (scanf("%d", &mat) != 1) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        printf("Erro: Matricula invalida.\n");
        return;
    }

    /* Garante que o vetor esteja ordenado por matricula antes da pesquisa binaria. */
    insertion_sort_usuarios_matricula(usuarios, total);

    int inicio = 0;
    int fim = total - 1;
    int comparacoes = 0;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        comparacoes++;

        if (usuarios[meio].matricula == mat) {
            printf("Encontrado (%d comparacoes) -> Matricula: %d | Nome: %s | Curso: %s\n",
                   comparacoes, usuarios[meio].matricula, usuarios[meio].nome, usuarios[meio].curso);
            return;
        }
        if (usuarios[meio].matricula < mat) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    printf("Erro: Usuario com matricula %d nao encontrado.\n", mat);
}
