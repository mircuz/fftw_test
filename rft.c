#include <stdlib.h>
#include "mpi.h"
#include "fftw3-mpi.h"

#define N 12
int main(int argc, char **argv) {

	MPI_Init(&argc,&argv);
	fftw_mpi_init();
	int rank,size;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	
	fftw_complex *comp;
	fftw_plan pb, pf;
	double *real;
	comp = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (N+2));
	real = (double*) fftw_malloc(sizeof(double) * N*2);
	pb = fftw_plan_dft_c2r_1d(24, comp, real, FFTW_MEASURE);
	int i=0;
	for(i=0; i<5; i++){
		comp[i][0]= 2*i+8;
		comp[i][1]= 2*i+1+8;
		printf("comp[i]= %f+I*%f\n", comp[i][0], comp[i][1]);	
	}
	for(i=5; i< N-4;i++){
		comp[i][0]=0;
		comp[i][0]=0;
		printf("comp[i]= %f+I*%f\n", comp[i][0], comp[i][1]);	
	}
	for(i=N-4; i<N; i++){
		comp[i][0]= 2*i-2*8;
		comp[i][1]= 2*i+1-2*8;
		printf("comp[i]= %f+I*%f\n", comp[i][0], comp[i][1]);	
	}
   
    fftw_execute(pb); /* repeat as needed */
	for(i=0; i<(N+3)*2; i++){
		printf("in[i]= %f\n", real[i]);
	}
 	
	for(i=0; i < 24; i++){
		real[i]=real[i]*real[i];
		//printf("conv[%d]=%f\n", i,real[i]);	
	}
	

	pf = fftw_plan_dft_r2c_1d(24, real, comp, FFTW_ESTIMATE);
	fftw_execute(pf);
	for (i=0; i < 12; i++){
		comp[i][0] = comp[i][0]/24;	comp[i][1] = comp[i][1]/24;
		printf("results[i]= %f+I*%f\n", comp[i][0], comp[i][1]);
	}



    fftw_destroy_plan(pf);	 fftw_destroy_plan(pb);
    fftw_free(comp); free(real);
	
	fftw_mpi_cleanup();
	MPI_Finalize();

}
