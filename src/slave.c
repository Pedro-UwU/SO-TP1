/* 72.11 - Sistemas Operativos - ITBA */

/*
Archivos slave:
Contienen todo el código de los programas esclavos que se encargarán de lanzar MiniSAT.
El programa recibe como argumentos los dos archivos a procesar primero, y recibe luego
por stdin un archivo con el que continuar el procesamiento cada vez que termina.
*/

#include "../include/slave.h"
#include <stdio.h>

int main (int argc, char** argv){
    unsigned short initialFiles;
    switch (argc){
        case 1:
            return 1;
        case 2:
            if (argv[1] == NULL){
                return 1;
            }
            initialFiles = 1;
            break;
        case 3:
            if (argv[1] == NULL){
                return 1;
            }
            if (argv[2] == NULL){
                initialFiles = 1;
            }
            else{
                initialFiles = 2;
            }
            break;
        default:
            break;
    }
    
}
