#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "defs.h"
/*
    TO DO:
    -c 
    -a
    -l
    -q
        -i
        -n


    PREGUNTAR POR LOS PUNTEROS DEL STRUCT
    parse_records
    dump_entries
    read_student_file
    loadstr
*/

student_t* parse_records(char* records[], int nr_records){
    student_t* students = malloc(sizeof(student_t) * nr_records);
    printf("%ld %ld\n", sizeof(student_t), sizeof(*students));
    for(int i = 0; i < nr_records; i++){
        students[i].id = atoi(strsep(&records[i], ":"));
        printf("primer strcpy\n");
        strcpy(students[i].NIF, strsep(&records[i], ":"));
        printf("NIF %s\n", students[i].NIF);
        strcpy(students[i].first_name, strsep(&records[i], ":"));
        printf("primer nom %s\n", students[i].first_name);
        strcpy(students[i].last_name, strsep(&records[i], ":"));
        printf("segundo nom %s\n", students[i].last_name);
    }
    return students;
}

int dump_entries(student_t* entries, int nr_entries, FILE* students){
    return 0;
}

student_t* read_student_file(FILE* students, int* nr_entries){
    fseek(students, 0, SEEK_SET);

    return NULL;
}

char* loadstr(FILE* students){
    return "\0";
}

int main(int argc, char** argv){
    char c;
    FILE* file = NULL;
    int nr_entries;
    student_t* students = NULL;
    while((c = getopt(argc, argv, "hl:q:i:n:c:a:l:f:")) != -1){
        switch(c){
            case 'h':
                fprintf(stderr, "Usage: ./student-records -f file [ -h | -l | -c | -a | -q [ -i|-n ID] ]  [ list of records ]\n");
                break;
            case 'f':
                if((file = fopen(optarg, "a+"))  == NULL){
                    return -1;
                }
                break;
            case 'l':
                if(file == NULL){
                    return -1;
                }
                students = read_student_file(file, &nr_entries);
                int i;
                for(i = 0; i < nr_entries; i++){
                    student_t s = students[i];
                    printf("[Entry #%d]\n\tstudent_id=%d\n\tNIF=%s\n\tfirst_name=%s\n\tlast_name=%s\n",
                            i, s.id, s.NIF, s.first_name, s.last_name);
                }
                break;
            case 'c':
                if(file == NULL){
                    return -1;
                }
                nr_entries = argc - optind + 1;
                students = parse_records(&argv[optind-1], nr_entries);
                dump_entries(students, nr_entries, file);
                break;
            default:
                break;
        }     
    }
    if(file!=NULL) fclose(file);
    if(students != NULL) free(students);
    return 0;
}