#include <stdlib.h>
#include "mpi.h"
#include "fftw3-mpi.h"

#define N 9
int main(int argc, char **argv) {

	MPI_Init(&argc,&argv);
	fftw_mpi_init();
	int rank,size,i,k;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	/*Declare var*/
	fftw_plan plan, transpose;
	ptrdiff_t alloc_local, local_x, local_x_start,local_z, local_z_start;
	double *trasp;	fftw_complex *data;
	
													/* x   ,z */
	alloc_local = fftw_mpi_local_size_2d_transposed (N/2+1, N, 
                                          MPI_COMM_WORLD, 
                                          &local_x, &local_x_start, &local_z, &local_z_start);
	printf("[%d]\tlocal_x= %d\tlocal_x_start= %d\n\tlocal_z= %d\tlocal_z_start= %d\n\n", rank, local_x, local_x_start, local_z, local_z_start);

	data = fftw_alloc_complex (alloc_local*5);
	for(i=0; i < 6; i++){
		for(k=0; k<N; k++){
			data[i*N+k][0] = 2*(k+(i/2)*N);
			data[i*N+k][1] = 2*(k+(i/2)*N)+1;
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	/*for(i=0; i < local_x; i++){
		for(k=0; k<N; k++){
			if (rank==0) printf("[%d] data[%d]= %f+I*%f\n", rank, k+i*N, data[i*N+k][0], data[i*N+k][1] );
		}
		if (rank==0) printf("--------------------\n");
	}
	MPI_Barrier(MPI_COMM_WORLD);
	for(i=0; i < local_x; i++){
		for(k=0; k<N; k++){
			if (rank==1) printf("[%d] data[%d]= %f+I*%f\n", rank, k+i*N, data[i*N+k][0], data[i*N+k][1] );
		}
		if (rank==1) printf("--------------------\n");
	}*/

	int count=1;
	trasp = fftw_alloc_real (2*alloc_local);
	transpose=fftw_mpi_plan_transpose(N/2+1, N,
                                  trasp, trasp,
                                  MPI_COMM_WORLD, FFTW_MPI_TRANSPOSED_OUT);
	trasp[0]= local_x_start*N;
	for (i=local_x_start; i < local_x_start+local_x; i++){
		for(k=0; k<N*2;k++){
			//trasp[count]= data[(i)*N+k][0];	count++;
			//trasp[count]= data[(i)*N+k][1];	count++;
			trasp[count]= trasp[count-1]+1; count++;
		}
	}
	/*Print data to transpose*/
	printf("\n\nData to double\n");
	for (i=0; i < local_x; i++){
		for(k=0; k<N;k++){
			if (rank==0)printf("[%d] DOUBLE[%d]=%f\n", rank, i*N+k,trasp[i*N+k]);
		}
		printf("-----------------------------\n");
	}

	/*Transpose*/
	fftw_execute(transpose);
	/*Print transposed array*/
	MPI_Barrier(MPI_COMM_WORLD);
	printf("\n\n\n\n\nTransposed\n");
	for (i=0; i < local_z; i++){
		for(k=0; k<N/2+1;k++){
			if (rank==0)printf("[%d] trasp[%d]=%f\n", rank, i*(N/2+1)+k,trasp[i*(N/2+1)+k] );
		}
	}

	MPI_Finalize();
}
