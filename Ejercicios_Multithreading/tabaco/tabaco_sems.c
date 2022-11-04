#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TABACO 0
#define CERILLAS 1
#define PAPEL 2

typedef struct{
    int ing;
}tharg_t;

int mesa[3] = {0};
sem_t sem_ag;
sem_t sem_comm;
sem_t sems_fum[3];

void *Agente(void * arg){
    int i,j,k;
    while(1){
        i = rand() % 3;
        j = (i + 1) % 3;
        k = (i + 1) % 3;
        sem_wait(&sem_ag);
        mesa[i] = 1;
        mesa[j] = 1;
        sem_post(&sems_fum[k]);
        sleep(1);
    }
}
void *Fumador(void * arg){
    int ing = ((tharg_t *)arg)->ing;
    int ing1 = (ing+1)%3;
    int ing2 = (ing+2)%3;
    while(1){
        sem_wait(&sems_fum[ing]);

        mesa[ing1] = 0;
        mesa[ing2] = 0;

        sem_post(&sem_ag);
        printf("Fumando %d\n", ing);
    }
}

int main(void){
    sem_init(&sem_ag, 0, 1);
    sem_init(&sem_comm, 0, 1);
    for(int i = 0; i < 3; i++){
        sem_init(&sems_fum[i], 0, 0);
    }

    pthread_t hilos[4];
    
    pthread_create(&hilos[0], NULL, Agente, NULL);

    tharg_t arg1 = {TABACO};
    pthread_create(&hilos[1], NULL, Fumador, &arg1);

    tharg_t arg2 = {CERILLAS};
    pthread_create(&hilos[2], NULL, Fumador, &arg2);

    tharg_t arg3 = {PAPEL};
    pthread_create(&hilos[3], NULL, Fumador, &arg3);

    for(int  i = 0; i < 4; i++){
        pthread_join(hilos[i], NULL);
    }
    return 0;
}