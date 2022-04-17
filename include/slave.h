/* 72.11 - Sistemas Operativos - ITBA */

/*
Archivo slave.h: Configuraciones para slave.c. El archivo slave.c (ubicado en src)
contiene todo el código de los programas esclavos que se encargarán de lanzar MiniSAT.
El programa recibe como argumentos los dos archivos a procesar primero, y recibe luego
por stdin un archivo con el que continuar el procesamiento cada vez que termina.
*/

#ifndef SLAVE_H
#define SLAVE_H

#define MIN_PARAMS 2                    //Minimum amount of command line parameters the slave program can take
#define MAX_PARAMS 3                    //Maximum amount of command line parameters the slave program can take

#define INSTANCES_AFTER_INITIAL 1       //Instances that will remain running after the initial files have been processed
                                        //If this constant is set to 0, the program will return after processing all initial files
                                        //If set to a value greater than (MAX_PARAMS - 1), the program will run the same amount
                                        //of instances it had when processing the initial files

#define EXEC_CMD "minisat"              //Command that will be executed
//#define GREP_PARSE_CMD "cat"
#define GREP_PARSE_CMD "grep -o -e \"Number of .*[0-9]+\" -e \"CPU time.*\" -e \".*SATISFIABLE\""

#endif //SLAVE_H
