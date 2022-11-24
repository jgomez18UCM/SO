#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */


#define M 10

int finish = 0;

sem_t *salvajes_q;

sem_t *cocinero;
sem_t *salvaje;


void putServingsInPot(int servings, int * caldero)
{
	sem_wait(&cocinero);
	*caldero += servings;
	sem_post(&salvaje);
}

void cook(int * caldero)
{
	while(!finish) {
		putServingsInPot(M, caldero);
	}
}

void handler(int signo)
{
	finish = 1;
}



int main(int argc, char *argv[])
{

	int shd, *caldero;

	shd = shm_open("/CALDERO", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(shd, sizeof(int));
	caldero = (int*) mmap(NULL, sizeof(int), PROT_WRITE, MAP_SHARED, shd, 0);

	//Modificar señales


	//Setup semaforos

	cocinero = sem_open("/COCINERO", O_CREAT, 0700, 0);
	salvaje = sem_open("/SALVAJE", O_CREAT, 0700, 0);

	salvajes_q = sem_open("/SALVAJES_Q", O_CREAT, 0700, 0);

	cook(caldero);

	munmap(caldero, sizeof(int));
	shm_unlink("/CALDERO");
	
	sem_close(cocinero);
	sem_close(salvaje);
	sem_close(salvajes_q);

	sem_unlink("/COCINERO");
	sem_unlink("/SALVAJE");
	sem_unlink("/SALVAJES_Q");



	return 0;
}
