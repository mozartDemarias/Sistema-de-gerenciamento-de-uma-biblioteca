#include <stddef.h>
#include "../include/ordenacao.h"

/* Converte letras ASCII maiusculas para minusculas para comparacao alfabetica. */
static char para_minuscula_ascii(char c) {
    if (c >= 'A' && c <= 'Z') {
        return (char)(c + 32);
    }
    return c;
}

/* Compara duas strings manualmente sem diferenciar maiusculas/minusculas ASCII. */
static int comparar_titulos(const char *a, const char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        unsigned char ca = (unsigned char)para_minuscula_ascii(a[i]);
        unsigned char cb = (unsigned char)para_minuscula_ascii(b[i]);
        if (ca != cb) {
            return (int)ca - (int)cb;
        }
        i++;
    }
    return (int)(unsigned char)a[i] - (int)(unsigned char)b[i];
}

void bubble_sort_livros_codigo(LIVRO *livros, int total) {
    if (livros == NULL || total <= 1) {
        return;
    }

    for (int i = 0; i < total - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < total - 1 - i; j++) {
            if (livros[j].codigo > livros[j + 1].codigo) {
                LIVRO temp = livros[j];
                livros[j] = livros[j + 1];
                livros[j + 1] = temp;
                trocou = 1;
            }
        }
        if (!trocou) {
            break;
        }
    }
}

void bubble_sort_livros_titulo(LIVRO *livros, int total) {
    if (livros == NULL || total <= 1) {
        return;
    }

    for (int i = 0; i < total - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < total - 1 - i; j++) {
            if (comparar_titulos(livros[j].titulo, livros[j + 1].titulo) > 0) {
                LIVRO temp = livros[j];
                livros[j] = livros[j + 1];
                livros[j + 1] = temp;
                trocou = 1;
            }
        }
        if (!trocou) {
            break;
        }
    }
}

void insertion_sort_livros_codigo(LIVRO *livros, int total) {
    if (livros == NULL || total <= 1) {
        return;
    }

    for (int i = 1; i < total; i++) {
        LIVRO chave = livros[i];
        int j = i - 1;
        while (j >= 0 && livros[j].codigo > chave.codigo) {
            livros[j + 1] = livros[j];
            j--;
        }
        livros[j + 1] = chave;
    }
}

void insertion_sort_livros_titulo(LIVRO *livros, int total) {
    if (livros == NULL || total <= 1) {
        return;
    }

    for (int i = 1; i < total; i++) {
        LIVRO chave = livros[i];
        int j = i - 1;
        while (j >= 0 && comparar_titulos(livros[j].titulo, chave.titulo) > 0) {
            livros[j + 1] = livros[j];
            j--;
        }
        livros[j + 1] = chave;
    }
}

void insertion_sort_usuarios_matricula(USUARIO *usuarios, int total) {
    if (usuarios == NULL || total <= 1) {
        return;
    }

    for (int i = 1; i < total; i++) {
        USUARIO chave = usuarios[i];
        int j = i - 1;
        while (j >= 0 && usuarios[j].matricula > chave.matricula) {
            usuarios[j + 1] = usuarios[j];
            j--;
        }
        usuarios[j + 1] = chave;
    }
}
