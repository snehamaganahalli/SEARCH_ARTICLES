all: main.o lib.o hash.o search.o
	gcc -o exe main.c lib.c hash.c search.c -g
main.o:
	gcc -c main.c -g
lib.o:
	gcc -c lib.c -g
hash.o:
	gcc -c hash.c -g
search.o:
	gcc -c search.c -g

clean:
	rm -rf *.o
	rm -rf exe

