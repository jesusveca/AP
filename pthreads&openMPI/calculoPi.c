#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <math.h>

#define MAX 1E8
#define MIN 1E-8

int main (int argc, char* argv[]){
    int myRank;
    int size;
    int i;
    int resultadoFinal=0;
    int sumaTotal=0;
    double pi=0.0;
    double inicio=0.0;
    double final=0.0;
    double x; 
    double y;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, & myRank); //Get ID del proceso
    MPI_Comm_size (MPI_COMM_WORLD, &size); //Get numero de proceso

    inicio = MPI_Wtime();
    srand((int)time(0));

    for (i=myRank; i<MAX; i+=size)  {    // cada proceso calculara una parte de la suma
        x=rand()/(RAND_MAX+1.0); y=rand()/(RAND_MAX+1.0);    // por lo menos 32767
        if((x*x+y*y)<1.0)   resultadoFinal++;
    }
    // los reduce todos al proceso root
    MPI_Reduce(&resultadoFinal, &sumaTotal, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);      //Suma todos los resultados
    final = MPI_Wtime();
    if (myRank==0){       //Calcula e imprime PI
        pi=4*MIN*sumaTotal; printf("Tiempo=%fs;    PI=%0.20f\n", final-inicio, pi);
    }
    MPI_Finalize();
    return 0;
}