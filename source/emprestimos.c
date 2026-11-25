#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/emprestimos.h"
#include "../include/utils.h"

/* Garante espaco no vetor dinamico de emprestimos, dobrando a capacidade quando necessario. */
static int garantir_capacidade_emprestimos(EMPRESTIMO **emprestimos, int total, int *capacidade) {
    if (*emprestimos == NULL || *capacidade <= 0) {
        int cap_inicial = CAPACIDADE_INICIAL_EMPRESTIMOS;
        EMPRESTIMO *novo = (EMPRESTIMO *)malloc((size_t)cap_inicial * sizeof(EMPRESTIMO));
        if (novo == NULL) {
            printf("Erro critico: Falha ao alocar memoria para emprestimos!\n");
            return 0;
        }
        *emprestimos = novo;
        *capacidade = cap_inicial;
    }

    if (total >= *capacidade) {
        int nova_cap = (*capacidade) * 2;
        EMPRESTIMO *temp = (EMPRESTIMO *)realloc(*emprestimos, (size_t)nova_cap * sizeof(EMPRESTIMO));
        if (temp == NULL) {
            printf("Erro critico: Falha ao realocar memoria para emprestimos!\n");
            return 0;
        }
        *emprestimos = temp;
        *capacidade = nova_cap;
    }

    return 1;
}

void carregar_emprestimos(EMPRESTIMO **emprestimos, int *total, int *capacidade) {
    FILE *arq = fopen("../data/emprestimos.csv", "r");
    *total = 0;
    if (arq == NULL) {
        arq = fopen("data/emprestimos.csv", "r");
        if (arq == NULL) return;
    }

    char linha[512];
    while (fgets(linha, sizeof(linha), arq) != NULL) {
        limpar_enter(linha);
        if (linha[0] == '\0') continue;
        if (*total == 0 && (linha[0] < '0' || linha[0] > '9')) continue;

        EMPRESTIMO temp = {0};
        char *token = strtok(linha, ";");
        if (!token) continue;
        temp.id = atoi(token);

        token = strtok(NULL, ";");
        if (!token) continue;
        temp.matricula_usuario = atoi(token);

        token = strtok(NULL, ";");
        if (!token) continue;
        temp.codigo_livro = atoi(token);

        token = strtok(NULL, ";");
        if (!token) continue;
        temp.ativo = atoi(token);

        if (!garantir_capacidade_emprestimos(emprestimos, *total, capacidade)) {
            break;
        }

        (*emprestimos)[*total] = temp;
        (*total)++;
    }
    fclose(arq);
}

void salvar_emprestimos(EMPRESTIMO *emprestimos, int total) {
    FILE *arq = fopen("../data/emprestimos.csv", "w");
    if (arq == NULL) {
        arq = fopen("data/emprestimos.csv", "w");
        if (arq == NULL) {
            printf("Erro ao salvar emprestimos.csv!\n");
            return;
        }
    }

    fprintf(arq, "id;matricula_usuario;codigo_livro;ativo\n");
    for (int i = 0; i < total; i++) {
        fprintf(arq, "%d;%d;%d;%d\n",
                emprestimos[i].id,
                emprestimos[i].matricula_usuario,
                emprestimos[i].codigo_livro,
                emprestimos[i].ativo);
    }
    fclose(arq);
}

int carregar_emprestimos_bin(EMPRESTIMO **emprestimos, int *total, int *capacidade) {
    FILE *arq = fopen("../data/emprestimos.bin", "rb");
    if (arq == NULL) {
        arq = fopen("data/emprestimos.bin", "rb");
        if (arq == NULL) {
            return 0;
        }
    }

    int qtd = 0;
    if (fread(&qtd, sizeof(int), 1, arq) != 1 || qtd < 0) {
        fclose(arq);
        return 0;
    }

    int nova_cap = (qtd > 0) ? qtd : CAPACIDADE_INICIAL_EMPRESTIMOS;
    EMPRESTIMO *novo = (EMPRESTIMO *)malloc((size_t)nova_cap * sizeof(EMPRESTIMO));
    if (novo == NULL) {
        printf("Erro critico: Falha ao alocar memoria ao carregar emprestimos.bin!\n");
        fclose(arq);
        return 0;
    }

    if (qtd > 0) {
        if (fread(novo, sizeof(EMPRESTIMO), (size_t)qtd, arq) != (size_t)qtd) {
            free(novo);
            fclose(arq);
            return 0;
        }
    }

    fclose(arq);
    free(*emprestimos);
    *emprestimos = novo;
    *total = qtd;
    *capacidade = nova_cap;
    return 1;
}

void salvar_emprestimos_bin(EMPRESTIMO *emprestimos, int total) {
    FILE *arq = fopen("../data/emprestimos.bin", "wb");
    if (arq == NULL) {
        arq = fopen("data/emprestimos.bin", "wb");
        if (arq == NULL) {
            printf("Erro ao salvar emprestimos.bin!\n");
            return;
        }
    }

    if (fwrite(&total, sizeof(int), 1, arq) != 1) {
        printf("Erro ao gravar cabecalho de emprestimos.bin!\n");
        fclose(arq);
        return;
    }

    if (total > 0 && emprestimos != NULL) {
        if (fwrite(emprestimos, sizeof(EMPRESTIMO), (size_t)total, arq) != (size_t)total) {
            printf("Erro ao gravar registros em emprestimos.bin!\n");
        }
    }

    fclose(arq);
}

void registrar_emprestimo(EMPRESTIMO **emprestimos, int *total_emp, int *capacidade_emp,
                          LIVRO *livros, int total_liv, USUARIO *usuarios, int total_usu) {
    int matricula;
    printf("Digite a matricula do usuario:\n");
    scanf("%d", &matricula);

    int idx_usu = -1;
    for (int i = 0; i < total_usu; i++) {
        if (usuarios[i].matricula == matricula) {
            idx_usu = i;
            break;
        }
    }
    if (idx_usu == -1) {
        printf("Erro: Usuario com matricula %d nao encontrado.\n", matricula);
        return;
    }

    int codigo_livro;
    printf("Digite o codigo do livro:\n");
    scanf("%d", &codigo_livro);

    int idx_liv = -1;
    for (int i = 0; i < total_liv; i++) {
        if (livros[i].codigo == codigo_livro) {
            idx_liv = i;
            break;
        }
    }
    if (idx_liv == -1) {
        printf("Erro: Livro com codigo %d nao encontrado.\n", codigo_livro);
        return;
    }

    if (livros[idx_liv].quantidade <= 0) {
        printf("Erro: Quantidade insuficiente! Nenhum exemplar disponivel.\n");
        return;
    }

    if (!garantir_capacidade_emprestimos(emprestimos, *total_emp, capacidade_emp)) {
        return;
    }

    int novo_id = 1;
    for (int i = 0; i < *total_emp; i++) {
        if ((*emprestimos)[i].id >= novo_id) {
            novo_id = (*emprestimos)[i].id + 1;
        }
    }

    EMPRESTIMO novo = {0};
    novo.id = novo_id;
    novo.matricula_usuario = matricula;
    novo.codigo_livro = codigo_livro;
    novo.ativo = 1;

    (*emprestimos)[*total_emp] = novo;
    (*total_emp)++;

    livros[idx_liv].quantidade--;

    salvar_emprestimos(*emprestimos, *total_emp);
    salvar_emprestimos_bin(*emprestimos, *total_emp);
    salvar_livros(livros, total_liv);
    salvar_livros_bin(livros, total_liv);
    printf("Emprestimo registrado com sucesso! ID: %d\n", novo_id);
}

void registrar_devolucao(EMPRESTIMO *emprestimos, int total_emp, LIVRO *livros, int total_liv) {
    if (total_emp == 0) {
        printf("Nenhum emprestimo registrado.\n");
        return;
    }

    int id;
    printf("Digite o ID do emprestimo para devolucao:\n");
    scanf("%d", &id);

    int idx_emp = -1;
    for (int i = 0; i < total_emp; i++) {
        if (emprestimos[i].id == id) {
            idx_emp = i;
            break;
        }
    }
    if (idx_emp == -1) {
        printf("Erro: Emprestimo com ID %d nao encontrado.\n", id);
        return;
    }

    if (emprestimos[idx_emp].ativo == 0) {
        printf("Erro: Este emprestimo ja foi devolvido!\n");
        return;
    }

    emprestimos[idx_emp].ativo = 0;

    for (int i = 0; i < total_liv; i++) {
        if (livros[i].codigo == emprestimos[idx_emp].codigo_livro) {
            livros[i].quantidade++;
            salvar_livros(livros, total_liv);
            salvar_livros_bin(livros, total_liv);
            break;
        }
    }

    salvar_emprestimos(emprestimos, total_emp);
    salvar_emprestimos_bin(emprestimos, total_emp);
    printf("Devolucao registrada com sucesso!\n");
}

void listar_emprestimos(EMPRESTIMO *emprestimos, int total_emp) {
    if (total_emp == 0) {
        printf("Nenhum emprestimo registrado.\n");
        return;
    }

    printf("\n--- LISTA DE EMPRESTIMOS ---\n");
    for (int i = 0; i < total_emp; i++) {
        printf("ID: %d | Matricula: %d | Cod. Livro: %d | Status: %s\n",
               emprestimos[i].id,
               emprestimos[i].matricula_usuario,
               emprestimos[i].codigo_livro,
               emprestimos[i].ativo ? "Ativo" : "Devolvido");
    }
}
