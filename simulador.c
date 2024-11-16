#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESSOS 10
#define QUANTUM 2
#define ALTA_PRIORIDADE 1
#define BAIXA_PRIORIDADE 0
#define TEMPO_MAX_SERVICO 20
#define TEMPO_MAX_IO 5

typedef struct Processo {
    int pid;
    int tempoServicoTotal;
    int tempoServicoRestante;
    int prioridade;
    int solicitouIO;
    char tipoIO;
    int tempoIO;
    struct Processo *proximo;
} Processo;

typedef struct Fila {
    Processo *frente;
    Processo *tras;
} Fila;

// Funções para manipulação das filas
void inicializarFila(Fila *fila) {
    fila->frente = NULL;
    fila->tras = NULL;
}

void enfileirar(Fila *fila, Processo *proc) {
    proc->proximo = NULL;
    if (fila->tras == NULL) {
        fila->frente = proc;
    } else {
        fila->tras->proximo = proc;
    }
    fila->tras = proc;
}

Processo* desenfileirar(Fila *fila) {
    if (fila->frente == NULL) return NULL;
    Processo *proc = fila->frente;
    fila->frente = proc->proximo;
    if (fila->frente == NULL) fila->tras = NULL;
    return proc;
}

// Função para criar processos iniciais
void criarProcessos(Fila *filaAltaPrioridade) {
    srand(time(NULL));
    for (int i = 0; i < MAX_PROCESSOS; i++) {
        Processo *proc = (Processo *)malloc(sizeof(Processo));
        proc->pid = i + 1;
        proc->tempoServicoTotal = (rand() % TEMPO_MAX_SERVICO) + 1;
        proc->tempoServicoRestante = proc->tempoServicoTotal;
        proc->prioridade = ALTA_PRIORIDADE;
        proc->solicitouIO = 0; // Inicialmente, não solicitou I/O
        proc->tipoIO = '\0';
        proc->tempoIO = 0;
        proc->proximo = NULL;
        enfileirar(filaAltaPrioridade, proc);
        printf("Processo %d criado com tempo de servico total %d\n", proc->pid, proc->tempoServicoTotal);
    }
}

// Função para atualizar processos em I/O
void atualizarProcessosIO(Fila *filaIO, Fila *filaAlta, Fila *filaBaixa, char tipoIO, int tempoAtual) {
    Processo *proc = filaIO->frente;
    Processo *procAnterior = NULL;

    while (proc != NULL) {
        proc->tempoIO--;
        if (proc->tempoIO <= 0) {
            printf("Tempo %d: Processo %d completou I/O (%c)\n", tempoAtual, proc->pid, tipoIO);
            // Remover da fila de I/O
            if (procAnterior == NULL) {
                filaIO->frente = proc->proximo;
            } else {
                procAnterior->proximo = proc->proximo;
            }
            if (proc == filaIO->tras) {
                filaIO->tras = procAnterior;
            }

            proc->solicitouIO = 0;
            // Decidir para qual fila retornar com base no tipo de I/O
            if (tipoIO == 'D') {
                proc->prioridade = BAIXA_PRIORIDADE;
                enfileirar(filaBaixa, proc);
            } else {
                proc->prioridade = ALTA_PRIORIDADE;
                enfileirar(filaAlta, proc);
            }

            // Avançar para o próximo processo
            if (procAnterior == NULL) {
                proc = filaIO->frente;
            } else {
                proc = procAnterior->proximo;
            }
        } else {
            procAnterior = proc;
            proc = proc->proximo;
        }
    }
}

// Função principal de escalonamento
void escalonarProcessos() {
    Fila filaAlta, filaBaixa;
    Fila filaDisco, filaFita, filaImpressora;
    inicializarFila(&filaAlta);
    inicializarFila(&filaBaixa);
    inicializarFila(&filaDisco);
    inicializarFila(&filaFita);
    inicializarFila(&filaImpressora);

    criarProcessos(&filaAlta);

    int tempoAtual = 0;
    while (filaAlta.frente != NULL || filaBaixa.frente != NULL ||
           filaDisco.frente != NULL || filaFita.frente != NULL || filaImpressora.frente != NULL) {

        // Atualizar processos em I/O
        atualizarProcessosIO(&filaDisco, &filaAlta, &filaBaixa, 'D', tempoAtual);
        atualizarProcessosIO(&filaFita, &filaAlta, &filaBaixa, 'F', tempoAtual);
        atualizarProcessosIO(&filaImpressora, &filaAlta, &filaBaixa, 'I', tempoAtual);

        // Escolher o próximo processo para executar
        Processo *procExec = NULL;
        if (filaAlta.frente != NULL) {
            procExec = desenfileirar(&filaAlta);
        } else if (filaBaixa.frente != NULL) {
            procExec = desenfileirar(&filaBaixa);
        }

        if (procExec != NULL) {
            // Executar o processo
            int tempoExec = (procExec->tempoServicoRestante < QUANTUM) ? procExec->tempoServicoRestante : QUANTUM;
            procExec->tempoServicoRestante -= tempoExec;
            tempoAtual += tempoExec;

            printf("Tempo %d: Executando processo %d por %d unidades de tempo (Prioridade: %s)\n",
                   tempoAtual, procExec->pid, tempoExec, procExec->prioridade == ALTA_PRIORIDADE ? "Alta" : "Baixa");

            // Decidir aleatoriamente se o processo solicita I/O
            if (procExec->tempoServicoRestante > 0 && (rand() % 5 == 0)) { // 20% de chance de solicitar I/O
                procExec->solicitouIO = 1;
                // Definir tipo de I/O aleatoriamente
                int tipoIO = rand() % 3;
                switch(tipoIO) {
                    case 0:
                        procExec->tipoIO = 'D'; // Disco
                        procExec->tempoIO = (rand() % TEMPO_MAX_IO) + 1;
                        enfileirar(&filaDisco, procExec);
                        break;
                    case 1:
                        procExec->tipoIO = 'F'; // Fita Magnética
                        procExec->tempoIO = (rand() % TEMPO_MAX_IO) + 1;
                        enfileirar(&filaFita, procExec);
                        break;
                    case 2:
                        procExec->tipoIO = 'I'; // Impressora
                        procExec->tempoIO = (rand() % TEMPO_MAX_IO) + 1;
                        enfileirar(&filaImpressora, procExec);
                        break;
                }
                printf("Tempo %d: Processo %d solicitou I/O (%c) por %d unidades de tempo\n",
                       tempoAtual, procExec->pid, procExec->tipoIO, procExec->tempoIO);
            } else if (procExec->tempoServicoRestante == 0) {
                // Processo terminou
                printf("Tempo %d: Processo %d finalizado\n", tempoAtual, procExec->pid);
                free(procExec);
            } else {
                // Preempção: mover para fila de baixa prioridade
                procExec->prioridade = BAIXA_PRIORIDADE;
                enfileirar(&filaBaixa, procExec);
                printf("Tempo %d: Processo %d sofreu preempcao e foi para a fila de baixa prioridade\n",
       tempoAtual, procExec->pid);

            }
        } else {
            // Não há processos prontos, avançar o tempo
            tempoAtual++;
        }
    }
}

int main() {
    escalonarProcessos();
    return 0;
}
