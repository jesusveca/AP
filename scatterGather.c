#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *vector=NULL;
    int dato;

    if (rank == 0) {
        vector = malloc(size * sizeof(int) );
        for (int i=0; i<size; i++)
            vector[i] = rand() % 10 + 1;

        printf("Proceso %d tiene: ", rank);
        for (int i=0; i<size; i++)
            printf("%d ", vector[i]);
        printf("\n");
    }

    MPI_Scatter(vector, 1, MPI_INT, &dato, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Proceso %d tiene %d\n", rank, dato);
    dato *= 3;
    printf("Proceso %d multiplica el dato por 3 y tiene %d\n", rank, dato);

    MPI_Gather(&dato, 1, MPI_INT, vector, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Proceso %d tiene: ", rank);
        for (int i=0; i<size; i++)
            printf("%d ", vector[i]);
        printf("\n");
    }

    if (rank == 0)
        free(vector);

    MPI_Finalize();
    return 0;
}