#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "queue.h"  // Inclui manipulação de filas
#include "process.h" // Inclui manipulação de processos

#define PRIORITY_LEVELS 4  // Define o número de níveis de prioridade
#define TIME_SLICE 3       // Define a fatia de tempo para cada processo

// Funções principais do simulador
void initializeQueues(Queue queues[]);                     // Inicializa as filas de prioridade
void simulateRoundRobin(Queue queues[]);                   // Executa a simulação principal
void executeProcess(Queue *queue, Queue *lower_priority_queue, int priority_level); // Executa um processo
void processIO(Process *p, Queue queues[]);                // Gerencia o retorno de processos após I/O

#endif
