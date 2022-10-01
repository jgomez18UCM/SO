#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>

void copy(int fdo, int fdd)
{

}

void copy_regular(int fdo, int fdd)
{
	char buf[512];
	ssize_t n;
	do {
		n = read ( fdo ,buf ,1) ;
		if (n > 0)
			n = write ( fdd , buf , 1) ;
	} while ( n > 0) ;

	close(fdo);
	close(fdd);
	if (n < 0) {
		perror ( " Error en la copia " );
		return ;
	}

}

void copy_link(char *orig, char *dest)
{
	struct stat s;
	lstat(orig, &s);
	ssize_t size = s.st_size + 1;
	char * buf = malloc(size * sizeof(char));
	int n;
	n = readlink(orig, buf, size-1);
	if(n==-1){
		perror("lectura del link fallida");
		exit(3);
	}
	buf[size-1] = '\0';
	n = symlink(buf, dest);
	if(n == -1){
		perror("copia del link fallida");
		exit(3);
	}
}

int main(int argc, char *argv[])
{
	int fdo, fdd;
	if((fdo = open(argv[1], O_RDONLY)) == -1){
		perror("No se pudo abrir el archivo de origen");
		return -1;
	}
	

	struct stat s;
	if(lstat(argv[1],&s) == -1){
		perror("lstat");
		return -1;
	};
	if(S_ISLNK(s.st_mode)){
		copy_link(argv[1],argv[2]);
		close(fdo);
	}else if(S_ISREG(s.st_mode)){
		if((fdd = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, 0660)) == -1){
			perror("No se pudo abrir el archivo de destino");
			close(fdo);
			return -1;
		}
		copy_regular(fdo, fdd);
	}else{
		err(3, "Archivo no valido");
		return -1;
	}

	return 0;
}
	

