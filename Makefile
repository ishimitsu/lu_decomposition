CC = gcc

all:
	${CC} -c main.c
#	${CC} -c string.c
#	${CC} -o main main.o string.o
	${CC} -o lu_decompositon main.o
	chmod u+x lu_decompositon
	./lu_decompositon

clean:
	rm -f *.o
