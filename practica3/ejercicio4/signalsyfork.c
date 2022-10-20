#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t pid;

void foo(int sig)
{
	kill(pid, SIGALRM);
}

/*programa que temporiza la ejecución de un proceso hijo */


int main(int argc, char **argv)
{

	pid = fork();

	if (pid == 0)
	{
		execvp(argv[1], argv + 1);
	}
	else
	{
		//crea la señal
		struct sigaction accion;
		accion.sa_handler = &foo;
		sigaction(SIGALRM, &accion, NULL);
		
		
		alarm(5);
	}
	int status = 0;

	while(wait(&status) != pid); //hace rla cosa de tal

	//hacer cosas con status

	return 0;
}
