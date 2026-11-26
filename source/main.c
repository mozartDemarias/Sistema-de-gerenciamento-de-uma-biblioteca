/*
 * main.c
 * Ponto de entrada do Sistema de Gerenciamento de Biblioteca Universitaria.
 * Inicializa vetores dinamicos (priorizando .bin com fallback para .csv),
 * executa o menu interativo e libera toda a memoria alocada ao encerrar.
 */
#include <stdio.h>
#include <stdlib.h>
#include "../include/livros.h"
#include "../include/usuario.h"
#include "../include/emprestimos.h"
#include "../include/ordenacao.h"
#include "../include/relatorios.h"
#include "../include/utils.h"

int main(void) {
    int capacidade_livros = 0;
    int total_livros = 0;
    LIVRO *lista_livros = NULL;

    int capacidade_usuarios = 0;
    int total_usuarios = 0;
    USUARIO *lista_usuarios = NULL;

    int capacidade_emprestimos = 0;
    int total_emprestimos = 0;
    EMPRESTIMO *lista_emprestimos = NULL;

    /* Tenta carregar dos arquivos binarios (.bin); caso nao existam, faz fallback para CSV. */
    if (!carregar_livros_bin(&lista_livros, &total_livros, &capacidade_livros)) {
        capacidade_livros = CAPACIDADE_INICIAL_LIVROS;
        lista_livros = (LIVRO *)malloc((size_t)capacidade_livros * sizeof(LIVRO));
        if (lista_livros == NULL) {
            printf("Erro critico: Falha ao alocar memoria inicial para livros!\n");
            return 1;
        }
        carregar_livros(&lista_livros, &total_livros, &capacidade_livros);
    }

    if (!carregar_usuarios_bin(&lista_usuarios, &total_usuarios, &capacidade_usuarios)) {
        capacidade_usuarios = CAPACIDADE_INICIAL_USUARIOS;
        lista_usuarios = (USUARIO *)malloc((size_t)capacidade_usuarios * sizeof(USUARIO));
        if (lista_usuarios == NULL) {
            printf("Erro critico: Falha ao alocar memoria inicial para usuarios!\n");
            free(lista_livros);
            return 1;
        }
        carregar_usuarios(&lista_usuarios, &total_usuarios, &capacidade_usuarios);
    }

    if (!carregar_emprestimos_bin(&lista_emprestimos, &total_emprestimos, &capacidade_emprestimos)) {
        capacidade_emprestimos = CAPACIDADE_INICIAL_EMPRESTIMOS;
        lista_emprestimos = (EMPRESTIMO *)malloc((size_t)capacidade_emprestimos * sizeof(EMPRESTIMO));
        if (lista_emprestimos == NULL) {
            printf("Erro critico: Falha ao alocar memoria inicial para emprestimos!\n");
            free(lista_livros);
            free(lista_usuarios);
            return 1;
        }
        carregar_emprestimos(&lista_emprestimos, &total_emprestimos, &capacidade_emprestimos);
    }

    /* Loop principal do menu interativo com protecao contra EOF e entradas nao numericas. */
    int escolha;
    do {
        printf("\n=== SISTEMA DE BIBLIOTECA ===\n");
        printf("1- Cadastrar livro\n");
        printf("2- Cadastrar usuario\n");
        printf("3- Listagem de livros\n");
        printf("4- Listagem de usuarios\n");
        printf("5- Listagem de emprestimos\n");
        printf("6- Busca por titulo\n");
        printf("7- Busca por autor\n");
        printf("8- Busca por matricula\n");
        printf("9- Registrar emprestimo\n");
        printf("10- Registrar devolucao\n");
        printf("11- Ordenar acervo (Bubble Sort / Insertion Sort)\n");
        printf("12- Ordenar usuarios por matricula (Insertion Sort)\n");
        printf("13- Busca binaria de livro por codigo\n");
        printf("14- Busca binaria de usuario por matricula\n");
        printf("15- Sincronizar/salvar dados em binario (.bin)\n");
        printf("16- Gerar Relatorio de Desempenho\n");
        printf("17- Exibir historico de emprestimos por usuario\n");
        printf("18- Exportar relatorio consolidado da biblioteca\n");
        printf("0- Sair\n");
        printf("Escolha a opcao que deseja: ");

        int status_leitura = scanf("%d", &escolha);
        if (status_leitura == EOF) {
            escolha = 0;
        } else if (status_leitura != 1) {
            limpar_buffer_entrada();
            escolha = -1;
        }

        switch (escolha) {
            case 1:
                cadastrar_livro(&lista_livros, &total_livros, &capacidade_livros);
                break;
            case 2:
                cadastrar_usuario(&lista_usuarios, &total_usuarios, &capacidade_usuarios);
                break;
            case 3:
                listar_livros(lista_livros, total_livros);
                break;
            case 4:
                listar_usuarios(lista_usuarios, total_usuarios);
                break;
            case 5:
                listar_emprestimos(lista_emprestimos, total_emprestimos);
                break;
            case 6:
                buscar_livro_titulo(lista_livros, total_livros);
                break;
            case 7:
                buscar_livro_autor(lista_livros, total_livros);
                break;
            case 8:
                buscar_usuario_matricula(lista_usuarios, total_usuarios);
                break;
            case 9:
                registrar_emprestimo(&lista_emprestimos, &total_emprestimos, &capacidade_emprestimos,
                                     lista_livros, total_livros, lista_usuarios, total_usuarios);
                break;
            case 10:
                registrar_devolucao(lista_emprestimos, total_emprestimos, lista_livros, total_livros);
                break;
            case 11: {
                if (total_livros == 0) {
                    printf("Nenhum livro cadastrado.\n");
                    break;
                }
                int algoritmo = 0;
                int criterio = 0;

                printf("\n--- ORDENAR ACERVO ---\n");
                printf("Escolha o algoritmo:\n");
                printf("1- Bubble Sort\n");
                printf("2- Insertion Sort\n");
                printf("Opcao: ");
                if (scanf("%d", &algoritmo) != 1) {
                    limpar_buffer_entrada();
                    printf("Opcao de algoritmo invalida!\n");
                    break;
                }

                printf("Escolha o criterio:\n");
                printf("1- Codigo\n");
                printf("2- Titulo (ordem alfabetica)\n");
                printf("Opcao: ");
                if (scanf("%d", &criterio) != 1) {
                    limpar_buffer_entrada();
                    printf("Criterio invalido!\n");
                    break;
                }

                if (algoritmo == 1 && criterio == 1) {
                    bubble_sort_livros_codigo(lista_livros, total_livros);
                } else if (algoritmo == 1 && criterio == 2) {
                    bubble_sort_livros_titulo(lista_livros, total_livros);
                } else if (algoritmo == 2 && criterio == 1) {
                    insertion_sort_livros_codigo(lista_livros, total_livros);
                } else if (algoritmo == 2 && criterio == 2) {
                    insertion_sort_livros_titulo(lista_livros, total_livros);
                } else {
                    printf("Opcao de algoritmo ou criterio invalida!\n");
                    break;
                }

                printf("Acervo ordenado com sucesso!\n");
                listar_livros(lista_livros, total_livros);
                break;
            }
            case 12:
                insertion_sort_usuarios_matricula(lista_usuarios, total_usuarios);
                if (total_usuarios > 0) {
                    printf("Usuarios ordenados por matricula com sucesso!\n");
                }
                listar_usuarios(lista_usuarios, total_usuarios);
                break;
            case 13:
                buscar_livro_binaria_codigo(lista_livros, total_livros);
                break;
            case 14:
                buscar_usuario_binaria_matricula(lista_usuarios, total_usuarios);
                break;
            case 15:
                salvar_livros_bin(lista_livros, total_livros);
                salvar_usuarios_bin(lista_usuarios, total_usuarios);
                salvar_emprestimos_bin(lista_emprestimos, total_emprestimos);
                printf("Dados sincronizados e salvos em arquivos binarios (.bin) com sucesso!\n");
                break;
            case 16:
                gerar_relatorio_desempenho();
                break;
            case 17:
                exibir_historico_usuario(lista_emprestimos, total_emprestimos,
                                         lista_livros, total_livros,
                                         lista_usuarios, total_usuarios);
                break;
            case 18:
                exportar_relatorio_geral(lista_livros, total_livros,
                                         lista_usuarios, total_usuarios,
                                         lista_emprestimos, total_emprestimos);
                break;
            case 0:
                printf("Encerrando sistema...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (escolha != 0);

    /* Libera toda a memoria alocada dinamicamente antes de encerrar o programa. */
    free(lista_livros);
    free(lista_usuarios);
    free(lista_emprestimos);
    lista_livros = NULL;
    lista_usuarios = NULL;
    lista_emprestimos = NULL;

    return 0;
}
