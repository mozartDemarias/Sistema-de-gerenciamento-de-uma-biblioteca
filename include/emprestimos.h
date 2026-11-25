#ifndef EMPRESTIMOS_H
#define EMPRESTIMOS_H

#include "livros.h"
#include "usuario.h"

#define CAPACIDADE_INICIAL_EMPRESTIMOS 4

typedef struct {
    int id;
    int matricula_usuario;
    int codigo_livro;
    int ativo;
} EMPRESTIMO;

void carregar_emprestimos(EMPRESTIMO **emprestimos, int *total, int *capacidade);
void salvar_emprestimos(EMPRESTIMO *emprestimos, int total);
int carregar_emprestimos_bin(EMPRESTIMO **emprestimos, int *total, int *capacidade);
void salvar_emprestimos_bin(EMPRESTIMO *emprestimos, int total);
void registrar_emprestimo(EMPRESTIMO **emprestimos, int *total_emp, int *capacidade_emp, LIVRO *livros, int total_liv, USUARIO *usuarios, int total_usu);
void registrar_devolucao(EMPRESTIMO *emprestimos, int total_emp, LIVRO *livros, int total_liv);
void listar_emprestimos(EMPRESTIMO *emprestimos, int total_emp);

#endif
