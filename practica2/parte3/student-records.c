#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "defs.h"
/*
    TO DO:
        Preguntar por queries encadenadas y varios argumentos con -c o -a
        
*/
//Forward declarations.
char* loadstr(FILE* students);

//Este metodo parsea las strings contenidas en el array records a structs de tipo student_t (definido en defs.h) y las devuelve en un array.
student_t* parse_records(char* records[], int nr_records){
    student_t* students = (student_t*) malloc(sizeof(student_t) * nr_records);
    for(int i = 0; i < nr_records; i++){
        students[i].id = atoi(strsep(&records[i], ":"));
        strcpy(students[i].NIF, strsep(&records[i], ":"));
        students[i].first_name = strsep(&records[i], ":");
        students[i].last_name = strsep(&records[i], ":");
    }
    return students;
}

//Este metodo recibe un array de student_t por el puntero entries y vuelca en binario el contenido al archivo students
int dump_entries(student_t* entries, int nr_entries, FILE* students){
    for(int i = 0; i < nr_entries; i++){
        fwrite(&entries[i].id, sizeof(int), 1, students);
        fwrite(entries[i].NIF, strlen(entries[i].NIF)+1, 1, students);
        fwrite(entries[i].first_name, strlen(entries[i].first_name)+1, 1, students);
        fwrite(entries[i].last_name, strlen(entries[i].last_name)+1, 1, students);
    }
    return 0;
}

//Este metodo devuelve un puntero a student_t donde se recogen las entradas leidas del archivo students. Ademas devuelve en el parametro
//nr_entries el numero de entradas recogidas.
student_t* read_student_file(FILE* students, int* nr_entries){
    fseek(students, 0, SEEK_SET);
    *nr_entries = 0;
    fread(nr_entries, sizeof(int), 1, students);
    student_t* studet_list = (student_t*) malloc(sizeof(student_t)*(*nr_entries));
    for(int i = 0; i < *nr_entries; i++){
        fread(&studet_list[i].id, sizeof(int), 1, students);
        strcpy(studet_list[i].NIF, loadstr(students));
        studet_list[i].first_name = loadstr(students); 
        studet_list[i].last_name = loadstr(students); 
       
    }
    return studet_list;
}

//Este metodo devuelve un puntero a una string null terminated del archivo students, empezando a leer desde donde este posicionado el marcador de posicion.
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
    fread(str, sizeof(char), len, students);
    return str;
}

/*Este metodo imprime nr_entries del array entries en pantalla, en el formato 

    [Entry #n]
        id=[ID]
        NIF=[NIF]
        first_name=[first_name]
        last_name=[last_name]
*/
void list_entries(student_t* entries, int nr_entries){
        int i;
        for(i = 0; i < nr_entries; i++){
            student_t student = entries[i];
            printf("[Entry #%d]\n\tstudent_id=%d\n\tNIF=%s\n\tfirst_name=%s\n\tlast_name=%s\n",
            i, student.id, student.NIF, student.first_name, student.last_name);
        }
}

//Este metodo filtra nr_args de entradas recibidas en el array list, segun nr_entries recibidas en el array students; volcandolas en el archivo file
//si no existen, e imprimiendo "Found duplicate student_id [id]", si la id esta repetida. Ademas devuelve el numero de entradas no repetidas en el puntero nr_new.
void filter(int nr_entries, int nr_args, int* nr_new, student_t* list, student_t* students, FILE* file){
    for(int i = 0; i< nr_args; i++){
        int found = 0;
        for(int j = 0; j < nr_entries; j++){
            if(list[i].id == students[j].id){
                found = 1;
                printf("Found duplicate student_id %d\n", list[i].id);
            }
        }
        if(found == 0){
            dump_entries(&list[i], 1, file);
            (*nr_new)++;
        }
    }
}

//Este metodo realiza una busqueda por id (tipo 1), o por NIF (tipo 2) en el array students, de tamaño nr_entries, segun el argumento arg.
//Devuelve el indice de dicha entrada, en caso de que no exista, devuelve -1.
int make_query(query_t type, student_t* students, int nr_entries, char* arg){
    int found = 0;
    int i = 0;
    int id;
    while(i < nr_entries && found == 0){
        switch(type){
            case Q_ID:
                id = atoi(arg);
                if(students[i].id == id){
                    found = 1;
                    return i;
                }
                break;
            case Q_NIF:
                if(strcmp(students[i].NIF, arg) == 0){
                    found = 1;
                    return i;
                }
                break;
            default:
                break;
        }
        ++i;
    }
    return -1;
}

char** cp_args(char** argv, int * nr_args){
    char p;
    int i = 0;
    do{
        p = argv[optind-1+i][0];
        if(p != '-') ++i;
    }while(i < (*nr_args) && p!='-');
    *nr_args = i;
    char ** buf = malloc(sizeof(char*) * (*nr_args));
    for(i = 0; i < (*nr_args); i++){
        buf[i] = strdup(argv[optind - 1 + i]);
    }
    return buf;
}

int main(int argc, char** argv){
    char c;
    FILE* file = NULL;
    int nr_entries;
    student_t* students = NULL;
    char pathname[10000];
    char** buf = NULL;
    int query = 0;
    char* queryarg = "";
    query_t querytype = Q_NONE;
    while((c = getopt(argc, argv, "hlqi:n:c:a:f:")) != -1){
        switch(c){
            case 'h':
                fprintf(stderr, "Usage: ./student-records -f file [ -h | -l | -c | -a | -q [ -i|-n ID] ]  [ list of records ]\n");
                break;
            case 'f':
                strcpy(pathname, optarg);
                break;
            case 'l':
                if((file = fopen(pathname, "rb")) == NULL){
                    return -1;
                }
                students = read_student_file(file, &nr_entries);
                list_entries(students, nr_entries);
                free(students);
                fclose(file);
                break;
            case 'c':
                if((file= fopen(pathname, "wb+")) == NULL){
                    return -1;
                }
                nr_entries = argc - optind + 1;
                buf = cp_args(argv, &nr_entries);
                students = parse_records(buf, nr_entries);
                printf("%d records written succesfully\n", nr_entries);
                fwrite(&nr_entries, sizeof(int), 1, file);
                dump_entries(students, nr_entries, file);
                fclose(file);
                free(students);
                free(buf);
                break;
            case 'a':
                if((file = fopen(pathname, "rb+")) == NULL){
                    return -1;
                }
                students = read_student_file(file, &nr_entries);
                int nr_args = argc - optind + 1;
                buf = cp_args(argv, &nr_args);
                student_t* list = parse_records(buf, nr_args);
                int nr_new = 0;
                filter(nr_entries, nr_args, &nr_new, list, students, file);
                nr_entries+=nr_new;
                fseek(file,0,SEEK_SET); 
                fwrite(&nr_entries, sizeof(int), 1, file);
                printf("%d extra records written\n", nr_new);
                fclose(file);
                free(students);
                free(buf);
                break;
            case 'q':
                query = 1;
                break;
            case 'i':
                querytype = Q_ID;
                queryarg = strdup(optarg);
                break;
            case 'n':
                querytype = Q_NIF;
                queryarg = strdup(optarg);
                break;
            default:
                break;
        }    
        if(query == 1 && querytype != Q_NONE){
            if((file = fopen(pathname, "rb")) == -1){
                return -1;
            }
            students = read_student_file(file, &nr_entries);
            int entry = make_query(querytype, students, nr_entries, queryarg);
            if(entry != -1){
                student_t student = students[entry];
                printf("[Entry #%d]\n\tstudent_id=%d\n\tNIF=%s\n\tfirst_name=%s\n\tlast_name=%s\n",
                entry, student.id, student.NIF, student.first_name, student.last_name);
                free(students);
                fclose(file);
            }else{
                printf("No entries found\n");
                free(students);
                fclose(file);
            }
            query = 0;
            querytype = 0;
        }           
    }
    return 0;
}