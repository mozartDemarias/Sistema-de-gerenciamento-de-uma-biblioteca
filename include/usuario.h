#ifndef USUARIOS_H
#define USUARIOS_H

#define MAX_CURSO 50
#define MAX_NOME 100

typedef struct{
    int matricula;
    char curso[50_CURSO];
    char nome[MAX_NOME];
}USUARIOS;

#endif