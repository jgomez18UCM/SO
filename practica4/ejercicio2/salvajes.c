#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

#define NUMITER 3

sem_t *salvajes_q;

sem_t *cocinero;
sem_t *salvaje;

int getServingsFromPot(int * caldero)
{
	sem_wait(salvajes_q);
	if((*caldero) == 0){
		sem_post(cocinero);
		sem_wait(salvaje);
	}

	(*caldero)--;
	sem_post(salvajes_q);

}

void eat(int i)
{
	unsigned long id = (unsigned long) getpid();
	printf("Savage %lu ate %d times\n", id, i+1);
	sleep(rand() * id % 5);
}

void savages(int * caldero)
{
	int i = 0;
	for(i = 0; i < NUMITER; i++){
		getServingsFromPot(caldero);
		eat(i);
	}
}

int main(int argc, char *argv[])
{

	int shd, *caldero;

	if((shd = shm_open("/CALDERO", O_RDWR, 0)) == -1){
		printf("No se crearon los recursos compartidos: CALDERO");
		exit(EXIT_FAILURE);
	}
	caldero = (int*) mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, shd, 0);

	if((cocinero = sem_open("/COCINERO", 0)) == SEM_FAILED){
		printf("No se crearon los recursos compartidos: SEMAFORO COCINERO\n");
		munmap(caldero, sizeof(int));
		shm_unlink("/CALDERO");
		exit(EXIT_FAILURE);
	}
	if((salvaje = sem_open("/SALVAJE", 0)) == SEM_FAILED){
		printf("No se crearon los recursos compartidos: SEMAFORO SALVAJE\n");
		munmap(caldero, sizeof(int));
		shm_unlink("/CALDERO");
		sem_close(cocinero);
		exit(EXIT_FAILURE);
	}

	if((salvajes_q = sem_open("/SALVAJES_Q", 0)) == SEM_FAILED){
		printf("No se crearon los recursos compartidos: SEMAFORO COCINERO\n");
		munmap(caldero, sizeof(int));
		shm_unlink("/CALDERO");
		sem_close(cocinero);
		sem_close(salvaje);
		exit(EXIT_FAILURE);
	}

	savages(caldero);

	munmap(caldero, sizeof(int));
	sem_close(cocinero);
	sem_close(salvaje);
	sem_close(salvajes_q);
	printf("SALVAJE %lu terminado\n", (unsigned long) getpid());
	return 0;
}
