#ifndef PROCESS_H
#define PROCESS_H

// Estrutura de um processo
typedef struct Process {
    int pid;             // ID do processo
    int burst_time;      // Tempo restante de execução na CPU
    int io_time;         // Tempo restante para operações de I/O
    char io_device[20];  // Tipo de dispositivo de I/O
} Process;

// Função para criar um processo aleatório
Process createRandomProcess(int pid); // Gera um processo aleatório com tempos e dispositivo de I/O

#endif
