#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura para armazenar dados de um território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// -------- Funções --------

// Função para cadastrar territórios dinamicamente
void cadastrarTerritorios(Territorio* mapa, int n, const char* cores[], int nCores) {
    int i, escolhaCor, c;
    for (i = 0; i < n; i++) {
        printf("\n--- Território %d ---\n", i + 1);

        // Nome
        printf("Digite o nome do território: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0'; // remove \n

        // Escolha da cor
        printf("Escolha a cor do exército:\n");
        for (c = 0; c < nCores; c++) {
            printf("  %d - %s\n", c + 1, cores[c]);
        }
        do {
            printf("Digite o número correspondente (1-%d): ", nCores);
            scanf("%d", &escolhaCor);
            getchar();
            if (escolhaCor < 1 || escolhaCor > nCores) {
                printf("Opção inválida! Tente novamente.\n");
            }
        } while (escolhaCor < 1 || escolhaCor > nCores);

        strcpy(mapa[i].cor, cores[escolhaCor - 1]);

        // Tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar();
    }
}

// Função para exibir territórios
void exibirTerritorios(Territorio* mapa, int n) {
    printf("\n=== Lista de Territórios ===\n");
    for (int i = 0; i < n; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor do exército: %s\n", mapa[i].cor);
        printf("Quantidade de tropas: %d\n", mapa[i].tropas);
    }
}

// Função para simular ataque
void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Não é possível atacar um território da mesma cor!\n");
        return;
    }
    if (atacante->tropas < 2) {
        printf("O atacante precisa ter pelo menos 2 tropas para atacar!\n");
        return;
    }

    printf("\n=== Batalha entre %s (atacante) e %s (defensor) ===\n",
           atacante->nome, defensor->nome);

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("%s rolou: %d\n", atacante->nome, dadoAtacante);
    printf("%s rolou: %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf(">> O atacante venceu a batalha!\n");
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;  // metade das tropas vão pro defensor
        atacante->tropas -= defensor->tropas;     // atacante perde metade
    } else {
        printf(">> O defensor resistiu! O atacante perde 1 tropa.\n");
        atacante->tropas -= 1;
    }
}

// Função para liberar memória
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

int main() {
    srand(time(NULL)); // semente para aleatoriedade

    int n, atacante, defensor;
    const char* cores[] = {"Verde", "Azul", "Amarelo", "Preto", "Roxo"};
    int nCores = 5;

    printf("Digite o número de territórios a cadastrar: ");
    scanf("%d", &n);
    getchar();

    // Alocação dinâmica
    Territorio* mapa = (Territorio*) calloc(n, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n, cores, nCores);
    exibirTerritorios(mapa, n);

    // Loop de ataque
    char continuar;
    do {
        printf("\nEscolha o território atacante (1-%d): ", n);
        scanf("%d", &atacante);
        printf("Escolha o território defensor (1-%d): ", n);
        scanf("%d", &defensor);
        getchar();

        if (atacante < 1 || atacante > n || defensor < 1 || defensor > n) {
            printf("Índices inválidos!\n");
        } else {
            atacar(&mapa[atacante - 1], &mapa[defensor - 1]);
            exibirTerritorios(mapa, n);
        }

        printf("\nDeseja realizar outro ataque? (s/n): ");
        scanf(" %c", &continuar);
        getchar();
    } while (continuar == 's' || continuar == 'S');

    liberarMemoria(mapa);
    return 0;
}