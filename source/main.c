#include <stdio.h>
#include "../include/livros.h"
#include "../include/usuario.h"
#include "../include/emprestimos.h"

int main() {
    LIVRO lista_livros[MAX_LIVROS];
    int total_livros = 0;

    USUARIO lista_usuarios[MAX_USUARIOS];
    int total_usuarios = 0;

    EMPRESTIMO lista_emprestimos[MAX_EMPRESTIMOS];
    int total_emprestimos = 0;

    // Carrega todos os dados dos arquivos CSV ao iniciar
    carregar_livros(lista_livros, &total_livros);
    carregar_usuarios(lista_usuarios, &total_usuarios);
    carregar_emprestimos(lista_emprestimos, &total_emprestimos);

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
        printf("0- Sair\n");
        printf("Escolha a opcao que deseja: ");

        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                cadastrar_livro(lista_livros, &total_livros);
                break;
            case 2:
                cadastrar_usuario(lista_usuarios, &total_usuarios);
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
                registrar_emprestimo(lista_emprestimos, &total_emprestimos, lista_livros, total_livros, lista_usuarios, total_usuarios);
                break;
            case 10:
                registrar_devolucao(lista_emprestimos, total_emprestimos, lista_livros, total_livros);
                break;
            case 0:
                printf("Encerrando sistema...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (escolha != 0);

    return 0;
}