ex: main.o action.o mainTable.o symbol.o
	gcc -g -ansi -Wall -pedantic main.o action.o mainTable.o symbol.o -o ex
main.o: main.c main.h
	gcc -g -ansi -Wall -pedantic main.c -c main.c
mainTable.o: mainTable.c main.h
	gcc -g -ansi -Wall -pedantic mainTable.c -c mainTable.c
action.o: action.c main.h
	gcc -g -ansi -Wall -pedantic action.c -c action.c
symbol.o: symbol.c main.h
	gcc -g -ansi -Wall -pedantic symbol.c -c symbol.c
