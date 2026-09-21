#ifndef LIVROS_H
#define LIVROS_H

#define MAX_TITULOS 150
#define MAX_AUTOR 100

typedef struct{
    int codigo;
    char titulo[MAX_TITULOS];
    char autor[MAX_AUTOR];
    int ano;
    int quantidade;
}LIVRO;

#endif