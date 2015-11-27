FLAGS=-Wall -pedantic -O2
OBJ_DIR=obj/
BIN_DIR=bin/
SRC_DIR=src/

all: link

objects:
	gcc -c $(SRC_DIR)send.c -o $(OBJ_DIR)send.o $(FLAGS)
	gcc -c $(SRC_DIR)status.c -o $(OBJ_DIR)status.o $(FLAGS)
	gcc -c $(SRC_DIR)threads.c -o $(OBJ_DIR)threads.o -lpthread $(FLAGS)
	gcc -c $(SRC_DIR)rec.c -o $(OBJ_DIR)rec.o $(FLAGS)
	gcc -c $(SRC_DIR)fila.c -o $(OBJ_DIR)fila.o $(FLAGS)
	gcc -c $(SRC_DIR)main.c -o $(OBJ_DIR)main.o $(FLAGS)

link: objects
	gcc $(OBJ_DIR)send.o $(OBJ_DIR)status.o $(OBJ_DIR)threads.o $(OBJ_DIR)rec.o \
		$(OBJ_DIR)fila.o $(OBJ_DIR)main.o -o $(BIN_DIR)server -lpthread $(FLAGS)

clean:
	rm $(OBJ_DIR)send.o $(OBJ_DIR)status.o $(OBJ_DIR)threads.o $(OBJ_DIR)rec.o \
		$(OBJ_DIR)fila.o $(OBJ_DIR)main.o
