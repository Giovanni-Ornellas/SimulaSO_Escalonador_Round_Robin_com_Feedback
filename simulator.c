#include <stddef.h> 
#include <stdio.h> 
#include <string.h>
#include "simulator.h"

// Inicializa todas as filas de prioridade
void initializeQueues(Queue queues[]) {
    for (int i = 0; i < PRIORITY_LEVELS; i++) {
        initializeQueue(&queues[i]);  // Inicializa cada fila individualmente
    }
}

// Simula o algoritmo Round Robin com Feedback
void simulateRoundRobin(Queue queues[]) {
    int allQueuesEmpty = 0;  // Flag para verificar se todas as filas estão vazias

    while (!allQueuesEmpty) {
        allQueuesEmpty = 1;  // Assume que todas as filas estão vazias inicialmente

        // Itera sobre as filas da mais alta para a mais baixa prioridade
        for (int i = PRIORITY_LEVELS - 1; i >= 0; i--) {
            if (!isQueueEmpty(&queues[i])) {
                allQueuesEmpty = 0;  // Se uma fila não está vazia, há processos para executar
                executeProcess(&queues[i], (i > 0) ? &queues[i - 1] : NULL, i);
            }
        }
    }
}

// Executa um processo e gerencia sua preempção ou operações de I/O
void executeProcess(Queue *queue, Queue *lower_priority_queue, int priority_level) {
    Process p = dequeue(queue);  // Remove o processo do início da fila

    printf("Executando processo %d na fila de prioridade %d...\n", p.pid, priority_level + 1);

    if (p.burst_time > TIME_SLICE) {
        // Processo excedeu a fatia de tempo e deve ser rebaixado
        p.burst_time -= TIME_SLICE;
        printf("Processo %d excedeu o tempo da fatia. Rebaixando de prioridade.\n", p.pid);
        if (lower_priority_queue) enqueue(lower_priority_queue, p);
    } else if (p.burst_time > 0) {
        // Processo terminou sua execução
        printf("Processo %d finalizado com sucesso!\n", p.pid);
    } else if (p.io_time > 0) {
        // Processo precisa realizar I/O
        printf("Processo %d entrando em I/O (%s)...\n", p.pid, p.io_device);
        processIO(&p, queue);
    }
}

// Gerencia o retorno de processos após operações de I/O
void processIO(Process *p, Queue queues[]) {
    // Reinsere o processo na fila correspondente ao tipo de I/O
    if (strcmp(p->io_device, "disco") == 0) {
        enqueue(&queues[0], *p);  // Volta para a fila de prioridade mais baixa
    } else if (strcmp(p->io_device, "fita magnética") == 0) {
        enqueue(&queues[2], *p);  // Volta para uma fila intermediária
    } else if (strcmp(p->io_device, "impressora") == 0) {
        enqueue(&queues[3], *p);  // Volta para a fila de alta prioridade
    }
    printf("Processo %d voltou para a fila após I/O.\n", p->pid);
}
