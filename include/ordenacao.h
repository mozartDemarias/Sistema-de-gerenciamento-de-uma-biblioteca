/*
 * ordenacao.h
 * Algoritmos manuais de ordenacao (Bubble Sort e Insertion Sort)
 * para livros e usuarios, sem uso de funcoes prontas como qsort.
 */
#ifndef ORDENACAO_H
#define ORDENACAO_H

#include "livros.h"
#include "usuario.h"

/* Ordena o vetor de livros em ordem crescente de codigo usando Bubble Sort. */
void bubble_sort_livros_codigo(LIVRO *livros, int total);

/* Ordena o vetor de livros em ordem alfabetica de titulo usando Bubble Sort. */
void bubble_sort_livros_titulo(LIVRO *livros, int total);

/* Ordena o vetor de livros em ordem crescente de codigo usando Insertion Sort. */
void insertion_sort_livros_codigo(LIVRO *livros, int total);

/* Ordena o vetor de livros em ordem alfabetica de titulo usando Insertion Sort. */
void insertion_sort_livros_titulo(LIVRO *livros, int total);

/* Ordena o vetor de usuarios em ordem crescente de matricula usando Insertion Sort. */
void insertion_sort_usuarios_matricula(USUARIO *usuarios, int total);

#endif
