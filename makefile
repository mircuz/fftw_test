all: compile link

compile:
	mpicc -O2 -fpic -c rft.c
	mpicc -O2 -fpic -c transpose.c

link:
	mpicc -o rft rft.o -lfftw3_mpi -lfftw3 -lm
	mpicc -o transpose transpose.o -lfftw3_mpi -lfftw3 -lm
	make remove_obj

remove_obj:
	rm *.o
	
clean:
	rm rft
	rm transpose
