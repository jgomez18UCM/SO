#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*programa que crea dos hijos: uno no cambia de ejecutable y otro si */


int main(int argc, char **argv)
{

	

	pid_t pid = fork();

	//si son padre o hijo escriben que lo son + su ID y el de su padre

	if(pid == 0)
	{
		printf("Soy el proceso HIJO de ID %d, mi padre tiene ID %d\n", getpid(), getppid());	
	}
	else
	{
		printf("Soy el proceso PADRE de ID %d, mi padre tiene ID %d\n", getpid(), getppid());
	}

	
	pid_t pid2 = fork();

	if(pid2 == 0)
	{
		printf("Soy el proceso HIJO de ID %d, mi padre tiene ID %d\n", getpid(), getppid());	
		execvp(argv[1], argv+1);
	}
	else
	{
		printf("Soy el proceso PADRE de ID %d, mi padre es %d\n", pid2, getppid());
	}

	while (waitpid(pid2, NULL, 0) != -1);
	if (pid != 0) while (waitpid(pid, NULL, 0) != -1);
	

	return 0;
}
