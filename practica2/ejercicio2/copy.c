#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <err.h>



void copy(int fdo, int fdd)
{
	char buf[512];
	size_t n;
	do {
		n = read ( fdo ,buf ,512) ;
		if (n > 0)
			n = write ( fdd , buf , n) ;
	} while ( n > 0) ;

	close ( fdo );
	close ( fdd );
	if (n < 0) {
		perror ( " Error en la copia " );
		return ;
	}
}

int main(int argc, char *argv[])
{
	int fdo = open(argv[1], O_RDONLY);
	int fdd = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, 0660);
	if(fdo == -1){
		perror("cannot open origin");
		return -1;
	}
	if(fdd == -1){
		perror("cannot open destiny");
		close(fdo);
		return -1;
	}

	copy(fdo,fdd);
	return 0;
}
