#include <stdio.h>
#include <stdlib.h> // Para malloc, calloc e free
#include <string.h> // Para strcpy e strcmp
#include <time.h>   // Para a função time (inicialização da semente de aleatoriedade)

// ----------------------------------------------------
// 1. Definição da Estrutura (Struct)
// Define o 'molde' para armazenar os dados de um único território
// ----------------------------------------------------
typedef struct {
    char nome[30];    // Nome do território
    char cor[10];     // Cor do exército (dono)
    int tropas;       // Quantidade de tropas
} Territorio;

// ----------------------------------------------------
// 2. Protótipos das Funções (Declaração)
// Informa ao compilador sobre as funções que serão usadas
// ----------------------------------------------------
Territorio* alocarTerritorios(int num_territorios);
void liberarMemoria(Territorio* mapa);
void cadastrarTerritorios(Territorio* mapa, int num_territorios);
void exibirTerritorios(const Territorio* mapa, int num_territorios);
void atacar(Territorio* atacante, Territorio* defensor);
int rolarDado(void); // Função auxiliar para rolar um dado de 6 lados

// ----------------------------------------------------
// 3. Função Principal (main)
// Onde o programa começa a ser executado
// ----------------------------------------------------
int main() {
    // Garante que os números aleatórios (dados) serão diferentes a cada execução
    srand(time(NULL)); 

    int num_territorios;
    Territorio* mapa = NULL; // Ponteiro que irá apontar para o vetor de territórios
    int indice_atacante, indice_defensor;
    char continuar_ataque;

    printf("--- WAR Estruturado com Alocação Dinâmica e Ataque ---\n");

    // Pede ao usuário o tamanho do vetor (alocação dinâmica)
    printf("Quantos territórios você deseja cadastrar? ");
    scanf("%d", &num_territorios);
    // Limpa o buffer de entrada
    while(getchar() != '\n'); 

    // Chama a função que aloca a memória e a armazena no ponteiro 'mapa'
    mapa = alocarTerritorios(num_territorios);

    if (mapa == NULL) {
        printf("Falha ao alocar memória. Programa encerrado.\n");
        return 1; // Retorna com erro
    }

    // Chama a função de cadastro
    cadastrarTerritorios(mapa, num_territorios);
    
    // Mostra os territórios cadastrados antes dos ataques
    exibirTerritorios(mapa, num_territorios);

    printf("\n\n--- INÍCIO DA FASE DE ATAQUE ---\n");

    do {
        // Pede ao usuário os índices dos territórios
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
        
        while(getchar() != '\n'); // Limpa o buffer de entrada

        // Ajusta para o índice do vetor (base 0)
        indice_atacante--; 
        indice_defensor--; 

        // ⚠️ Validação: O atacante não pode atacar um território da própria cor
        if (strcmp(mapa[indice_atacante].cor, mapa[indice_defensor].cor) == 0) {
            printf("\nERRO: Você não pode atacar um território da sua própria cor (%s)!\n", mapa[indice_atacante].cor);
        } 
        // ⚠️ Validação: O atacante deve ter pelo menos 2 tropas para atacar
        else if (mapa[indice_atacante].tropas < 2) {
             printf("\nERRO: O atacante (%s) precisa de pelo menos 2 tropas para atacar. Tropas atuais: %d.\n", mapa[indice_atacante].nome, mapa[indice_atacante].tropas);
        }
        // Condição válida, executa o ataque
        else {
            // Chamada da função de ataque, usando ponteiros para os elementos do vetor
            // &mapa[indice_atacante] é o endereço da struct na posição 'indice_atacante'
            printf("\n--- %s [%s] ATACA %s [%s] ---\n", 
                   mapa[indice_atacante].nome, mapa[indice_atacante].cor, 
                   mapa[indice_defensor].nome, mapa[indice_defensor].cor);
            atacar(&mapa[indice_atacante], &mapa[indice_defensor]);

            // Exibe os dados atualizados
            printf("\n--- STATUS APÓS O ATAQUE ---\n");
            exibirTerritorios(mapa, num_territorios);
        }

        printf("\nDeseja realizar outro ataque? (S/N): ");
        scanf(" %c", &continuar_ataque);
        while(getchar() != '\n'); // Limpa o buffer de entrada

    } while (continuar_ataque == 'S' || continuar_ataque == 's');

    // 4. Liberação da Memória
    liberarMemoria(mapa);
    printf("\n--- Memória liberada e Programa Finalizado ---\n");

    return 0;
}

// ----------------------------------------------------
// 5. Implementação das Funções
// ----------------------------------------------------

/**
 * @brief Aloca dinamicamente o vetor de territórios.
 * @param num_territorios O tamanho do vetor a ser alocado.
 * @return Um ponteiro para o início da memória alocada, ou NULL se falhar.
 */
Territorio* alocarTerritorios(int num_territorios) {
    // calloc(quantidade, tamanho_em_bytes) aloca o espaço e ZERA todos os bytes.
    // É uma boa prática usar calloc para structs.
    Territorio* mapa = (Territorio*) calloc(num_territorios, sizeof(Territorio));
    return mapa;
}

/**
 * @brief Libera a memória alocada dinamicamente.
 * @param mapa O ponteiro para o início da memória alocada.
 */
void liberarMemoria(Territorio* mapa) {
    // É essencial chamar 'free' para devolver a memória ao sistema
    if (mapa != NULL) {
        free(mapa);
    }
}

/**
 * @brief Rola um dado de 6 lados (número aleatório de 1 a 6).
 * @return O valor do dado (1 a 6).
 */
int rolarDado(void) {
    // rand() % 6 gera de 0 a 5. Somando 1, gera de 1 a 6.
    return (rand() % 6) + 1;
}

/**
 * @brief Permite ao usuário cadastrar os dados de cada território.
 * @param mapa O ponteiro para o primeiro Territorio do vetor.
 * @param num_territorios O total de territórios.
 */
void cadastrarTerritorios(Territorio* mapa, int num_territorios) {
    int i;
    printf("\n--- Cadastro de Territórios ---\n");
    for (i = 0; i < num_territorios; i++) {
        printf("\n--- Território #%d ---\n", i + 1);

        // Acesso usando notação de vetor (mapa[i])
        // O C sabe que mapa[i] é o mesmo que *(mapa + i)
        printf("Nome do Território: ");
        scanf("%s", mapa[i].nome);

        printf("Cor do Exército: ");
        scanf("%s", mapa[i].cor);

        printf("Quantidade de Tropas: ");
        // scanf precisa do endereço, por isso usamos &
        scanf("%d", &mapa[i].tropas); 
        while(getchar() != '\n'); // Limpa o buffer
    }
}

/**
 * @brief Exibe os dados de todos os territórios cadastrados.
 * @param mapa O ponteiro constante para o vetor de territórios.
 * @param num_territorios O total de territórios.
 */
void exibirTerritorios(const Territorio* mapa, int num_territorios) {
    int i;
    printf("\n--- Relatório de Territórios (MAPA) ---\n");
    for (i = 0; i < num_territorios; i++) {
        // Acesso aos membros da struct usando a notação de ponto (.)
        printf("[%d] %s\n", i + 1, mapa[i].nome);
        printf("   - Cor:    %s\n", mapa[i].cor);
        printf("   - Tropas: %d\n", mapa[i].tropas);
    }
}

/**
 * @brief Simula um ataque entre dois territórios e atualiza seus dados.
 * @param atacante Ponteiro para a struct do território atacante.
 * @param defensor Ponteiro para a struct do território defensor.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    int dado_ataque, dado_defesa;
    
    // Rola os dados
    dado_ataque = rolarDado();
    dado_defesa = rolarDado();

    printf("  -> Dado Atacante (%s): %d\n", atacante->cor, dado_ataque);
    printf("  -> Dado Defensor (%s): %d\n", defensor->cor, dado_defesa);
    
    // Comparação: quem rolou o maior valor vence o confronto.
    if (dado_ataque > dado_defesa) {
        printf("  🏆 O ATAQUE VENCEU! %s conquista %s.\n", atacante->cor, defensor->nome);
        
        // Regra de Conquista: O território defensor muda de dono (cor)
        // Usa a setinha -> para acessar membros da struct através do ponteiro
        strcpy(defensor->cor, atacante->cor);
        
        // Regra de Tropas: O atacante move METADE de suas tropas (mínimo 1) para o defensor.
        int tropas_movidas = atacante->tropas / 2;
        if (tropas_movidas < 1) tropas_movidas = 1; // Pelo menos 1 tropa deve se mover

        atacante->tropas -= tropas_movidas;
        defensor->tropas = tropas_movidas; // As tropas do defensor agora são as que foram movidas

    } else if (dado_defesa > dado_ataque) {
        printf("  🛡️ A DEFESA VENCEU! %s resistiu ao ataque.\n", defensor->cor);
        
        // Regra de Perda: O atacante perde 1 tropa.
        atacante->tropas -= 1;
        if (atacante->tropas < 1) atacante->tropas = 1; // Não pode ter 0 tropas no WAR (deve ter pelo menos 1)

    } else {
        printf("  🤝 EMPATE! A defesa prevalece. Atacante perde 1 tropa.\n");
        // Em caso de empate, o defensor vence (regra comum no WAR/RISK).
        atacante->tropas -= 1;
        if (atacante->tropas < 1) atacante->tropas = 1;
    }

    // Informa o saldo de tropas
    printf("  -> Tropas de %s: %d\n", atacante->nome, atacante->tropas);
    printf("  -> Tropas de %s: %d\n", defensor->nome, defensor->tropas);
}