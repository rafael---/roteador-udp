FLAGS=-g
OBJ_DIR=../obj/
BIN_DIR=../bin/

all: link

obj:
	gcc -c send.c -o $(OBJ_DIR)send.o $(FLAGS)
	gcc -c status.c -o $(OBJ_DIR)status.o $(FLAGS)
	gcc -c threads.c -o $(OBJ_DIR)threads.o -lpthread $(FLAGS)
	gcc -c rec.c -o $(OBJ_DIR)rec.o $(FLAGS)
	gcc -c main.c -o $(OBJ_DIR)main.o $(FLAGS)
	gcc -c fila.c -o $(OBJ_DIR)fila.o $(FLAGS)

link: obj
	gcc $(OBJ_DIR)send.o $(OBJ_DIR)status.o $(OBJ_DIR)threads.o $(OBJ_DIR)rec.o \
		$(OBJ_DIR)fila.o $(OBJ_DIR)main.o -o $(BIN_DIR)server -lpthread $(FLAGS)

clean:
	rm $(OBJ_DIR)send.o $(OBJ_DIR)status.o $(OBJ_DIR)threads.o $(OBJ_DIR)rec.o \
		$(OBJ_DIR)fila.o $(OBJ_DIR)main.o
