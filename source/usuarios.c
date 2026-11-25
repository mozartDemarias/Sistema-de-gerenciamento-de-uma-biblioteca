#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/usuario.h"
#include "../include/utils.h"

/* Garante espaco no vetor dinamico de usuarios, dobrando a capacidade quando necessario. */
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

        USUARIO temp;
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

void cadastrar_usuario(USUARIO **usuarios, int *total, int *capacidade) {
    USUARIO novo;
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
