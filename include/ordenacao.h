#ifndef ORDENACAO_H
#define ORDENACAO_H

#include "livros.h"
#include "usuario.h"

void bubble_sort_livros_codigo(LIVRO *livros, int total);
void bubble_sort_livros_titulo(LIVRO *livros, int total);
void insertion_sort_livros_codigo(LIVRO *livros, int total);
void insertion_sort_livros_titulo(LIVRO *livros, int total);
void insertion_sort_usuarios_matricula(USUARIO *usuarios, int total);

#endif
