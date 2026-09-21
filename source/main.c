#include <stdio.h>
#include "../include/livros.h"
#include "../include/usuarios.h"


int main(){
    int escolha;
    LIVRO novo_livro;
    USUARIOS novo_usuario;
    do{
        printf("Escolha a opção que deseja");
        printf("1- Cadastrar livro\n");
        printf("2- Cadastar usuario\n");
        printf("3- Listagem de livros\n");
        printf("4- Listagem de usuarios\n");
        printf("5- Listagem de emprestimos\n");
        printf("6- Busca por titulo\n");
        printf("7- Busca por autor\n");
        printf("8- Busca por matricula\n");
        printf("0- Sair");

        scanf("%d", &escolha);

        switch(escolha){ 
            case 1:
                printf("Você escolheu a opção 1-cadastrar livro\n");
                printf("Digite o codigo do livro:  \n");
                scanf("%d", &novo_livro.codigo);
                printf("Digite o titulo do livro: \n");
                scanf("%s", novo_livro.titulo);
                printf("Digite o nome do autor(a) do livro: \n");
                scanf("%s", novo_livro.autor);
                printf("Digite o ano do livro: \n");
                scanf("%d", &novo_livro.ano);
                printf("Digite a quantidade: \n");
                scanf("%d", &novo_livro.quantidade);
                break;

            case 2:
                printf("Você escolheu cadastrar usuario!\n");
                printf("Digite o numero de matricula: \n");
                scanf("%d", &novo_usuario.matricula);
                printf("digite o nome de usuario: \n");
                scanf("%s", novo_usuario.nome);
                printf("Digite o curso: \n");
                scanf("%s", novo_usuario.curso);
                break;
            case 0:
                printf("Encerrando sistema\n");
                break;
        }
            
    }while (escolha != 0);

    return 0;
}