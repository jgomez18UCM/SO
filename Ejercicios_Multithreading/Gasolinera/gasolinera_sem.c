#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define NSUR 2
#define NCLIENT 10

typedef struct{
    int dinero;
}tharg_t;

int surtidores[NSUR] = {0};
// int turno = 0;
// int ticket = 0;

sem_t cola, m;

void PumpFuel(int pump_id, int price){
    printf("Cliente %d en surtidor %d\n", price, pump_id);
    sleep(2);  
}

int libre(void){
    for(int i = 0; i < NSUR; i++){
        if(surtidores[i] == 0) {
            //printf("%d\n", i);
            return i;
        }
    }
    return -1;
}

int getUnusedPump(void){
    sem_wait(&cola);
    sem_wait(&m);
    int id = libre();
    surtidores[id] = 1;
    sem_post(&m);
    return id;
}

void releasePump(int pump_id){   
    surtidores[pump_id] = 0;  
    sem_post(&cola);

}

void* cliente(void * arg){
    int dinero = ((tharg_t*)arg)->dinero;

    int pump;
    pump=getUnusedPump();
    /* Uso del surtidor */
    PumpFuel(pump,dinero);
    
    /* Deja de usar el surtidor */
    releasePump(pump);

    free(arg);
    pthread_exit(NULL);
}
int main(void){

    sem_init(&cola, 0, NSUR);
    sem_init(&m, 0, 1);
    pthread_t clientes[NCLIENT];

    for (int i = 0; i < NCLIENT; i++)
    {
        tharg_t* arg = malloc(sizeof(tharg_t));
        arg->dinero = i;
        pthread_create(&clientes[i], NULL, cliente, arg);
    }
    
    for(int i = 0; i < NCLIENT; i++){
        pthread_join(clientes[i], NULL);
    }

    return 0;
}