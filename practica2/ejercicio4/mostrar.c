#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



typedef struct {
	int n;
	int e;
} args_t;

int show(char* pathname, args_t * args){
	int file;
	char buff[1];
	int copied;
	int start = SEEK_SET;
	

	if((file = open(pathname, O_RDONLY)) == -1){
		err(1, "The input file %s could not be opened\n", pathname);
		return -1;
	}

	if(args->e == 1) {
		start = SEEK_END;
		args->n = -args->n;
	}
	
	lseek(file, args->n, start);
	

	while((copied = read(file, buff, sizeof(char))) > 0){
		printf("%s", buff); 
	}

	if(copied == -1){
		perror("error al enseñar");
		return -1;
	}

	close(file);
	return 0;
	
}
int main(int argc, char *argv[])
{
	char c;
	args_t arg = {0, 0};

	if(argc < 2){
		fprintf(stderr, "Usage %s [-n N [-e]] <filename>\n", argv[0]);
		return -1;	
	}
	while((c = getopt(argc, argv, "n:e"))!=-1){
		switch(c){
			case 'n':
				sscanf(optarg, "%d", &arg.n);
				break;
			case 'e':
				arg.e = 1;
				break;
			default:
				break;
		}
	}

	if(show(argv[optind], &arg) == -1){
		return -1;
	}

	return 0;
}
