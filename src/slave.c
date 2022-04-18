/* 72.11 - Sistemas Operativos - ITBA */

/*
Archivo slave.c:
Contiene todo el código de los programas esclavos que se encargarán de lanzar MiniSAT.
El programa recibe como argumentos los archivos a procesar primero, y recibe luego
por stdin un archivo con el que continuar el procesamiento cuando termina con los iniciales.
*/

//INCLUDE HEADER FILES AND STANDARD LIBRARIES
#include "../include/slave.h"   //Configuration constants and dependencies

//CONSTANT DEFINITIONS USING #define
#define MAX_INITIAL_FILES MAX_PARAMS - 1
#define PIPE_READ 0             //Read end of pipes
#define PIPE_WRITE 1            //Write end of pipes
#define STDIN 0                 //Stdin file descriptor
#define STDOUT 1                //Stdout file descriptor
#define CMD_MAX_LEN 750
#define BUFF_MAX_LEN 65536      //Output read buffer maximum length
#define PATH_MAX_LEN 4096       //File path maximum length

#define MIN_PARAMS MIN_FILES + 1    //Minimum amount of command line parameters the slave program can take
#if MIN_PARAMS < 2
#undef MIN_PARAMS
#define MIN_PARAMS 2
#endif

#define MAX_PARAMS MAX_FILES + 1    //Maximum amount of command line parameters the slave program can take
#if MAX_PARAMS < MIN_PARAMS
#undef MAX_PARAMS
#define MAX_PARAMS MIN_PARAMS
#endif

//MACRO DEFINITIONS:
#define CLEAR_BUFFER((_buff_), (_len_)) { int macro_buffer_iterator = 0; for (; macro_buffer_iterator < (_len_); macro_buffer_iterator++){ (_buff_)[macro_buffer_iterator] = 0; } }

//FUNCTION HEADERS:
short processFile(char* filePath, pid_t* childPID, int* readFD);                                //This function executes the program that does the processing of the file
int getFileDescriptor(pid_t pid, const pid_t* pidArray, const int* readFDArray, int dim);       //This function searches for a PID and returns its corresponding file descriptor, or -1 if the PID could not be found
void output(int fd);                                                                            //This function receives a file descriptor and outputs its stream to stdout with a defined format. Closes the file descriptor

int main (int argc, char** argv){
    //VALIDATE PARAMETERS
    if (argc < MIN_PARAMS || argc > MAX_PARAMS){                                    //This program receives up to 2 filePaths
        exit_error("Incorrect parameter amount.");
    }
    unsigned short initialFiles = argc - 1, i, processing = 0;
    #if MAX_PARAMS == 3
    if (initialFiles == MAX_INITIAL_FILES && strcmp(argv[1], argv[2]) == 0){        //Avoid processing the same file twice. This algorithm only works for 2 files
        initialFiles--;
    }
    #endif
    //START PROCESSING INITIAL FILES
    pid_t pidArray[initialFiles];                                                   //Execute processing program. Its PID and the read end of its pipe will be stored in the arrays
    int readFDArray[initialFiles];
    for (i = 0; i < initialFiles; i++){
        if (processFile(argv[i + 1], &(pidArray[i]), &(readFDArray[i])) != 0){       //If processFile does not return 0, an error occurred
            exit_error("Error creating pipe.");
        }
        processing++;
    }
    //WHEN READY, RETURN RESPONSES TO MAIN PROGRAM
    pid_t child;                                                                    //This variable will store the PID of a child process that has ended
    int fd;                                                                         //And the read-end file descriptor of the pipe that communicates the program with it
    while (processing > 0){
        child = wait(NULL);                                                         //Wait for any child process to end
        fd = getFileDescriptor(child, pidArray, readFDArray, initialFiles);
        if (fd == -1){
            exit_error("File Descriptor not found.");
        }
        output(fd);
        processing--;
    }
    //AFTER PROCESSING THE INITIAL FILES, ASK FOR A NEW FILE USING STDIN AND PROCESS IT.
    char path[PATH_MAX_LEN] = {0};
    scanf("%s", path);                                                              //The file path (or the terminate command) will be received via stdin
    while (strcmp(path, TERMINATE_EXECUTION_CMD) != 0){                             //The program will end when the TERMINATE_EXECUTION_CMD is received
        if(processFile(path, &child, &fd) != 0){
            exit_error("Error creating pipe.");
        }
        waitpid(child, NULL, 0);
        output(fd);
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

int getFileDescriptor(pid_t pid, const pid_t* pidArray, const int* readFDArray, int dim){
    int i;
    for (i = 0; i < dim; i++){
        if (pid == pidArray[i]){
            return readFDArray[i];
        }
    }
    return -1;
}

void output(int fd){
    //READ FROM FILE DESCRIPTOR
    static char buff[BUFF_MAX_LEN] = {0};
    read(fd, buff, BUFF_MAX_LEN);
    close(fd);                                                                      //After retrieving the returned information, the fd is no longer needed
    //FORMAT OUTPUT STRING
    int i = 0, j = 0;
    while (buff[j] != '\0'){                                                        //Delete unnecesary spaces
        if (buff[j] != ' ' || buff[j + 1] != ' '){
            buff[i] = buff[j];
            i++;
        }
        j++;
    }
    buff[i] = '\0';                                                                 //Put back the null terminator. If any non-zero chars are left after it, they will be set back to 0 anyway when CLEAR_BUFFER is called
    puts(buff); //FORMATO
    CLEAR_BUFFER(buff, BUFF_MAX_LEN)
}
//TODO: FORMATO LINEA 139
