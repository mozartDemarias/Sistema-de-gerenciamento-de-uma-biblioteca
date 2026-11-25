/*
 * usuario.h
 * Definicao da estrutura USUARIO e operacoes de persistencia (CSV/binario),
 * cadastro dinamico, listagem e buscas por matricula (sequencial e binaria).
 */
#ifndef USUARIO_H
#define USUARIO_H

#define MAX_CURSO 50
#define MAX_NOME 100
#define CAPACIDADE_INICIAL_USUARIOS 4

/* Representa um usuario cadastrado na biblioteca universitaria. */
typedef struct {
    int matricula;
    char nome[MAX_NOME];
    char curso[MAX_CURSO];
} USUARIO;

/* Carrega usuarios de data/usuarios.csv expandindo o vetor dinamicamente via realloc. */
void carregar_usuarios(USUARIO **usuarios, int *total, int *capacidade);

/* Salva todos os usuarios do vetor no arquivo texto data/usuarios.csv. */
void salvar_usuarios(USUARIO *usuarios, int total);

/* Carrega usuarios de data/usuarios.bin alocando memoria conforme o total gravado. Retorna 1 se sucesso, 0 caso contrario. */
int carregar_usuarios_bin(USUARIO **usuarios, int *total, int *capacidade);

/* Grava o inteiro 'total' seguido do bloco de structs USUARIO em data/usuarios.bin. */
void salvar_usuarios_bin(USUARIO *usuarios, int total);

/* Le os dados de um novo usuario via stdin, valida duplicidade, garante capacidade e persiste em CSV e binario. */
void cadastrar_usuario(USUARIO **usuarios, int *total, int *capacidade);

/* Exibe todos os usuarios cadastrados no terminal. */
void listar_usuarios(USUARIO *usuarios, int total);

/* Busca sequencial manual de usuario pelo numero de matricula. */
void buscar_usuario_matricula(USUARIO *usuarios, int total);

/* Ordena os usuarios por matricula e executa pesquisa binaria manual exibindo o resultado e o numero de comparacoes. */
void buscar_usuario_binaria_matricula(USUARIO *usuarios, int total);

#endif
