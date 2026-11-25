/*
 * relatorios.h
 * Benchmark comparativo de desempenho (ordenacao e busca) e
 * exportacao do relatorio consolidado da biblioteca em texto.
 */
#ifndef RELATORIOS_H
#define RELATORIOS_H

#include "livros.h"
#include "usuario.h"
#include "emprestimos.h"

#define QTD_LIVROS_BENCHMARK 5000
#define QTD_BUSCAS_BENCHMARK 1000

/* Mede Bubble Sort vs. Insertion Sort e Busca Sequencial vs. Pesquisa Binaria, exibindo e salvando o relatorio. */
void gerar_relatorio_desempenho(void);

/* Exporta resumo estatistico, acervo completo e emprestimos ativos para data/relatorio_biblioteca.txt. */
void exportar_relatorio_geral(LIVRO *livros, int total_liv,
                              USUARIO *usuarios, int total_usu,
                              EMPRESTIMO *emprestimos, int total_emp);

#endif
