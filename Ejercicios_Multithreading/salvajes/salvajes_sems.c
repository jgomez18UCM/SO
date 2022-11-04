#include <pthread.h>
#include <semaphore.h>

#define M 10

int Caldero = 0;

sem_t q_salvajes;
sem_t salvaje, cocinero;

void* Salvaje(void * arg){
    while(1){
        sem_wait(&q_salvajes);

        if(Caldero == 0){
            sem_post(&cocinero);
            sem_wait(&salvaje);
        }

        Caldero--;
        sem_post(&q_salvajes);
        //comer
    }
}

void* Cocinero(void * arg){
    while(1){
        sem_wait(&cocinero);
        Caldero += M;
        sem_post(&salvaje);
    }
}

int main(void){
    sem_init(&q_salvajes, 0, 1);
    sem_init(&cocinero, 0, 0);
    sem_init(&salvaje, 0, 0);
}