#include <stdio.h>
#include <stdlib.h> // Para malloc, calloc e free
#include <string.h> // Para strcpy e strcmp, strstr
#include <time.h>   // Para a função time (aleatoriedade)

// ----------------------------------------------------
// 1. Definição da Estrutura (Struct)
// O molde para um território.
// ----------------------------------------------------
typedef struct {
    char nome[30];    // Nome do território
    char cor[10];     // Cor do exército (dono)
    int tropas;       // Quantidade de tropas
} Territorio;

// ----------------------------------------------------
// 2. Definição de Dados Globais e Missões
// ----------------------------------------------------

// Vetor de Strings (char*) para armazenar as descrições das missões
char *MISSOES[] = {
    "Missão 1: Conquistar 3 territórios seguidos em um único turno.",
    "Missão 2: Controlar 5 territórios com 3 ou mais tropas em cada.",
    "Missão 3: Eliminar todas as tropas da cor 'Vermelho'.",
    "Missão 4: Eliminar todas as tropas da cor 'Azul'.",
    "Missão 5: Conquistar um total de 8 territórios no mapa."
};
// Calcula quantos elementos (missões) existem no vetor
#define TOTAL_MISSOES (sizeof(MISSOES) / sizeof(MISSOES[0]))
// Cor assumida para o jogador principal, usada na verificação de missão
#define COR_JOGADOR "Verde" 

// ----------------------------------------------------
// 3. Protótipos das Funções (CORRIGIDO)
// ----------------------------------------------------
Territorio* alocarTerritorios(int num_territorios);
void liberarMemoria(Territorio* mapa, char* missao_jogador);
void cadastrarTerritorios(Territorio* mapa, int num_territorios);
void exibirTerritorios(const Territorio* mapa, int num_territorios);
void atacar(Territorio* atacante, Territorio* defensor);
int rolarDado(void);

// Funções de Missão
void atribuirMissao(char** destino_missao, char* missoes[], int totalMissoes);
void exibirMissao(const char* missao);
// CORREÇÃO: Adicionamos 'int vitorias_consecutivas' ao protótipo!
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho, int vitorias_consecutivas);


// ----------------------------------------------------
// 4. Função Principal (main)
// ----------------------------------------------------
int main() {
    srand(time(NULL)); 

    int num_territorios;
    Territorio* mapa = NULL; 
    char* missao_jogador = NULL; 
    int indice_atacante, indice_defensor;
    char continuar_ataque;
    int vitorias_consecutivas = 0; 
    int jogador_venceu = 0;

    printf("--- WAR Estruturado: Missões Estratégicas ---\n");

    // Alocação e Atribuição da Missão
    atribuirMissao(&missao_jogador, MISSOES, TOTAL_MISSOES);
    exibirMissao(missao_jogador);

    // Alocação dos Territórios
    printf("\nQuantos territórios você deseja cadastrar? ");
    scanf("%d", &num_territorios);
    while(getchar() != '\n'); 

    mapa = alocarTerritorios(num_territorios);
    if (mapa == NULL) {
        printf("Falha ao alocar memória. Programa encerrado.\n");
        liberarMemoria(NULL, missao_jogador); 
        return 1;
    }

    cadastrarTerritorios(mapa, num_territorios);
    exibirTerritorios(mapa, num_territorios);

    printf("\n\n--- INÍCIO DA FASE DE ATAQUE ---\n");

    do {
        printf("\nEscolha os territórios pelo índice [1 a %d]:\n", num_territorios);
        printf("Índice do Território ATACANTE: ");
        if (scanf("%d", &indice_atacante) != 1 || indice_atacante < 1 || indice_atacante > num_territorios) {
            printf("Índice ATACANTE inválido.\n");
            while(getchar() != '\n');
            continue;
        }

        printf("Índice do Território DEFENSOR: ");
        if (scanf("%d", &indice_defensor) != 1 || indice_defensor < 1 || indice_defensor > num_territorios) {
            printf("Índice DEFENSOR inválido.\n");
            while(getchar() != '\n');
            continue;
        }
        
        while(getchar() != '\n'); 

        indice_atacante--; 
        indice_defensor--; 

        // Validações
        if (strcmp(mapa[indice_atacante].cor, mapa[indice_defensor].cor) == 0) {
            printf("\nERRO: Você não pode atacar um território da sua própria cor (%s)!\n", mapa[indice_atacante].cor);
            vitorias_consecutivas = 0; 
        } else if (mapa[indice_atacante].tropas < 2) {
             printf("\nERRO: O atacante (%s) precisa de pelo menos 2 tropas para atacar.\n", mapa[indice_atacante].nome);
             vitorias_consecutivas = 0; 
        } else {
            // Guarda a cor original do defensor
            char cor_defensor_original[10];
            strcpy(cor_defensor_original, mapa[indice_defensor].cor);

            printf("\n--- %s [%s] ATACA %s [%s] ---\n", 
                   mapa[indice_atacante].nome, mapa[indice_atacante].cor, 
                   mapa[indice_defensor].nome, mapa[indice_defensor].cor);
            
            // Chama o ataque
            atacar(&mapa[indice_atacante], &mapa[indice_defensor]);

            // Se a cor mudou, significa que houve conquista
            if (strcmp(mapa[indice_defensor].cor, cor_defensor_original) != 0) {
                vitorias_consecutivas++;
            } else {
                vitorias_consecutivas = 0;
            }

            printf("\n--- STATUS APÓS O ATAQUE ---\n");
            exibirTerritorios(mapa, num_territorios);
            
            // CHAMADA DA FUNÇÃO DE VERIFICAÇÃO DE MISSÃO
            if (verificarMissao(missao_jogador, mapa, num_territorios, vitorias_consecutivas)) {
                jogador_venceu = 1;
                break; 
            }
        }

        printf("\nDeseja realizar outro ataque? (S/N): ");
        scanf(" %c", &continuar_ataque);
        while(getchar() != '\n'); 

    } while (continuar_ataque == 'S' || continuar_ataque == 's');

    // Mensagem final
    if (jogador_venceu) {
        printf("\n\n PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO E VENCEU O JOGO! \n");
    } else {
        printf("\n\nFim de jogo. Missão não cumprida.\n");
    }

    // Liberação de Memória
    liberarMemoria(mapa, missao_jogador);
    printf("\n--- Memória liberada e Programa Finalizado ---\n");

    return 0;
}

// ----------------------------------------------------
// 5. Implementação das Funções Auxiliares
// ----------------------------------------------------

Territorio* alocarTerritorios(int num_territorios) {
    // calloc zera a memória, bom para structs
    Territorio* mapa = (Territorio*) calloc(num_territorios, sizeof(Territorio));
    return mapa;
}

int rolarDado(void) {
    return (rand() % 6) + 1;
}

void cadastrarTerritorios(Territorio* mapa, int num_territorios) {
    int i;
    printf("\n--- Cadastro de Territórios ---\n");
    for (i = 0; i < num_territorios; i++) {
        printf("\n--- Território #%d ---\n", i + 1);
        printf("Nome do Território: ");
        scanf("%s", mapa[i].nome);
        printf("Cor do Exército: ");
        scanf("%s", mapa[i].cor);
        printf("Quantidade de Tropas: ");
        scanf("%d", &mapa[i].tropas); 
        while(getchar() != '\n'); 
    }
}

void exibirTerritorios(const Territorio* mapa, int num_territorios) {
    int i;
    printf("\n--- Relatório de Territórios (MAPA) ---\n");
    for (i = 0; i < num_territorios; i++) {
        printf("[%d] %s\n", i + 1, mapa[i].nome);
        printf("   - Cor:    %s\n", mapa[i].cor);
        printf("   - Tropas: %d\n", mapa[i].tropas);
    }
}

void atacar(Territorio* atacante, Territorio* defensor) {
    int dado_ataque, dado_defesa;
    
    dado_ataque = rolarDado();
    dado_defesa = rolarDado();

    printf("  -> Dado Atacante (%s): %d\n", atacante->cor, dado_ataque);
    printf("  -> Dado Defensor (%s): %d\n", defensor->cor, dado_defesa);
    
    // Conquista
    if (dado_ataque > dado_defesa) {
        printf("   O ATAQUE VENCEU! %s conquista %s.\n", atacante->cor, defensor->nome);
        
        // Mudança de dono
        strcpy(defensor->cor, atacante->cor);
        
        // Transferência de tropas
        int tropas_movidas = atacante->tropas / 2;
        if (tropas_movidas < 1) tropas_movidas = 1; 

        atacante->tropas -= tropas_movidas;
        defensor->tropas = tropas_movidas; 

    } else { // Defesa vence ou Empate
        printf("   DEFESA PREVALECE! Atacante perde 1 tropa.\n");
        
        // O atacante perde 1 tropa
        atacante->tropas -= 1;
        if (atacante->tropas < 1) atacante->tropas = 1; 
    }
}

void liberarMemoria(Territorio* mapa, char* missao_jogador) {
    // Libera o vetor de territórios
    if (mapa != NULL) {
        free(mapa);
    }
    // Libera a string da missão do jogador
    if (missao_jogador != NULL) {
        free(missao_jogador);
    }
}

// ----------------------------------------------------
// 6. Implementação das Funções de Missão
// ----------------------------------------------------

void atribuirMissao(char** destino_missao, char* missoes[], int totalMissoes) {
    int indice_sorteado = rand() % totalMissoes; 
    char* missao_sorteada = missoes[indice_sorteado];
    int tamanho_missao = strlen(missao_sorteada) + 1; 

    // Aloca a memória e armazena o endereço no ponteiro da main (usando o ponteiro para ponteiro)
    *destino_missao = (char*) malloc(tamanho_missao * sizeof(char));

    if (*destino_missao == NULL) {
        printf("Erro na alocação da missão.\n");
        return;
    }

    // Copia o conteúdo para o novo espaço alocado
    strcpy(*destino_missao, missao_sorteada); 
}

void exibirMissao(const char* missao) {
    printf("\n--- SUA MISSÃO SECRETA ---\n");
    printf("OBJETIVO: %s\n", missao);
    printf("--------------------------\n");
}

/**
 * @brief Avalia se a missão do jogador foi cumprida.
 * @param vitorias_consecutivas Novo parâmetro para checar a Missão 1.
 */
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho, int vitorias_consecutivas) {
    
    // Lógica da Missão 1: Conquistar 3 territórios seguidos
    if (strstr(missao, "Conquistar 3 territórios seguidos") != NULL) {
        if (vitorias_consecutivas >= 3) {
            printf("\n[Missão Cumprida]: Conquistou 3 territórios seguidos!\n");
            return 1;
        }
    }

    // Lógica da Missão 3 e 4: Eliminar Cor
    const char* cor_a_eliminar = NULL;

    if (strstr(missao, "Eliminar todas as tropas da cor 'Vermelho'") != NULL) {
        cor_a_eliminar = "Vermelho";
    } else if (strstr(missao, "Eliminar todas as tropas da cor 'Azul'") != NULL) {
        cor_a_eliminar = "Azul";
    }

    if (cor_a_eliminar != NULL) {
        for (int i = 0; i < tamanho; i++) {
            // Se encontrarmos um território da cor a ser eliminada
            if (strcmp(mapa[i].cor, cor_a_eliminar) == 0) {
                return 0; // Missão NÃO cumprida
            }
        }
        printf("\n[Missão Cumprida]: Eliminou todas as tropas da cor %s!\n", cor_a_eliminar);
        return 1; // Se o loop terminar, a cor foi eliminada!
    }

    // Lógica da Missão 5: Conquistar um total de 8 territórios
    if (strstr(missao, "Conquistar um total de 8 territórios") != NULL) {
        int territorios_do_jogador = 0;
        for (int i = 0; i < tamanho; i++) {
            // Conta quantos territórios são do COR_JOGADOR
            if (strcmp(mapa[i].cor, COR_JOGADOR) == 0) {
                territorios_do_jogador++;
            }
        }
        if (territorios_do_jogador >= 8) {
            printf("\n[Missão Cumprida]: Controla 8 ou mais territórios!\n");
            return 1;
        }
    }

    // Retorno padrão (incluindo a Missão 2 que não teve lógica implementada)
    return 0; 
}