#ifndef RELATORIOS_H
#define RELATORIOS_H

#include "livros.h"
#include "usuario.h"
#include "emprestimos.h"

#define QTD_LIVROS_BENCHMARK 5000
#define QTD_BUSCAS_BENCHMARK 1000

void gerar_relatorio_desempenho(void);
void exportar_relatorio_geral(LIVRO *livros, int total_liv,
                              USUARIO *usuarios, int total_usu,
                              EMPRESTIMO *emprestimos, int total_emp);

#endif
