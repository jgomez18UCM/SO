#include <pthread.h>
#define M 10
int caldero = 0;

pthread_mutex_t mut;
pthread_cond_t vc_salvajes, vc_cocinero;

void *Salvajes(void*arg){
    while (1)
    {
        pthread_mutex_lock(&mut);
        while(caldero == 0){
            pthread_cond_signal(&vc_cocinero);
            pthread_cond_wait(&vc_salvajes, &mut);
        }
        caldero --;
        pthread_mutex_unlock(&mut);
    }
    
}

void *Cocinero(void *arg){
    while(1){
        pthread_mutex_lock(&mut);
        while(caldero !=0){
            pthread_cond_wait(&vc_cocinero, &mut);
        }
        caldero += M;
        pthread_cond_broadcast(&vc_salvajes);
        pthread_mutex_lock(&mut);
    }

}

int main(void){
    pthread_t coc, sal[M];
    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&vc_salvajes, NULL);
    pthread_cond_init(&vc_cocinero, NULL);

    pthread_create(&coc, NULL, Cocinero, NULL);
    for(int i = 0; i < M; i++){
        pthread_create(&sal[i], NULL, Salvajes, NULL);
    }
    pthread_join(coc, NULL);
    for(int i = 0; i < M; i++){
        pthread_join(sal[i], NULL);
    }
    return 0;
}