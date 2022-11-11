#include <pthread.h>

#define NP 10

int paquetes = 0;
int hayCamion = 0;

pthread_mutex_t m;
pthread_cond_t vc_workers, vc_entrada, vc_salida;

void *truck(void *arg){
    while(1){
        //Esperar a poder entrar
        pthread_mutex_lock(&m);
        while(hayCamion)
            pthread_cond_wait(&vc_entrada, &m);
        enter_dock();
        hayCamion=1;
        paquetes = 0;
        broadcast(&vc_workers);
        while(paquetes<10)
            pthread_cond_wait(&vc_salida, &m);
        hayCamion = 0;
        signal(&vc_entrada);
        unlock(&m);
        //Esperar a camion cargado
        delivery();
    }
}

void *worker(void *arg){
    int p;
    while(1){
        p = get_parcel();
        //Esperar a poder cargar
        while(!hayCamion || paquetes == 10)
            pthread_cond_wait(&vc_workers, &m);
        load_parcel(p);
        paquetes++;
        //Notificar carga
        signal(&vc_salida);
        unlock(&m);
    }
}