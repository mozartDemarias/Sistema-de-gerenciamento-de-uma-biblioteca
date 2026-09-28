#include "../include/utils.h"

void limpar_enter(char str[]) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n' || str[i] == '\r') {
            str[i] = '\0';
            break;
        }
        i++;
    }
}

char para_minuscula(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}

// Compara se duas strings são exatamente iguais (case-insensitive) manualmente
int textos_iguais(char str1[], char str2[]) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (para_minuscula(str1[i]) != para_minuscula(str2[i])) {
            return 0;
        }
        i++;
    }
    return (str1[i] == '\0' && str2[i] == '\0');
}

// Verifica se a string 'busca' está dentro de 'texto_completo' manualmente
int contem_texto(char texto_completo[], char busca[]) {
    int i, j;
    if (busca[0] == '\0') return 0;

    for (i = 0; texto_completo[i] != '\0'; i++) {
        j = 0;
        while (busca[j] != '\0' && texto_completo[i + j] != '\0' &&
               para_minuscula(texto_completo[i + j]) == para_minuscula(busca[j])) {
            j++;
        }
        if (busca[j] == '\0') {
            return 1; // Encontrou!
        }
    }
    return 0;
}