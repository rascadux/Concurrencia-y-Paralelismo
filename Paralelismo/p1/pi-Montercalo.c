#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char *argv[]){

  int i, j, done= 0, n= 0, count, my_id, numPtsPorProceso;  
  double PI25DT = 3.141592653589793238462643;
  double pi, x, y, z;
  int numprocs , rank ;

  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD , &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD , &rank);

  while (!done){
    if(rank == 0){
      sleep(1);
      printf("Enter the number of points: (0 quits) \n");
      scanf("%d",&n);

      int numPts = 0;
      numPtsProceso = n/numprocs;

      for(i = 1; i < numprocs; i++) {
        MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        numPts += numPtsPorProceso;
        }

        numPtsPorProceso = n - numPts;

    }else {
      MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      numPtsPorProceso= n/numprocs;
    }

    if (n == 0) break;
    count = 0;

    for (j = rank+1; j <= n; j+=numprocs) {  // Get the random numbers between 0 and 1
      x = ((double) rand()) / ((double) RAND_MAX);
      y = ((double) rand()) / ((double) RAND_MAX);

      z = sqrt((x*x)+(y*y));    // Calculate the square root of the squares

      if(z <= 1.0)    // Check whether z is within the circle
        count++;

    }

    if(rank == 0) {
      int countFin = count;
      for(i = 1; i < numprocs-1; i++) {
          MPI_Recv(&count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
          countFinal= countFinal + count;
      }
      pi = ((double) countFin/(double) n)*4.0;
      printf("Pi is approx. %.16f. Error is %.16f\n", pi, fabs(pi - PI25DT));
    }else{
      MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }

  MPI_Finalize();
}
