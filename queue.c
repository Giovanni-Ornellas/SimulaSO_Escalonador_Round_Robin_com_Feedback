#include "queue.h"  // Cabeçalho contendo as definições de Queue e funções associadas
#include <stdio.h> 

// Inicializa uma fila, configurando os ponteiros de início e fim
void initializeQueue(Queue *q) {
    q->front = -1;
    q->rear = -1;
}

// Verifica se uma fila está vazia
int isQueueEmpty(Queue *q) {
    return q->front == -1;  // Retorna verdadeiro se o ponteiro front não foi configurado
}

// Verifica se a fila está cheia
int isQueueFull(Queue *q) {
    return (q->rear + 1) % MAX_PROCESSES == q->front;  // Checa overflow circular
}

// Adiciona um processo ao final da fila
void enqueue(Queue *q, Process p) {
    if (isQueueFull(q)) {
        printf("Erro: Fila cheia!\n");
        return;
    }
    if (isQueueEmpty(q)) q->front = 0;  // Define front se for o primeiro elemento
    q->rear = (q->rear + 1) % MAX_PROCESSES;  // Incrementa rear circularmente
    q->processes[q->rear] = p;  // Insere o processo na posição rear
}

// Remove um processo do início da fila
Process dequeue(Queue *q) {
    if (isQueueEmpty(q)) {
        printf("Erro: Fila vazia!\n");
        Process empty = {-1, 0, 0, ""};
        return empty;  // Retorna processo vazio em caso de erro
    }
    Process p = q->processes[q->front];  // Salva o processo da posição front
    if (q->front == q->rear) {
        q->front = -1;  // Reseta fila vazia
        q->rear = -1;
    } else {
        q->front = (q->front + 1) % MAX_PROCESSES;  // Incrementa front circularmente
    }
    return p;  // Retorna o processo removido
}
