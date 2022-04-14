/* 72.11 - Sistemas Operativos - ITBA */

/*
Archivo slave.c:
Contiene todo el código de los programas esclavos que se encargarán de lanzar MiniSAT.
El programa recibe como argumentos los dos archivos a procesar primero, y recibe luego
por stdin un archivo con el que continuar el procesamiento cada vez que termina.
*/

//INCLUDE HEADER FILES AND STANDARD LIBRARIES
#include "../include/slave.h"   //Configuration constants
#include <stdio.h>
#include <string.h>             //strcmp, strcat, etc.
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//CONSTANT DEFINITIONS USING #define
#define MAX_INITIAL_FILES MAX_PARAMS - 1
#define PIPE_READ 0             //Read end of pipes
#define PIPE_WRITE 1            //Write end of pipes
#define STDIN 0                 //Stdin file descriptor
#define STDOUT 1                //Stdout file descriptor
#define CMD_MAX_LEN 750

//FUNCTION HEADERS:
short processFile(char* filePath, pid_t* childPID, int* readFD);                    //This function executes the program that does the processing of the file

int main (int argc, char** argv){
    if (argc < MIN_PARAMS || argc > MAX_PARAMS){                                    //This program receives up to 2 filePaths
        return EXIT_FAILURE;
    }
    unsigned short initialFiles = argc - 1, i, processing = 0;
    #if MAX_PARAMS == 3
    if (initialFiles == MAX_INITIAL_FILES && strcmp(argv[1], argv[2]) == 0){        //Avoid processing the same file twice. This algorithm only works for 2 files
        initialFiles--;
    }
    #endif

    pid_t pidArray[initialFiles];                                                   //Execute processing program. Its PID and the read end of its pipe will be stored in the arrays
    int readFDArray[initialFiles];
    for (i = 0; i < initialFiles; i++){
        if(processFile(argv[i + 1], &(pidArray[i]), &(readFDArray[i])) != 0){       //If processFile does not return 0, an error occurred
            return EXIT_FAILURE;
        }
        processing++;
    }
    /* ! */ //HASTA ACA ESTA TODO LISTO
    while (processing > 0){
        wait(NULL);
        processing--;
        /* ! */
    }
    return EXIT_SUCCESS;
}

short processFile(char* filePath, pid_t* childPID, int* readFD){
    int fd[2];                                                                      //File descriptor array for a pipe
    if (pipe(fd) == -1){                                                            //Create pipe for IPC usage
        return 1;
    }
    pid_t pid = fork();                                                             //Create a child process which will process the file

    //CHILD PROCESS ONLY
    if (pid == 0){
        char command[CMD_MAX_LEN];                                                  //Command the shell has to execute
        snprintf(command, CMD_MAX_LEN, "%s %s | %s", EXEC_CMD, filePath, GREP_PARSE_CMD);
        close(fd[PIPE_READ]);                                                       //Close unused file descriptor
        dup2(fd[PIPE_WRITE], STDOUT);                                               //Makes the processing program print to the pipe instead of stdout
        close(fd[PIPE_WRITE]);                                                      //Close unused file descriptor (from now on, only stdout will be used by the child process)
        execlp("/bin/sh" /*Exec*/, "/bin/sh" /*First arg is executable name*/,      //The child executes the processing program
            "-c" /*Read command from argument string*/,  command /*CMD to be executed*/,
            (char*) NULL /*NULL terminator*/);
    }                                                                               //Nothing is executed beyond the exec instruction in the child process

    //PARENT PROCESS ONLY
    close(fd[PIPE_WRITE]);                                                          //Close unused file descriptor
    *childPID = pid;                                                                //Return the pid and fd of the child process
    *readFD = fd[PIPE_READ];
    return 0;                                                                       //No errors occurred
}

//FILE* writeEnd = fdopen(fd[PIPE_WRITE], "w");

/*ESTADO:
processFile listo
TODO:
    * Recuperar la respuesta de los minisat | grep (estan en el arreglo de file descriptors y se tiene el PID de cada subproceso en el arreglo)
    * Avisar a main cuando se termino un archivo
        *Codigo para que siempre se mantenga con la cantidad maxima de minisats trabajando al mismo tiempo (tiene que pedir un nuevo archivo cada vez e iniciar un nuevo processFile, no puede ser bloqueante)
*/
