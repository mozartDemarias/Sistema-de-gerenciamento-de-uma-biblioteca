/*
 * relatorios.c
 * Benchmark comparativo de desempenho com <time.h> (Bubble Sort vs. Insertion Sort e
 * Busca Sequencial vs. Pesquisa Binaria) e exportacao do relatorio consolidado da biblioteca.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/relatorios.h"
#include "../include/ordenacao.h"

/* Busca sequencial manual por codigo, contabilizando comparacoes realizadas. */
static int busca_sequencial_codigo_bench(const LIVRO *livros, int total, int codigo, long *comparacoes) {
    for (int i = 0; i < total; i++) {
        (*comparacoes)++;
        if (livros[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

/* Pesquisa binaria manual por codigo, contabilizando comparacoes realizadas. */
static int busca_binaria_codigo_bench(const LIVRO *livros, int total, int codigo, long *comparacoes) {
    int inicio = 0;
    int fim = total - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        (*comparacoes)++;
        if (livros[meio].codigo == codigo) {
            return meio;
        }
        if (livros[meio].codigo < codigo) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1;
}

/* Formata e grava a tabela comparativa de desempenho no stream informado (terminal ou arquivo). */
static void escrever_relatorio(FILE *saida,
                               int qtd_livros,
                               int qtd_buscas,
                               double tempo_bubble_ms,
                               double tempo_insertion_ms,
                               double tempo_seq_ms,
                               double tempo_bin_ms,
                               long comp_seq_por_busca,
                               long comp_bin_por_busca) {
    fprintf(saida, "\n=====================================================================\n");
    fprintf(saida, "                 RELATORIO DE DESEMPENHO - PARTE 2                   \n");
    fprintf(saida, "=====================================================================\n");
    fprintf(saida, "Massa de teste (ordenacao): %d livros ficticios desordenados\n", qtd_livros);
    fprintf(saida, "Massa de teste (busca):     %d buscas no pior caso (ultimo codigo)\n", qtd_buscas);
    fprintf(saida, "---------------------------------------------------------------------\n");
    fprintf(saida, "%-28s | %-15s | %-18s\n", "Algoritmo de Ordenacao", "Tempo (ms)", "Observacao");
    fprintf(saida, "-----------------------------+-----------------+--------------------\n");
    fprintf(saida, "%-28s | %12.3f ms | O(n^2)\n", "Bubble Sort (codigo)", tempo_bubble_ms);
    fprintf(saida, "%-28s | %12.3f ms | O(n^2)\n", "Insertion Sort (codigo)", tempo_insertion_ms);
    fprintf(saida, "---------------------------------------------------------------------\n");
    fprintf(saida, "%-28s | %-15s | %-18s\n", "Algoritmo de Busca", "Tempo (ms)", "Comparacoes/busca");
    fprintf(saida, "-----------------------------+-----------------+--------------------\n");
    fprintf(saida, "%-28s | %12.3f ms | %ld\n", "Busca Sequencial", tempo_seq_ms, comp_seq_por_busca);
    fprintf(saida, "%-28s | %12.3f ms | %ld\n", "Pesquisa Binaria", tempo_bin_ms, comp_bin_por_busca);
    fprintf(saida, "=====================================================================\n");

    if (tempo_insertion_ms > 0.0) {
        fprintf(saida, "Razao Bubble / Insertion: %.2fx\n", tempo_bubble_ms / tempo_insertion_ms);
    }
    if (tempo_bin_ms > 0.0) {
        fprintf(saida, "Razao Sequencial / Binaria: %.2fx\n", tempo_seq_ms / tempo_bin_ms);
    }
    fprintf(saida, "Reducao de comparacoes por busca: %ld -> %ld\n\n",
            comp_seq_por_busca, comp_bin_por_busca);
}

/*
 * Executa o benchmark em massa temporaria alocada dinamicamente, libera a memoria ao termino,
 * exibe a tabela comparativa no terminal e persiste em data/relatorio_desempenho.txt.
 */
void gerar_relatorio_desempenho(void) {
    const int n = QTD_LIVROS_BENCHMARK;
    const int buscas = QTD_BUSCAS_BENCHMARK;

    LIVRO *base = (LIVRO *)malloc((size_t)n * sizeof(LIVRO));
    LIVRO *copia_bubble = (LIVRO *)malloc((size_t)n * sizeof(LIVRO));
    LIVRO *copia_insertion = (LIVRO *)malloc((size_t)n * sizeof(LIVRO));

    if (base == NULL || copia_bubble == NULL || copia_insertion == NULL) {
        printf("Erro critico: Falha ao alocar memoria para o relatorio de desempenho!\n");
        free(base);
        free(copia_bubble);
        free(copia_insertion);
        return;
    }

    for (int i = 0; i < n; i++) {
        memset(&base[i], 0, sizeof(LIVRO));
        base[i].codigo = i + 1;
        snprintf(base[i].titulo, MAX_TITULOS, "Livro Teste %05d", i + 1);
        snprintf(base[i].autor, MAX_AUTOR, "Autor Teste %05d", (i % 100) + 1);
        base[i].ano = 1950 + (i % 75);
        base[i].quantidade = 1 + (i % 10);
    }

    /* Embaralha deterministicamente (Fisher-Yates) para garantir entrada desordenada. */
    srand(42U);
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        LIVRO temp = base[i];
        base[i] = base[j];
        base[j] = temp;
    }

    /* Duplica o mesmo conjunto desordenado para comparar ambos os algoritmos em condicoes identicas. */
    memcpy(copia_bubble, base, (size_t)n * sizeof(LIVRO));
    memcpy(copia_insertion, base, (size_t)n * sizeof(LIVRO));

    clock_t inicio = clock();
    bubble_sort_livros_codigo(copia_bubble, n);
    clock_t fim = clock();
    double tempo_bubble_ms = ((double)(fim - inicio) * 1000.0) / CLOCKS_PER_SEC;

    inicio = clock();
    insertion_sort_livros_codigo(copia_insertion, n);
    fim = clock();
    double tempo_insertion_ms = ((double)(fim - inicio) * 1000.0) / CLOCKS_PER_SEC;

    /* Pior caso: ultimo elemento do vetor ordenado; acumuladores volatile impedem dead-code elimination. */
    int alvo = copia_insertion[n - 1].codigo;
    long total_comp_seq = 0;
    long total_comp_bin = 0;
    volatile int acc_seq = 0;
    volatile int acc_bin = 0;

    inicio = clock();
    for (int i = 0; i < buscas; i++) {
        acc_seq += busca_sequencial_codigo_bench(copia_insertion, n, alvo, &total_comp_seq);
    }
    fim = clock();
    double tempo_seq_ms = ((double)(fim - inicio) * 1000.0) / CLOCKS_PER_SEC;

    inicio = clock();
    for (int i = 0; i < buscas; i++) {
        acc_bin += busca_binaria_codigo_bench(copia_insertion, n, alvo, &total_comp_bin);
    }
    fim = clock();
    double tempo_bin_ms = ((double)(fim - inicio) * 1000.0) / CLOCKS_PER_SEC;

    (void)acc_seq;
    (void)acc_bin;

    long comp_seq_por_busca = total_comp_seq / buscas;
    long comp_bin_por_busca = total_comp_bin / buscas;

    free(base);
    free(copia_bubble);
    free(copia_insertion);

    escrever_relatorio(stdout, n, buscas,
                       tempo_bubble_ms, tempo_insertion_ms,
                       tempo_seq_ms, tempo_bin_ms,
                       comp_seq_por_busca, comp_bin_por_busca);

    FILE *arq = fopen("../data/relatorio_desempenho.txt", "w");
    if (arq == NULL) {
        arq = fopen("data/relatorio_desempenho.txt", "w");
    }

    if (arq == NULL) {
        printf("Erro ao salvar data/relatorio_desempenho.txt!\n");
        return;
    }

    escrever_relatorio(arq, n, buscas,
                       tempo_bubble_ms, tempo_insertion_ms,
                       tempo_seq_ms, tempo_bin_ms,
                       comp_seq_por_busca, comp_bin_por_busca);
    fclose(arq);
    printf("Relatorio salvo em data/relatorio_desempenho.txt!\n");
}

/*
 * Consolida estatisticas gerais, acervo completo e emprestimos ativos (cruzando usuario e livro)
 * no arquivo data/relatorio_biblioteca.txt.
 */
void exportar_relatorio_geral(LIVRO *livros, int total_liv,
                              USUARIO *usuarios, int total_usu,
                              EMPRESTIMO *emprestimos, int total_emp) {
    FILE *arq = fopen("../data/relatorio_biblioteca.txt", "w");
    if (arq == NULL) {
        arq = fopen("data/relatorio_biblioteca.txt", "w");
    }
    if (arq == NULL) {
        printf("Erro ao salvar data/relatorio_biblioteca.txt!\n");
        return;
    }

    int total_exemplares = 0;
    if (livros != NULL) {
        for (int i = 0; i < total_liv; i++) {
            total_exemplares += livros[i].quantidade;
        }
    }

    int ativos = 0;
    int devolvidos = 0;
    if (emprestimos != NULL) {
        for (int i = 0; i < total_emp; i++) {
            if (emprestimos[i].ativo) {
                ativos++;
            } else {
                devolvidos++;
            }
        }
    }

    fprintf(arq, "=====================================================================\n");
    fprintf(arq, "              RELATORIO CONSOLIDADO DA BIBLIOTECA                    \n");
    fprintf(arq, "=====================================================================\n\n");

    fprintf(arq, "1. RESUMO ESTATISTICO\n");
    fprintf(arq, "---------------------------------------------------------------------\n");
    fprintf(arq, "Total de titulos no acervo:      %d\n", total_liv);
    fprintf(arq, "Total de exemplares disponiveis: %d\n", total_exemplares);
    fprintf(arq, "Total de usuarios cadastrados:   %d\n", total_usu);
    fprintf(arq, "Total de emprestimos (geral):    %d\n", total_emp);
    fprintf(arq, "Emprestimos ativos:              %d\n", ativos);
    fprintf(arq, "Emprestimos devolvidos:          %d\n\n", devolvidos);

    fprintf(arq, "2. ACERVO COMPLETO DE LIVROS\n");
    fprintf(arq, "---------------------------------------------------------------------\n");
    if (total_liv == 0 || livros == NULL) {
        fprintf(arq, "Nenhum livro cadastrado.\n\n");
    } else {
        for (int i = 0; i < total_liv; i++) {
            fprintf(arq, "Cod: %d | Titulo: %s | Autor: %s | Ano: %d | Qtd: %d\n",
                    livros[i].codigo,
                    livros[i].titulo,
                    livros[i].autor,
                    livros[i].ano,
                    livros[i].quantidade);
        }
        fprintf(arq, "\n");
    }

    fprintf(arq, "3. EMPRESTIMOS ATIVOS\n");
    fprintf(arq, "---------------------------------------------------------------------\n");
    if (ativos == 0 || emprestimos == NULL) {
        fprintf(arq, "Nenhum emprestimo ativo no momento.\n");
    } else {
        for (int i = 0; i < total_emp; i++) {
            if (emprestimos[i].ativo) {
                /* Busca sequencial manual para resolver nome do usuario e titulo do livro. */
                const char *nome_usuario = "Usuario nao encontrado";
                if (usuarios != NULL) {
                    for (int u = 0; u < total_usu; u++) {
                        if (usuarios[u].matricula == emprestimos[i].matricula_usuario) {
                            nome_usuario = usuarios[u].nome;
                            break;
                        }
                    }
                }

                const char *titulo_livro = "Livro nao encontrado";
                if (livros != NULL) {
                    for (int l = 0; l < total_liv; l++) {
                        if (livros[l].codigo == emprestimos[i].codigo_livro) {
                            titulo_livro = livros[l].titulo;
                            break;
                        }
                    }
                }

                fprintf(arq, "ID: %d | Usuario: %s (Mat: %d) | Livro: %s (Cod: %d)\n",
                        emprestimos[i].id,
                        nome_usuario,
                        emprestimos[i].matricula_usuario,
                        titulo_livro,
                        emprestimos[i].codigo_livro);
            }
        }
    }

    fprintf(arq, "\n=====================================================================\n");
    fclose(arq);
    printf("Relatorio consolidado exportado para data/relatorio_biblioteca.txt!\n");
}
