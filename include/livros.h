#ifndef LIVROS_H
#define LIVROS_H

#define MAX_TITULOS 150
#define MAX_AUTOR 100
#define MAX_LIVROS 200

typedef struct {
    int codigo;
    char titulo[MAX_TITULOS];
    char autor[MAX_AUTOR];
    int ano;
    int quantidade;
} LIVRO;

void carregar_livros(LIVRO livros[], int *total);
void salvar_livros(LIVRO livros[], int total);
void cadastrar_livro(LIVRO livros[], int *total);
void listar_livros(LIVRO livros[], int total);
void buscar_livro_titulo(LIVRO livros[], int total);
void buscar_livro_autor(LIVRO livros[], int total);

#endif