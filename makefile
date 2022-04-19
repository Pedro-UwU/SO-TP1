tp1: slave view
	gcc -o main -Wall -std=c99 -g -I ./include/ src/main.c src/error_handler.c src/shared_utils.c -pthread -lrt

view:
	gcc -o view -Wall -std=c99 -g -I ./include/ src/view.c src/error_handler.c src/shared_utils.c -pthread -lrt

slave:
	gcc -o dummy_slave -Wall -std=c99 -g -I ./include/ src/dummy_slave.c src/error_handler.c
