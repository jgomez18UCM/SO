#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* programa que crea N hijos siguiendo un grado de dependencias. Usa las
 * versiones de execl y execlp */


int main(int argc, char **argv)
{
	pid_t pid1 = fork();
	
	if(pid1 == 0){
		execlp("echo", "echo", "P1", NULL);
	}

	while(waitpid(pid1, NULL, 0) !=-1);

	pid_t pid2 = fork();

	if(pid2 == 0){
		execl("/bin/echo", "/bin/echo", "P2", NULL);
	} 

	pid_t pid5 = fork();

	if(pid5 == 0){
		execlp("echo", "echo", "P5", NULL);
	} 

	pid_t pid7 = fork();

	if(pid7 == 0){
		execlp("echo", "echo", "P7", NULL);
	} 

	while(waitpid(pid2, NULL ,0) != -1);

	pid_t pid3 = fork();
	
	if(pid3 == 0){
		execlp("echo", "echo", "P3", NULL);
	} 

	pid_t pid4 = fork();

	if(pid4 == 0){
		execl("/bin/echo", "/bin/echo", "P4", NULL);
	} 

	while(waitpid(pid4, NULL, 0) != -1); 
	while(waitpid(pid5, NULL, 0) != -1);

	pid_t pid6 = fork();

	if(pid6 == 0){
		execl("/bin/echo", "/bin/echo", "P6", NULL);
	} 

	while (waitpid(pid6,NULL,0) != -1);
	while (waitpid(pid3,NULL,0) != -1);
	while (waitpid(pid7,NULL,0) != -1);

	pid_t pid8 = fork();

	if(pid8 == 0){
		execl("/bin/echo", "/bin/echo", "P8", NULL);
	} 

	while(waitpid(pid8, NULL, 0) != -1);

	return 0;
}
