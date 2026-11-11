#include "student.h"
#include <stdlib.h>
#include <mpi.h>

static double get_rand_minus_one_one(void) {
    return 2 * (rand() / (double)RAND_MAX) - 1;
}

/* Replace this sequential baseline with an MPI implementation.
 * num_tosses is the total number of tosses across all ranks.
 * MPI is already initialized; rank 0 prints the returned estimate.
 */
double estimate_pi(int num_tosses, int rank, int size) {
    long local_hits = 0;
    long local_tosses = num_tosses/size;
    long remainder = num_tosses % size;
    if (rank < remainder) {
        local_tosses++;
    }
    srand(rank);
    for (long toss = 0; toss < local_tosses; toss++) {
        double x = get_rand_minus_one_one();
        double y = get_rand_minus_one_one();
        if (x*x + y*y <= 1) {
            local_hits++;
        }
    }
    long num_hits = 0;
    MPI_Reduce(
        &local_hits,
        &num_hits,
        1,
        MPI_LONG,
        MPI_SUM,
        0,
        MPI_COMM_WORLD
    );
    if (rank == 0)
        return (double)(4.0 * num_hits) / (double)num_tosses;
    return 0.0;   /* only rank 0's value is used by the harness */
}
