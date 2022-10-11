#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "defs.h"
/*
    TO DO:
    -a
    -q
        -i
        -n
*/
char* loadstr(FILE* students);

student_t* parse_records(char* records[], int nr_records){
    student_t* students = malloc(sizeof(student_t) * nr_records);
    for(int i = 0; i < nr_records; i++){
        students[i].id = atoi(strsep(&records[i], ":"));
        strcpy(students[i].NIF, strsep(&records[i], ":"));
        students[i].first_name = strsep(&records[i], ":");
        students[i].last_name = strsep(&records[i], ":");
    }
    return students;
}

int dump_entries(student_t* entries, int nr_entries, FILE* students){
    for(int i = 0; i < nr_entries; i++){
        fwrite(&entries[i].id, sizeof(int), 1, students);
        fwrite(&entries[i].NIF, strlen(entries[i].NIF)+1, 1, students);
        fwrite(entries[i].first_name, strlen(entries[i].first_name)+1, 1, students);
        fwrite(entries[i].last_name, strlen(entries[i].last_name)+1, 1, students);
    }
    return 0;
}

student_t* read_student_file(FILE* students, int* nr_entries){
    fseek(students, 0, SEEK_SET);
    fread(nr_entries, sizeof(int), 1, students);
    student_t* studet_list = malloc(sizeof(student_t)*(*nr_entries));
    for(int i = 0; i < *nr_entries; i++){
        fread(&studet_list[i].id, sizeof(int), 1, students);
        
        fread(studet_list[i].NIF, sizeof(char), MAX_CHARACTERS_NIF+1, students);
        
        studet_list[i].first_name = loadstr(students); 
        studet_list[i].last_name = loadstr(students); 
       
    }
    return studet_list;
}

char* loadstr(FILE* students){
    char c;
    int len = 1;
    fread(&c, sizeof(char), 1, students); 
    while(c!='\0'){
        ++len;
        fread(&c, sizeof(char), 1, students);
    }
    char * str = malloc(sizeof(char)*len);
    fseek(students, -len, SEEK_CUR);
    int i;
    for(i = 0; i < len; i++) fread(&str[i], sizeof(char), 1, students);
    return str;
}

int main(int argc, char** argv){
    char c;
    FILE* file = NULL;
    int nr_entries;
    student_t* students = malloc(sizeof(student_t)*argc);
    char* pathname;
    char** buf;
    while((c = getopt(argc, argv, "q:i:n:c:a:f:hl")) != -1){
        switch(c){
            case 'h':
                fprintf(stderr, "Usage: ./student-records -f file [ -h | -l | -c | -a | -q [ -i|-n ID] ]  [ list of records ]\n");
                break;
            case 'f':
                pathname = optarg;
                break;
            case 'l':
                if((file = fopen(pathname, "rb")) == NULL){
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
                if((file= fopen(pathname, "wb+")) == NULL){
                    return -1;
                }
                nr_entries = argc - optind + 1;
                buf = malloc(sizeof(char*) * nr_entries); 
                for(int i = 0; i < nr_entries; i++){
                    buf[i] = argv[optind-1+i];
                }
                students = parse_records(buf, nr_entries);
                free(buf);
                printf("%d records written succesfully\n", nr_entries);
                fwrite(&nr_entries, sizeof(int), 1, file);
                dump_entries(students, nr_entries, file);
                break;
            case 'a':
                if((file = fopen(pathname, "ab+")) == NULL){
                    return -1;
                }
                students = read_student_file(file, &nr_entries);
                int nr_args = argc - optind + 1;
                buf = malloc(sizeof(char*) * nr_args);
                for(int i = 0; i < nr_args; i++){
                    buf[i] = argv[optind - 1 + i];
                }
                break;
            default:
                break;
        }     
    }
    
    if(file!=NULL) fclose(file);
    
    if(students != NULL) free(students);
   
    return 0;
}