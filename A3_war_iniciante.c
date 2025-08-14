#include <stdio.h>
#include <string.h>

// Estrutura para armazenar dados de um território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {
    Territorio territorios[5];      // Vetor para armazenar até 5 territórios
    int i, escolhaCor, c;           // Variáveis de controle

    // Lista de cores pré-definidas
    const char *cores[] = {"Verde", "Azul", "Amarelo", "Preto", "Roxo"};

    printf("=== Cadastro de Territorios ===\n");

    // Loop de cadastro
    for (i = 0; i < 5; i++) {
        printf("\n--- Territorio %d ---\n", i + 1);

        // Nome do território
        printf("Digite o nome do territorio: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0'; // Remove \n

        // Escolha de cor
        printf("Escolha a cor do exercito:\n");
        for (c = 0; c < 5; c++) {
            printf("  %d - %s\n", c + 1, cores[c]);
        }
        do {
            printf("Digite o numero correspondente (1-5): ");
            scanf("%d", &escolhaCor);
            getchar(); // Limpa o \n deixado no buffer
            if (escolhaCor < 1 || escolhaCor > 5) {
                printf("Opcao invalida! Tente novamente.\n");
            }
        } while (escolhaCor < 1 || escolhaCor > 5);

        // Copia a cor escolhida para a struct
        strcpy(territorios[i].cor, cores[escolhaCor - 1]);

        // Quantidade de tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
        getchar(); // Limpa o \n
    }

    // Exibição dos dados
    printf("\n=== Lista de Territorios Cadastrados ===\n");
    for (i = 0; i < 5; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exercito: %s\n", territorios[i].cor);
        printf("Quantidade de tropas: %d\n", territorios[i].tropas);
    }

    return 0;
}