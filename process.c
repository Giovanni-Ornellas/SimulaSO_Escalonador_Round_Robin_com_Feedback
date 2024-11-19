#include "process.h"
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>

// Cria um processo aleatório com valores para tempo de CPU e I/O
Process createRandomProcess(int pid) {
    Process p;
    p.pid = pid;  // Atribui um ID único ao processo
    p.burst_time = rand() % 20 + 5;  // Tempo de CPU aleatório entre 5 e 25
    p.io_time = rand() % 10;         // Tempo de I/O aleatório entre 0 e 10

    // Escolhe aleatoriamente o dispositivo de I/O
    int device = rand() % 3;
    if (device == 0)
        strcpy(p.io_device, "disco");
    else if (device == 1)
        strcpy(p.io_device, "fita magnética");
    else
        strcpy(p.io_device, "impressora");

    return p;  // Retorna o processo criado
}
