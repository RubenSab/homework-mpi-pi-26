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
    long local_tosses = num_tosses/size;
    local_tosses += rank < (num_tosses % size);
    srand(rank);
    long local_hits = 0;
        for (long toss = 0; toss < local_tosses; toss++) {
        double x = get_rand_minus_one_one();
        double y = get_rand_minus_one_one();
        local_hits += (x*x + y*y <= 1);
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
    return 0.0;
}
