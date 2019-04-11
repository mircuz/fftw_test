all: compile link

compile:
	mpicc -O2 -fpic -c rft.c
	mpicc -O2 -fpic -c transpose.c
	mpicc -O2 -fpic -c 2drft.c

link:
	mpicc -o rft rft.o -lfftw3_mpi -lfftw3 -lm
	mpicc -o 2drft 2drft.o -lfftw3_mpi -lfftw3 -lm
	mpicc -o transpose transpose.o -lfftw3_mpi -lfftw3 -lm
	make remove_obj

remove_obj:
	rm *.o
	
clean:
	rm rft
	rm transpose
	rm 2drft
