#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

/*programa que crea dos hijos: uno no cambia de ejecutable y otro si */


int main(int argc, char **argv)
{
	pid_t pid = 0;
	pid = fork();

	if(pid == 0){
		printf("Soy el hijo, mi identificador es %d, y el de mi padre es %d\n", getpid(), getppid());
	}else{
		printf("Soy el padre, mi identificador es %d, y el de mi padre es %d\n", getpid(), getppid());
	}

	pid_t pid2 = fork();

	if(pid2 == 0){
		printf("Soy el hijo, mi identificador es %d, y el de mi padre es %d\n", getpid(), getppid());
		execvp(argv[1], argv+1);
	}else{
		printf("Soy el padre, mi identificador es %d, y el de mi padre es %d\n", getpid(), getppid());
	}

	while(waitpid(pid2, NULL, 0)!=-1);
	if(pid !=0) while(waitpid(pid, NULL,0)!=-1);
	return 0;
}
