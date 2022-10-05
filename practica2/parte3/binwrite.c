#include "defs.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


/*

TODO:

-c


limpia lo que hubiera en el fichero
por cada entrada de alumno:
    parsea el input
    lo añade al array de la db del programa

escribe al archivo:
    el numero de entradas
    todas las entradas


-l


parsea el archivo y puebla la db
por cada entrada de la db:
    imprime con formato cada entrada al output

-h

imprime el uso del programa


-a

por cada entrada de alumno:
    parsea el input
    busca si existe en la db
    si no: añade a la db



-q

dependiendo del param:
    -i
        busca si existe alguien con id = param
        si existe: printea esa cosa
    -n
        busca si existe alguien con nif = param
        si existe: printea esa cosa


independientemente de lo que tenga que hacer:
-f

abre el archivo optarg
parsea las nelems entradas a db.

(hace lo que tenga que hacer)

reescribe las entradas de db en el archivo
cierra el archivo
*/

/*
PREGUNTAS AL POFE

student_t es un puntero opaco al struct, cómo sacamos su tamaño? ¿cuál es su tamaño siquiera?


*/




student_t * parse_records(char * records[], int nrecords)
{
    student_t something[nrecords] = malloc(sizeof(student_t) * nrecords);

    for (int i = 0; i < nrecords ; i++){
        something[i].student_id =  atoi(strsep(records[i], ':'));
        strcpy(something[i].NIF, strsep(records[i], ':'));
        strcpy(something[i].first_name, strsep(records[i], ':'));
        strcpy(something[i].last_name, strsep(records[i], ':'));
    }

    return something;
}



int dump_entries(student_t* entries, int nr_entries, FILE* students)
{

}



student_t* read_student_file(FILE* students, int* nr_entries) 
{

}



char* loadstr(FILE* students)
{

}


//argc - optint + 1

int main(int argc, char argv[]){

    //abrir el archivo del param -f

    //switch de opciones varias

    //cerrar el archivo del param -f

    printf("%d", sizeof(student_t));

    return 0;
}