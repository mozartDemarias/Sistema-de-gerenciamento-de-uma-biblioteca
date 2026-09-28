#ifndef USUARIO_H
#define USUARIO_H

#define MAX_CURSO 50
#define MAX_NOME 100

typedef struct{
    int matricula;
    char curso[MAX_CURSO];
    char nome[MAX_NOME];
}USUARIO;

#endif