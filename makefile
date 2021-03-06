CC=mpicc
CLIBS=-lfftw3_mpi -lfftw3 -lm
CFLAGS=-O2 -fpic $(CLIBS)

all: rft transpose many_transpose 2drft

rft.o: rft.c
transpose.o: transpose.c
many_transpose: many_transpose.c
2drft.o: 2drft.c

rft: rft.o 
	$(CC) rft.o $(CLIBS) -o rft

transpose: transpose.o
	$(CC) -o transpose transpose.o $(CLIBS)

2drft: 2drft.o
	$(CC) -o 2drft 2drft.o $(CLIBS)

many_transpose: many_transpose.o
	$(CC) -o many_transpose many_transpose.o $(CLIBS)

remove_obj:
	rm *.o
	
clean:
	make remove_obj
	rm rft
	rm transpose
	rm 2drft
	rm many_transpose
