#include <stdio.h>
#include <string.h>
#include "../include/livros.h"
#include "../include/usuario.h"

int main(){
    int escolha;
    LIVRO novo_livro;
    USUARIO novo_usuario; 
    FILE *arq; 

    do{
        printf("\n=== SISTEMA DE BIBLIOTECA ===\n");
        printf("Escolha a opcao que deseja:\n");
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
        printf("Opcao: ");

        scanf("%d", &escolha);

        switch(escolha){ 
            case 1:
                printf("\n--- Cadastrar Livro ---\n");
                
                printf("Digite o codigo do livro:\n");
                scanf("%d", &novo_livro.codigo);
                getchar(); 
                
                printf("Digite o titulo do livro:\n");
                fgets(novo_livro.titulo, sizeof(novo_livro.titulo), stdin);
                novo_livro.titulo[strcspn(novo_livro.titulo, "\n")] = '\0'; 
                
                printf("Digite o nome do autor(a) do livro:\n");
                fgets(novo_livro.autor, sizeof(novo_livro.autor), stdin);
                novo_livro.autor[strcspn(novo_livro.autor, "\n")] = '\0'; 
                
                printf("Digite o ano do livro:\n");
                scanf("%d", &novo_livro.ano);
                
                printf("Digite a quantidade:\n");
                scanf("%d", &novo_livro.quantidade);

                arq = fopen("../data/livros.csv", "a");
                if(arq == NULL){
                    printf("Erro ao abrir o arquivo livros.csv!\n");
                    break; 
                }

                fprintf(arq, "%d;%s;%s;%d;%d\n", 
                        novo_livro.codigo, 
                        novo_livro.titulo, 
                        novo_livro.autor, 
                        novo_livro.ano, 
                        novo_livro.quantidade);

                fclose(arq);
                printf("Livro cadastrado com sucesso!\n");
                break; 
            case 2:
                printf("\n--- Cadastrar Usuario ---\n");
                
                printf("Digite o numero de matricula:\n");
                scanf("%d", &novo_usuario.matricula);
                getchar(); 
                
                printf("Digite o nome de usuario:\n");
                fgets(novo_usuario.nome, sizeof(novo_usuario.nome), stdin);
                novo_usuario.nome[strcspn(novo_usuario.nome, "\n")] = '\0'; 
                
                printf("Digite o curso:\n");
                fgets(novo_usuario.curso, sizeof(novo_usuario.curso), stdin);
                novo_usuario.curso[strcspn(novo_usuario.curso, "\n")] = '\0'; 

                arq = fopen("../data/usuarios.csv", "a");
                if(arq == NULL){
                    printf("Erro ao abrir o arquivo usuarios.csv!\n");
                    break;
                }

                fprintf(arq, "%d;%s;%s\n", 
                        novo_usuario.matricula, 
                        novo_usuario.nome, 
                        novo_usuario.curso);

                fclose(arq);
                printf("Usuario cadastrado com sucesso!\n");
                break;

            case 3: 
                printf("\n--- Listagem de Livros ---\n");
                break;

            case 4: 
                printf("\n--- Listagem de Usuarios ---\n");
                break;

            case 5: 
                printf("\n--- Listagem de Emprestimos ---\n");
                break;

            case 6: 
                printf("\n--- Busca por Titulo ---\n");
                break;

            case 7: 
                printf("\n--- Busca por Autor ---\n");
                break;

            case 8: 
                printf("\n--- Busca por Matricula ---\n");
                break;

            case 9:
                printf("\n--- Registrar Emprestimo ---\n");
                break;

            case 10:
                printf("\n--- Registrar Devolucao ---\n");
                break;

            case 0:
                printf("\nEncerrando sistema...\n");
                break;

            default:
                printf("\nErro: Opcao invalida! Tente novamente.\n");
                break;
        }
            
    } while (escolha != 0);

    return 0;
}