#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 1
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

typedef struct {
	int id;
	int vip;
} client_t;

pthread_mutex_t m;

pthread_cond_t cola;

int turnoVip = 0; 
int turnoNormal = 0;

int ticketVip = 0;
int ticketNormal = 0;

int usuariosVip = 0;

int aforo = CAPACITY;

void enter_normal_client(int id)
{

	pthread_mutex_lock(&m);
	int mTicket = ticketNormal;
	++ticketNormal;
	
	while(aforo == 0 || turnoNormal != mTicket || usuariosVip != 0)
		pthread_cond_wait(&cola, &m);
	
	turnoNormal++;
	aforo--;
	printf("Client %2d (not vip) entering the disco\n", id);
	pthread_cond_broadcast(&cola);
	pthread_mutex_unlock(&m);
}

void enter_vip_client(int id)
{
	pthread_mutex_lock(&m);
	int mTicket = ticketVip;
	++ticketVip;
	usuariosVip++;
	while(aforo == 0 || turnoVip != mTicket)
		pthread_cond_wait(&cola, &m);
	
	turnoVip++;
	usuariosVip--;
	aforo--;
	printf("Client %2d (  vip  ) entering the disco\n", id);
	pthread_cond_broadcast(&cola);
	pthread_mutex_unlock(&m);
}

void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

void disco_exit(int id, int isvip)
{
	pthread_mutex_lock(&m);
	printf("Client %2d (%s) is leaving the disco\n", id, VIPSTR(isvip));
	aforo++;
	pthread_cond_broadcast(&cola);
	pthread_mutex_unlock(&m);
}

void *client(void *arg)
{
	client_t cliente  = *((client_t*)arg);

	if(cliente.vip){
		enter_vip_client(cliente.id);
		
	}else{
		enter_normal_client(cliente.vip);
	}
	dance(cliente.id, cliente.vip);
	disco_exit(cliente.id, cliente.vip);

}

int main(int argc, char *argv[])
{

	if(argc < 2){
		printf("Usage: ./disco file\n");
		return -1;
	}

	FILE* file;
	if((file = fopen(argv[1], "r")) == NULL){
		perror("Archivo");
		exit(EXIT_FAILURE);
	}

	int nClients = 0;
	if(fscanf(file, "%d", &nClients) == 0){
		perror("lectura");
		exit(EXIT_FAILURE);
	}
	fseek(file, 1, SEEK_CUR);
	printf("%d\n",nClients);
	client_t clients[nClients];
	pthread_t hilos[nClients];
	for(int i = 0; i < nClients; i++){
		fscanf(file,"%d",&clients[i].vip);
		clients[i].id = i;
		fseek(file, 1, SEEK_CUR);
		pthread_create(&hilos[i], NULL, client, &clients[i]);
	}

	for(int i = 0; i < nClients; i++){
		pthread_join(hilos[i], NULL);
	}
	
	return 0;
}
