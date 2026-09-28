#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/usuario.h"
#include "../include/utils.h"

void carregar_usuarios(USUARIO usuarios[], int *total) {
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

        char *token = strtok(linha, ";");
        if (!token) continue;
        usuarios[*total].matricula = atoi(token);

        token = strtok(NULL, ";");
        if (!token) continue;
        strcpy(usuarios[*total].nome, token);

        token = strtok(NULL, ";");
        if (!token) continue;
        strcpy(usuarios[*total].curso, token);

        (*total)++;
    }
    fclose(arq);
}

void salvar_usuarios(USUARIO usuarios[], int total) {
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

void cadastrar_usuario(USUARIO usuarios[], int *total) {
    if (*total >= MAX_USUARIOS) {
        printf("Erro: Limite de usuarios atingido!\n");
        return;
    }

    USUARIO novo;
    printf("Digite o numero de matricula:\n");
    scanf("%d", &novo.matricula);
    getchar();

    for (int i = 0; i < *total; i++) {
        if (usuarios[i].matricula == novo.matricula) {
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

    usuarios[*total] = novo;
    (*total)++;
    salvar_usuarios(usuarios, *total);
    printf("Usuario cadastrado com sucesso!\n");
}

void listar_usuarios(USUARIO usuarios[], int total) {
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

void buscar_usuario_matricula(USUARIO usuarios[], int total) {
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