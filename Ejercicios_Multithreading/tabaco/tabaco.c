#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define N 4
enum ingredient_t {
    TABACO,
    CERILLA,
    PAPEL
};

int mesa[3] = {0};

pthread_mutex_t m;
pthread_cond_t vc_fumador, vc_agente;


void* Fumador(void* arg){
    int ing = *((int *) arg);
    int ing1 = (ing+1)%3;
    int ing2 = (ing+2)%3;
    while(1){
        pthread_mutex_lock(&m);
        while(mesa[ing1] == 0 || mesa[ing2] == 0){        
            pthread_cond_wait(&vc_fumador, &m);
        }
        mesa[ing1] = 0;
        mesa[ing2] = 0;
        pthread_cond_signal(&vc_agente);
        pthread_mutex_unlock(&m);
        printf("Fumando %d\n", ing);
    }    
    pthread_exit(0);
}

void* Agente(void* arg){
    while(1){
        int ing1 = rand() % 3;
        int ing2 = (ing1 + (rand()%2+1)) % 3;
        pthread_mutex_lock(&m);
        while(mesa[TABACO] != 0 || mesa[PAPEL] != 0 || mesa[CERILLA] != 0){
            pthread_cond_wait(&vc_agente, &m);
        }
        mesa[ing1] = 1;
        mesa[ing2] = 1;
        pthread_cond_broadcast(&vc_fumador);
        pthread_mutex_unlock(&m);
        sleep(1);
    }
    pthread_exit(0);
}

int main(void){
    pthread_t hilos[N];
    srand(time(NULL));

    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&vc_agente, NULL);
    pthread_cond_init(&vc_fumador, NULL);
    
    pthread_create(&hilos[0], NULL, Agente, NULL);
    
    enum ingredient_t arg1 = TABACO;
    pthread_create(&hilos[1], NULL, Fumador, &arg1);

    enum ingredient_t arg2 = CERILLA;
    pthread_create(&hilos[2], NULL, Fumador, &arg2);

    enum ingredient_t arg3 = PAPEL;
    pthread_create(&hilos[3], NULL, Fumador, &arg3);


    for(int i = 0; i < N; i++){
        pthread_join(hilos[i], NULL);
    }
    return 0;
}