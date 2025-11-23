#include <stdio.h>
#include <string.h>

// 1. Definição da Estrutura (struct) Territorio
// Define um 'molde' para armazenar dados de um único território
typedef struct {
    char nome[30];    // Nome do território (String)
    char cor[10];     // Cor do exército (String)
    int tropas;       // Quantidade de tropas (Inteiro)
} Territorio;

// Definindo quantos territórios vamos cadastrar
#define NUM_TERRITORIOS 5

int main() {
    // Declaração: Cria um vetor (lista) capaz de guardar 5 territórios
    Territorio base_territorios[NUM_TERRITORIOS];
    int i; 

    printf("--- Cadastro Simplificado de Territórios ---\n");
    printf("Você irá cadastrar os dados de %d territórios.\n", NUM_TERRITORIOS);

    // 2. Laço para Entrada dos Dados (Cadastro)
    // Este loop executa 5 vezes (de i=0 até i=4)
    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\n--- Território #%d ---\n", i + 1);

        // Entrada do Nome (String)
        printf("Nome do Território: ");
        // %s lê uma palavra e armazena no campo 'nome'
        scanf("%s", base_territorios[i].nome);

        // Entrada da Cor (String)
        printf("Cor do Exército: ");
        // %s lê uma palavra e armazena no campo 'cor'
        scanf("%s", base_territorios[i].cor);

        // Entrada da Quantidade de Tropas (Inteiro)
        printf("Quantidade de Tropas: ");
        // %d lê um número inteiro e armazena no campo 'tropas' (usando & para o endereço)
        scanf("%d", &base_territorios[i].tropas);
    }

    // 3. Exibição dos Dados Registrados
    printf("\n\n---  Relatório Final de Territórios ---\n");

    // Este loop percorre o vetor e exibe os dados cadastrados
    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\n[%d] Território: %s\n", i + 1, base_territorios[i].nome);
        // Acesso aos membros da struct usando a notação de ponto (.)
        printf("   - Cor:    %s\n", base_territorios[i].cor);
        printf("   - Tropas: %d\n", base_territorios[i].tropas);
    }

    printf("\n--- Programa Finalizado ---\n");

    return 0;
}