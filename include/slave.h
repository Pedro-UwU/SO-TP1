/* 72.11 - Sistemas Operativos - ITBA */

/*
Archivo slave.h: Configuraciones para slave.c. El archivo slave.c (ubicado en src)
contiene todo el código de los programas esclavos que se encargarán de lanzar MiniSAT.
El programa recibe como argumentos los archivos a procesar primero, y recibe luego
por stdin un archivo con el que continuar el procesamiento cuando termina con los iniciales.
*/

#ifndef SLAVE_H
#define SLAVE_H

#define MIN_FILES 1                    //Minimum amount of initial input files the slave program can take
#define MAX_FILES 2                    //Maximum amount of initial input files the slave program can take

#define EXEC_CMD "minisat"              //Command that will be executed
#define GREP_PARSE_CMD "grep -o -e \"Number of .*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\"" //Parser

#endif //SLAVE_H
