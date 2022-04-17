/*************************************
* 72.11 - Sistemas Operativos - ITBA *
* Integrantes del grupo:             *
*   Causse, Juan Ignacio (61105)     *
*   Lopez, Pedro (60711)             *
*   Vasquez, Malena (60072)          *
*************************************/

//INCLUDE HEADER FILES:
#include <stdio.h>
#include <main.h>
#include <error_handler.h>
#include <utils.h>
#include <unistd.h>

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

	int total_slaves = min(config.total_jobs, MAX_SLAVES);

	// Create the slaves Array and then initilize them

	slave slaves[total_slaves];
	init_slaves(slaves, total_slaves, (char**)(argv+1), &config);


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
