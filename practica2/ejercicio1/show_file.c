#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	int c,ret;

	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);


	while(fread(&c, sizeof(char), 1, file) > 0){
		
		ret = fwrite(&c, sizeof(char), 1 ,stdout);

		if(ret <= 0){
			fclose(file);
			err(3, "fread() failed");
		}
	
	}

	fclose(file);
	return 0;
}
