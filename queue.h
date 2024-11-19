#ifndef QUEUE_H
#define QUEUE_H

#include "process.h" // Inclui a definição da estrutura Process

#define MAX_PROCESSES 10

// Estrutura da fila circular
typedef struct Queue {
    Process processes[MAX_PROCESSES]; // Array de processos
    int front, rear;                  // Índices para frente e trás da fila
} Queue;

// Funções de manipulação de filas
void initializeQueue(Queue *q);           // Inicializa uma fila
int isQueueEmpty(Queue *q);               // Verifica se a fila está vazia
int isQueueFull(Queue *q);                // Verifica se a fila está cheia
void enqueue(Queue *q, Process p);        // Adiciona um processo à fila
Process dequeue(Queue *q);                // Remove um processo da fila

#endif
