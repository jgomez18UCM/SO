#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void){

    int fd = -1; 
    
    if((fd = open("/dev/chardev_leds", O_WRONLY)) == -1){
        printf("Modulo no encontrado, asegurate de que está instalado");
        return -1;
    }
    
    int palanca = -1;
    int leds[3] = {0, 1, 0};
    int todosEncendidos = 0;
    char str[4] = "";
    write(fd, "2", 1);

    while(!todosEncendidos)
    {
        scanf("%d", &palanca);
        strcpy(str, "");
        switch(palanca){
            case 1: 
                leds[2] += 1; leds[2] %= 2;
                leds[1] += 1; leds[1] %= 2;
                
            break;  
            case 2: 
                leds[0] += 1; leds[0] %= 2;
                leds[2] += 1; leds[2] %= 2;
            break;
            case 3: 
                leds[0] += 1; leds[0] %= 2;
                leds[1] += 1; leds[1] %= 2;
                break;
            default:
                printf("Ese input no es válido\n");
            continue;
        }
       
        if(leds[0]) strcat(str, "1");
        if(leds[1]) strcat(str, "2");
        if(leds[2]) strcat(str, "3");
        
        todosEncendidos = leds[0] && leds[1] && leds[2];
        write(fd, str, strlen(str));

    }
    close(fd);
    return 0;
}