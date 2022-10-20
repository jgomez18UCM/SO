#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define HILOS 10


typedef struct{
	int id;
	char priority;
} usuario_t;

void *thread_usuario(void *arg)
{
	usuario_t* copia = (usuario_t*) arg;
	usuario_t us = {copia->id, copia->priority};
	//free(arg);
	printf("Mi id es %ld, con numero de usuario %d, y prioridad %c\n", pthread_self(), us.id, us.priority);
	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t threads[HILOS];
	
	for(int i = 0; i < HILOS; i++){	
		usuario_t* args = malloc(sizeof(usuario_t));
		args->id = i;
		args->priority =  i%2 == 0?'P':'N';
		pthread_create(&threads[i], NULL, &thread_usuario, args);
			
	}
	
	for(int i = 0; i < HILOS; i++)
		pthread_join(threads[i], NULL);
	return 0;
}
