#include "defs.h"
#include <string.h>
#include <stdlib.h>

/*

TODO:
-c


-l
-h
-a
-q

*/




student_t * parse_records(char * records[], int nrecords)
{
    student_t something[nrecords];

    for (int i = 0; i < nrecords ; i++){
        something[i].student_id =  atoi(strsep(records[i], ':'));
        strcpy(something[i].NIF, strsep(records[i], ':'));
        strcpy(something[i].first_name, strsep(records[i], ':'));
        strcpy(something[i].last_name, strsep(records[i], ':'));
    }

    return something;
}





int main(int argc, char argv[]){

    //abrir el archivo del param -f

    //switch de opciones varias

    //cerrar el archivo del param -f
    return 0;
}