/*
 * livros.h
 * Definicao da estrutura LIVRO e operacoes de persistencia (CSV/binario),
 * cadastro dinamico, listagem e buscas (sequencial e binaria).
 */
#ifndef LIVROS_H
#define LIVROS_H

#define MAX_TITULOS 150
#define MAX_AUTOR 100
#define CAPACIDADE_INICIAL_LIVROS 4

/* Representa um titulo cadastrado no acervo da biblioteca. */
typedef struct {
    int codigo;
    char titulo[MAX_TITULOS];
    char autor[MAX_AUTOR];
    int ano;
    int quantidade;
} LIVRO;

/* Carrega livros de data/livros.csv expandindo o vetor dinamicamente via realloc. */
void carregar_livros(LIVRO **livros, int *total, int *capacidade);

/* Salva todos os livros do vetor no arquivo texto data/livros.csv. */
void salvar_livros(LIVRO *livros, int total);

/* Carrega livros de data/livros.bin alocando memoria conforme o total gravado. Retorna 1 se sucesso, 0 caso contrario. */
int carregar_livros_bin(LIVRO **livros, int *total, int *capacidade);

/* Grava o inteiro 'total' seguido do bloco de structs LIVRO em data/livros.bin. */
void salvar_livros_bin(LIVRO *livros, int total);

/* Le os dados de um novo livro via stdin, garante capacidade dinamica e persiste em CSV e binario. */
void cadastrar_livro(LIVRO **livros, int *total, int *capacidade);

/* Exibe todos os livros cadastrados no terminal. */
void listar_livros(LIVRO *livros, int total);

/* Busca sequencial manual de livros por trecho do titulo. */
void buscar_livro_titulo(LIVRO *livros, int total);

/* Busca sequencial manual de livros por trecho do nome do autor. */
void buscar_livro_autor(LIVRO *livros, int total);

/* Ordena o acervo por codigo e executa pesquisa binaria manual exibindo o resultado e o numero de comparacoes. */
void buscar_livro_binaria_codigo(LIVRO *livros, int total);

#endif
