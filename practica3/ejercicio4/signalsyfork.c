#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

pid_t pid;
/*programa que temporiza la ejecución de un proceso hijo */
void func(int sig){
	kill(pid, SIGKILL);
}

int main(int argc, char **argv)
{
	pid = fork();
	if(pid == 0){
		execvp(argv[1], argv+1);
		exit(EXIT_FAILURE);
	}else{
		struct sigaction accion;
		accion.sa_handler = &func;
		alarm(5);
		sigaction(SIGALRM, &accion, NULL);
		struct sigaction ignorar;
		ignorar.sa_handler = SIG_IGN;
		sigaction(SIGINT, &ignorar, NULL);
	}

	int status;
	int ret;
	do{
		ret = wait(&status);
	}while(ret == -1 && errno == EINTR);
	
	if (ret == -1) {
		perror("wait");
		exit(EXIT_FAILURE);
	}
	
	if(WIFEXITED(status)){
		printf("El hijo ha salido\n");
	} 
	else if(WIFSIGNALED(status)){
		printf("He matado a mi hijo\n");
	}
	return 0;
}
