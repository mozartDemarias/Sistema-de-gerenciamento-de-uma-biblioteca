/*
 * emprestimos.h
 * Definicao da estrutura EMPRESTIMO e operacoes de persistencia (CSV/binario),
 * registro de emprestimos/devolucoes, listagem e historico por usuario.
 */
#ifndef EMPRESTIMOS_H
#define EMPRESTIMOS_H

#include "livros.h"
#include "usuario.h"

#define CAPACIDADE_INICIAL_EMPRESTIMOS 4

/* Representa um registro de emprestimo (ativo = 1) ou devolvido (ativo = 0). */
typedef struct {
    int id;
    int matricula_usuario;
    int codigo_livro;
    int ativo;
} EMPRESTIMO;

/* Carrega emprestimos de data/emprestimos.csv expandindo o vetor dinamicamente via realloc. */
void carregar_emprestimos(EMPRESTIMO **emprestimos, int *total, int *capacidade);

/* Salva todos os emprestimos do vetor no arquivo texto data/emprestimos.csv. */
void salvar_emprestimos(EMPRESTIMO *emprestimos, int total);

/* Carrega emprestimos de data/emprestimos.bin alocando memoria conforme o total gravado. Retorna 1 se sucesso, 0 caso contrario. */
int carregar_emprestimos_bin(EMPRESTIMO **emprestimos, int *total, int *capacidade);

/* Grava o inteiro 'total' seguido do bloco de structs EMPRESTIMO em data/emprestimos.bin. */
void salvar_emprestimos_bin(EMPRESTIMO *emprestimos, int total);

/* Registra um novo emprestimo validando usuario, livro, estoque e persistindo emprestimos e livros em CSV/binario. */
void registrar_emprestimo(EMPRESTIMO **emprestimos, int *total_emp, int *capacidade_emp, LIVRO *livros, int total_liv, USUARIO *usuarios, int total_usu);

/* Registra a devolucao de um emprestimo ativo por ID, restaurando o estoque do livro e salvando CSV/binario. */
void registrar_devolucao(EMPRESTIMO *emprestimos, int total_emp, LIVRO *livros, int total_liv);

/* Exibe todos os emprestimos registrados com seu status atual. */
void listar_emprestimos(EMPRESTIMO *emprestimos, int total_emp);

/* Lista todos os emprestimos (ativos e devolvidos) de um usuario, exibindo o titulo de cada livro emprestado. */
void exibir_historico_usuario(EMPRESTIMO *emprestimos, int total_emp,
                              LIVRO *livros, int total_liv,
                              USUARIO *usuarios, int total_usu);

#endif
