#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NSUR 2
#define NCLIENT 10

typedef struct{
    int dinero;
}tharg_t;

int surtidores[NSUR] = {0};
int turno = 0;
int ticket = 0;

pthread_mutex_t m;
pthread_cond_t cond;
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
    pthread_mutex_lock(&m);
    int my_ticket = ticket;
    ticket++;
    int id = -1;
    while (turno < my_ticket || (id = libre()) == -1)
    {
        pthread_cond_wait(&cond, &m);
    }
    surtidores[id] = 1;
    ++turno;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&m);
    return id;
}

void releasePump(int pump_id){
    pthread_mutex_lock(&m);
    surtidores[pump_id] = 0;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&m);

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

    pthread_mutex_init(&m,NULL);
    pthread_cond_init(&cond, NULL);
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