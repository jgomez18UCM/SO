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
	
	sem_wait(&salvajes_q);

        if(caldero == 0){
            sem_post(&cocinero);
            sem_wait(&salvaje);
        }

        caldero--;
        sem_post(&salvajes_q);
	
}

void eat(void)
{
	unsigned long id = (unsigned long) getpid();
	printf("Savage %lu eating\n", id);
	sleep(rand() % 5);
}

void savages(int * caldero)
{
	int i = 0;
	for(i = 0; i < NUMITER; i++){
		getServingsFromPot(caldero);
		eat();
	}
}

int main(int argc, char *argv[])
{

	int shd, *caldero;

	shd = shm_open("/CALDERO", O_RDWR, 0);
	caldero = (int*) mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, shd, 0);

	cocinero = sem_open("/COCINERO", 0);
	salvaje = sem_open("/SALVAJE", 0);
	salvajes_q = sem_open("/SALVAJES_Q", 0);

	savages(caldero);

	return 0;
}
