all: main.o lib.o hash.o search.o
	gcc -o exe main.c lib.c hash.c search.c
main.o:
	gcc -c main.c
lib.o:
	gcc -c lib.c
hash.o:
	gcc -c hash.c
search.o:
	gcc -c search.c

clean:
	rm -rf *.o
	rm -rf exe

