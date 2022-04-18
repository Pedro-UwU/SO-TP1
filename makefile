tp1: slave
	gcc -o main -Wall -std=c99 -g -I ./include/ src/main.c src/error_handler.c src/shared_utils.c -pthread -lrt

slave:
	gcc -o slave -Wall -std=c99 -g -I ./include/ src/slave.c src/error_handler.c
