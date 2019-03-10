CC = gcc
EXEC = Picross.o

Picross.o:./source/main.o ./source/picross.o
	$(CC) -o $(EXEC) ./source/main.o ./source/picross.o -lncurses -lm
	./$(EXEC)

./source/main.o:./source/main.c ./header/picross.h
	$(CC) -o ./source/main.o -c ./source/main.c


./source/picross.o:./source/picross.c ./header/picross.h
	$(CC) -o ./source/picross.o -c ./source/picross.c

clean:
	rm -rf ./source/*.o

mrproper:clean
	rm -rf $(EXEC)

