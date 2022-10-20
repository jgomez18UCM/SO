#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_HILOS 5
typedef struct 
{
	int t_ID;
	char t_TYPE;
}thread_data;


void *thread_usuario(void *arg)
{
	thread_data * copia = (thread_data *) arg;

	int copiaTID = copia->t_ID;
	char copiaTTYPE = copia->t_TYPE;

	free(arg);

	pthread_t self = pthread_self();

	printf("Hola, soy el proceso %ld, nº %d y soy", self, copiaTID);
	if(copiaTTYPE == 'P'){printf(" prioritario\n");}
	else if(copiaTTYPE == 'N'){printf(" no prioritario\n");}
	else printf(" un error\n");
}

int main(int argc, char* argv[])
{

		pthread_t threads [NUM_HILOS];
	for (int i = 0; i < (NUM_HILOS); i++)
	{
		 thread_data * data = malloc(sizeof(thread_data));
		data->t_ID = i;
		if(i % 2 == 0)
		{//es par
			data->t_TYPE = 'P';
		}
		else
		{//es parn't 	
			data->t_TYPE = 'N';
		}
		
		pthread_attr_t attr;
		pthread_create(&threads[i], NULL, thread_usuario, data);

	}
	
		for (int i = 0; i < NUM_HILOS; i++)
		{
		pthread_join(threads[i], NULL);
			/* code */
		}
		




	return 0;
}
