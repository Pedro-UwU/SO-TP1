/*************************************
* 72.11 - Sistemas Operativos - ITBA *
* Integrantes del grupo:             *
*   Causse, Juan Ignacio (61105)     *
*   Lopez, Pedro (60711)             *
*   Vasquez, Malena (TODO_LEGAJO)    *
*************************************/

//INCLUDE HEADER FILES:
#include <stdio.h>
#include <main.h>
#include <error_handler.h>
#include <utils.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>

#define MAX_BUFF_SIZE 65536
#define MAX_CHILD_BUFF_SIZE 1024
 

//MACRO AND CONSTANT DEFINITIONS:

int main(int argc, char** argv){
	
	//Check Arguments
	if (argc == 1) {
		exit_error("ERROR: At least 1 argument must be given");
	}

	master_conf config;
	config.total_jobs = argc - 1; // Total files minus the executable argument
	config.assigned_jobs = 0;
	config.done_jobs = 0;

	// Initialize file to write the results
	FILE * output_file = fopen(OUTPUT_FILE, "w");
	if (output_file == NULL) {
		exit_error("ERROR opening output file");
	}
	
	/* temp */
	char buffer[MAX_BUFF_SIZE] = {0};
	int buffer_index = 0;

	int total_slaves = min(config.total_jobs, MAX_SLAVES);

	// Create the slaves Array and then initilize them

	slave slaves[total_slaves];
	init_slaves(slaves, total_slaves, (char**)(argv+1), &config);
	
	start_executing(slaves, total_slaves, buffer, &buffer_index, (char**)(argv+1), &config);
	
	printf("%s\n", buffer);
	return 0;
}

void init_slaves(slave * slaves, int total_slaves, char * files[], master_conf * conf) {
	//For each slave:
	// Create Pipes
	// dup2 each pipe
	// Fork anc exec each slave
	// Save each slave pid, pipe in and pipe out in the array
	for (int i = 0; i < total_slaves; i++) {
		int input[2]; //Pipe for passing the paths
		int output[2]; //Pipe for receiving the ouputs
		if (pipe(input) == -1) {
			exit_error("ERROR: Creating input pipe");
		}
		if (pipe(output) == -1) {
			exit_error("ERROR: Creating output pipe");
		}
		
		int pid = fork();
		switch (pid)
		{
		case -1:
			exit_error("ERROR: Forking");
		case 0: // Child
			//Close the unncesary ends
			if (close(input[FD_WRITE]) == -1) {
				exit_error("ERROR: Closing input fd write end");
			}
			if (close(output[FD_READ]) == -1) {
				exit_error("ERROR: Closing output fd read end");
			}

			//Dup to use stdin and stdout
			if (dup2(input[FD_READ], STDIN) == -1) {
				exit_error("ERROR: Dup2 input file descriptor");
			}
			if (dup2(output[FD_WRITE], STDOUT) == -1) {
				exit_error("ERROR: Dup2 output file descriptor");
			}

			//Close the unused file descriptors
			if (close(input[FD_READ]) == -1) {
				exit_error("ERROR: Closing input file descripor read end");
			}
			if (close(output[FD_WRITE]) == -1) {
				exit_error("ERROR: Closing output file descriptor write end");
			}

			//Create the execv params
			char * params[INITIAL_FILES + 2];
			params[0] = SLAVE_COMMAND;
			for (int j = 0; j < INITIAL_FILES; j++) {
				params[j+1] = files[conf->assigned_jobs];
				conf->assigned_jobs++;
			}
			params[INITIAL_FILES + 1] = NULL;
			execv(params[0], params);
			exit_error("ERROR: Execv");
		default:
			slaves[i].pid = pid;
			slaves[i].stdin_fd = input[FD_WRITE];
			slaves[i].stdout_fd = output[FD_READ];

			if (close(input[FD_READ]) == -1) {
				exit_error("ERROR: Closing input read");
			}
			if (close(output[FD_WRITE]) == -1) {
				exit_error("ERROR: Closing output write");
			}

			//Add INITIAL_FILES because it didn't change in the parent
			conf->assigned_jobs += INITIAL_FILES;
		}
	}
}

void start_executing(slave slaves[], int total_slaves, char * buffer, int * buffer_index, char * files[], master_conf * conf) {
	//Mientras queden files por procesar (done < total)
	//Creo un fd set y lo lleno con los fd
	//Hago un select
	// Por cada fd listo, hago una lectura del fd y lo guardo en el buffer
	// Luego de leer, me fijo si hay mas tareas para el slave, si hay, le paso por stdin otro archivo
	// Sino, le paso null de parametro y lo mato, y cierro el fd
	if (conf == NULL) {
		exit_error("ERROR: Conf is null");
	}
	while(conf->done_jobs < conf->total_jobs) {
		fd_set set;
		FD_ZERO(&set);
		int max_fd = 0;
		for (int i = 0; i < total_slaves; i++) {
			if (max_fd < slaves[i].stdout_fd) {
				max_fd = slaves[i].stdout_fd;
			}
			FD_SET(slaves[i].stdout_fd, &set);
		}

		/* To delete
		struct timeval tv;
		tv.tv_sec = 3;
		tv.tv_usec = 0;
		*/

		// printf("LLegue aca 1\n");
		int ready = select(max_fd + 1, &set, NULL, NULL, NULL);
		printf("LLegue aca 2, select = %d\n", ready);
		if (ready == 0) continue;
		for (int i = 0; i < total_slaves; i++) {
			if (FD_ISSET(slaves[i].stdout_fd, &set)) {
				//Leo
				char buff[MAX_CHILD_BUFF_SIZE] = {0};
				int dim = read(slaves[i].stdout_fd, buff, MAX_CHILD_BUFF_SIZE);
				if (dim > 0) conf->done_jobs++;
				printf("buffer: %s\n", buff);
				strcpy((buffer + *(buffer_index)), buff);
				*(buffer_index) += dim;

				//Write
				if (conf->assigned_jobs < conf->total_jobs) {
					write(slaves[i].stdin_fd, files[conf->assigned_jobs], strlen(files[conf->assigned_jobs]) + 1);
					conf->assigned_jobs++;
				} else {
					write(slaves[i].stdin_fd, NULL, 0);
				}
			} 
		}
	}
}	
