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

// Estrutura para jogador
typedef struct {
    char* missao;   // missão sorteada dinamicamente
    char cor[10];   // cor principal do jogador
} Jogador;

// -------- Funções --------

// Cadastro dos territórios
void cadastrarTerritorios(Territorio* mapa, int n, const char* cores[], int nCores) {
    int i, escolhaCor;
    for (i = 0; i < n; i++) {
        printf("\n--- Território %d ---\n", i + 1);

        // Nome do território
        printf("Digite o nome do território: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        // Escolha da cor
        printf("Escolha a cor do exército:\n");
        for (int c = 0; c < nCores; c++) {
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

// Exibição do mapa
void exibirTerritorios(Territorio* mapa, int n) {
    printf("\n=== Lista de Territórios ===\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %s (Exército: %s , Tropas: %d)\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Ataque entre territórios
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
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas -= defensor->tropas;
    } else {
        printf(">> O defensor resistiu! O atacante perde 1 tropa.\n");
        atacante->tropas -= 1;
    }
}

// Sorteio e atribuição de missão
void atribuirMissao(char** destino, const char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    *destino = (char*) malloc((strlen(missoes[sorteio]) + 1) * sizeof(char));
    if (*destino == NULL) {
        printf("Erro ao alocar memória para missão!\n");
        exit(1);
    }
    strcpy(*destino, missoes[sorteio]);
}

// Exibir missão (apenas no início)
void exibirMissao(const char* missao, const char* cor, int jogador) {
    printf("\n--- SUA MISSAO (Jogador %d - Exército %s) ---\n", jogador, cor);
    printf("%s\n", missao);
}

// Verificar se a missão foi cumprida (exemplo simples)
int verificarMissao(const char* missao, Territorio* mapa, int tamanho, const char* corJogador) {
    // Exemplo: conquistar 3 territórios
    if (strstr(missao, "3 territorios") != NULL) {
        int count = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                count++;
            }
        }
        if (count >= 3) return 1;
    }

    // Exemplo: eliminar exército Verde
    if (strstr(missao, "exercito Verde") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Verde") == 0 && mapa[i].tropas > 0) {
                return 0;
            }
        }
        return 1;
    }

    return 0; // ainda não cumpriu
}

// Liberação de memória
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int totalJogadores) {
    free(mapa);
    for (int i = 0; i < totalJogadores; i++) {
        free(jogadores[i].missao);
    }
}

// -------- Função Principal --------
int main() {
    srand(time(NULL));

    int n;
    const char* cores[] = {"Verde", "Azul", "Amarelo", "Preto", "Roxo", "Branco"};
    int nCores = 6;

    const char* missoes[] = {
        "Conquistar 3 territorios",
        "Destruir o exercito Verde",
        "Controlar 2 territorios Azuis",
        "Ter 20 tropas em um unico territorio",
        "Conquistar todos os territorios Pretos"
    };
    int totalMissoes = 5;

    printf("Digite o número de territórios a cadastrar: ");
    scanf("%d", &n);
    getchar();

    Territorio* mapa = (Territorio*) calloc(n, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n, cores, nCores);
    exibirTerritorios(mapa, n);

    // Criando 2 jogadores
    Jogador jogadores[2];
    strcpy(jogadores[0].cor, "Azul");
    strcpy(jogadores[1].cor, "Vermelho");

    for (int i = 0; i < 2; i++) {
        atribuirMissao(&jogadores[i].missao, missoes, totalMissoes);
        exibirMissao(jogadores[i].missao, jogadores[i].cor, i + 1);
    }

    int turno = 0; // 0 -> jogador 1, 1 -> jogador 2
    int vencedor = -1;
    int opcao;

    do {
        printf("\n--- MENU DE ACOES (Jogador %d - Exército %s) ---\n",
               turno + 1, jogadores[turno].cor);
        printf("1 - Atacar\n");
        printf("2 - Verificar Missao\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao == 1) {
            int atacante, defensor;
            exibirTerritorios(mapa, n);
            printf("Escolha o território atacante (1-%d): ", n);
            scanf("%d", &atacante);
            printf("Escolha o território defensor (1-%d): ", n);
            scanf("%d", &defensor);
            getchar();

            if (atacante < 1 || atacante > n || defensor < 1 || defensor > n) {
                printf("Índices inválidos!\n");
            } else {
                atacar(&mapa[atacante - 1], &mapa[defensor - 1]);
                exibirTerritorios(mapa, n);

                // Verifica se missão foi cumprida automaticamente
                if (verificarMissao(jogadores[turno].missao, mapa, n, jogadores[turno].cor)) {
                    vencedor = turno + 1;
                }
            }
        } else if (opcao == 2) {
            if (verificarMissao(jogadores[turno].missao, mapa, n, jogadores[turno].cor)) {
                printf("\nParabéns Jogador %d! Você cumpriu sua missão!\n", turno + 1);
                vencedor = turno + 1;
            } else {
                printf("\nMissão ainda não cumprida.\n");
            }
        } else if (opcao == 0) {
            printf("\nJogo encerrado.\n");
            break;
        } else {
            printf("Opção inválida!\n");
        }

        if (vencedor != -1) {
            printf("\n=== O Jogador %d venceu cumprindo sua missão! ===\n", vencedor);
            break;
        }

        // Alterna turno entre os jogadores
        turno = (turno + 1) % 2;

    } while (1);

    liberarMemoria(mapa, jogadores, 2);
    return 0;
}