/*
 * utils.h
 * Utilitarios manuais de manipulacao e comparacao de strings (sem funcoes prontas de busca).
 */
#ifndef UTILS_H
#define UTILS_H

/* Remove os caracteres de quebra de linha ('\n' e '\r') de uma string in-place. */
void limpar_enter(char str[]);

/* Descarta todos os caracteres restantes no buffer de entrada (stdin) ate '\n' ou EOF. */
void limpar_buffer_entrada(void);

/* Compara duas strings ignorando diferencas de maiusculas/minusculas ASCII. Retorna 1 se iguais, 0 caso contrario. */
int textos_iguais(char str1[], char str2[]);

/* Verifica manualmente se 'busca' ocorre como substring em 'texto_completo' (case-insensitive ASCII). */
int contem_texto(char texto_completo[], char busca[]);

#endif
