#include <stdio.h> 
#include "queue.h"       // Inclui funções para manipulação de filas
#include "process.h"     // Inclui funções para criação e manipulação de processos
#include "simulator.h"   // Inclui funções do simulador principal

int main() {
    Queue queues[PRIORITY_LEVELS];  // Define as filas de prioridade
    initializeQueues(queues);      // Inicializa todas as filas vazias

    // Criação de processos aleatórios
    for (int i = 0; i < MAX_PROCESSES; i++) {
        Process p = createRandomProcess(i + 1);  // Gera processo com ID único
        enqueue(&queues[PRIORITY_LEVELS - 1], p); // Insere na fila de maior prioridade
    }

    printf("Iniciando a simulação do algoritmo Round Robin com Feedback...\n");
    simulateRoundRobin(queues);  // Executa o simulador principal
    printf("Simulação concluída.\n");

    return 0;  // Indica que o programa terminou com sucesso
}
