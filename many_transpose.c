#include <stdlib.h>
#include "mpi.h"
#include "fftw3-mpi.h"

#define N 8
int main(int argc, char **argv) {

	MPI_Init(&argc,&argv);
	fftw_mpi_init();
	int rank,size,i,k;
	int nx=N/2+1, nz=N+1;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	/*Declare var*/
	fftw_plan plan, transpose;
	ptrdiff_t alloc_local, local_x, local_x_start,local_z, local_z_start, block_x, block_z;
	ptrdiff_t n[2];		n[0]=nx;	n[1]=nz;
	double *trasp;
	block_x= (ptrdiff_t) 2;	block_z= (ptrdiff_t) 3;		/*The blocks MUST be as big as the array or wider, NOT LESS!!*/ 
	int des_rank=0;	/*Set the investigated processor from which print data*/ 
	alloc_local = fftw_mpi_local_size_many_transposed(2, &n, 2,
                                              block_x, block_z, MPI_COMM_WORLD,
                                              &local_x, &local_x_start,
                                              &local_z, &local_z_start);

	printf("[%d]\tlocal_x= %d\tlocal_x_start= %d\n\tlocal_z= %d\tlocal_z_start= %d\n", rank, local_x, local_x_start, local_z, local_z_start);
	MPI_Barrier(MPI_COMM_WORLD);
	int count=1;
	trasp = fftw_alloc_real (2*alloc_local);
	transpose=fftw_mpi_plan_many_transpose(nx, nz, 2, block_x, block_z, trasp, trasp,
                                  MPI_COMM_WORLD, FFTW_MPI_TRANSPOSED_OUT);
	trasp[0]= local_x_start*nz*2;
	for (i=local_x_start; i < local_x_start+local_x; i++){
		for(k=0; k<nz*2;k++){
			trasp[count]= trasp[count-1]+1; count++;
		}
	}
	/*Print data to transpose*/
	if(rank==des_rank)printf("\n\nData to double\n");
	for (i=0; i < local_x; i++){
		for(k=0; k<nz*2;k++){
			if (rank==des_rank)printf("[%d] DOUBLE[%d]=%f\n", rank, i*nz*2+k,trasp[i*nz*2+k]);
		}
		if(rank==des_rank)printf("-----------------------------\n");
	}

	/*Transpose*/
	fftw_execute(transpose);
	/*Print transposed array*/
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank==des_rank)printf("\n\n\n\n\nTransposed\n");
	for(i=0; i < nx; i++){
		for(k=0; k<local_z*2; k++){
			if (rank==des_rank)printf("[%d] trasp[%d]=%f\n", rank, i*(local_z*2)+k,trasp[i*(local_z*2)+k] );
		}
		if(rank==des_rank)printf("-------------\n");
	}

	MPI_Finalize();
}
