#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "student.h"

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (argc != 2 || atoi(argv[1]) <= 0) {
        if (rank == 0) fprintf(stderr, "Usage: %s <positive_num_tosses>\n", argv[0]);
        MPI_Finalize();
        return EXIT_FAILURE;
    }
    int num_tosses = atoi(argv[1]);
    srand((unsigned)time(NULL) + (unsigned)rank);
    double pi_estimate = estimate_pi(num_tosses, rank, size);
    if (rank == 0) printf("Estimate of pi = %f\n", pi_estimate);
    MPI_Finalize();
    return 0;
}
