#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>


/* Forward declaration */
int get_size_dir(char *fname, size_t *blocks);

/* Gets in the blocks buffer the size of file fname using lstat. If fname is a
 * directory get_size_dir is called to add the size of its contents.
 */
int get_size(char *fname, size_t *blocks)
{
	//printf("consiguiendo los datos de: %s", fname);
	struct stat s;
	if(lstat(fname, &s) == -1){
		perror("Error al conseguir los datos");
		return -1;
	}

	if(S_ISDIR(s.st_mode)){
		*blocks += s.st_blocks/2;
		if(get_size_dir(fname, blocks) == -1) return -1;
	}else{
		*blocks += s.st_blocks/2;
	}
	return 0;
}


/* Gets the total number of blocks occupied by all the files in a directory. If
 * a contained file is a directory a recursive call to get_size_dir is
 * performed. Entries . and .. are conveniently ignored.
 */
int get_size_dir(char *dname, size_t *blocks)
{
	//printf("recursive call to %s ",dname);
	DIR* dir;
	if((dir = opendir(dname)) == NULL){
		perror("Could not open dir");
		return -1;
	}
	struct dirent *nextfile;
	int i = 0;
	char thisDir[3];
	while((nextfile = readdir(dir)) != NULL){
		if(!((strcmp(nextfile->d_name, ".") == 0) || (strcmp(nextfile->d_name, "..") == 0))){
			char dest[10000];
			strcpy(dest, dname);
			strcat(dest, "/");
			strcat(dest, nextfile->d_name);
			if(nextfile->d_type == DT_DIR){				
				get_size(dest, blocks);
			}
			else{
				
				get_size(dest, blocks);
			}
		}
	}
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[])
{
	if(argc < 2){
		fprintf(stderr, "Usage: ./espacio [ files ]\n");
		return -1;
	}
	int i = 1;
	while(i < argc){
		size_t buffer = 0;
		if(get_size(argv[i], &buffer) == -1){
			return -1;
		}
		printf("%dK %s\n", (int)buffer, argv[i]);
		++i;
	}
	return 0;
}
