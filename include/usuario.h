#ifndef USUARIO_H
#define USUARIO_H

#define MAX_CURSO 50
#define MAX_NOME 100
#define CAPACIDADE_INICIAL_USUARIOS 4

typedef struct {
    int matricula;
    char nome[MAX_NOME];
    char curso[MAX_CURSO];
} USUARIO;

void carregar_usuarios(USUARIO **usuarios, int *total, int *capacidade);
void salvar_usuarios(USUARIO *usuarios, int total);
void cadastrar_usuario(USUARIO **usuarios, int *total, int *capacidade);
void listar_usuarios(USUARIO *usuarios, int total);
void buscar_usuario_matricula(USUARIO *usuarios, int total);
void buscar_usuario_binaria_matricula(USUARIO *usuarios, int total);

#endif
