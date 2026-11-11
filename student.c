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
	double *darts_x = malloc(num_tosses * sizeof *darts_x);
	double *darts_y = malloc(num_tosses * sizeof *darts_y);
	for (int i = 0; i < num_tosses; i++) {
		darts_x[i] = get_rand_minus_one_one();
		darts_y[i] = get_rand_minus_one_one();
	}
	if (rank != 0) {
		MPI_Send(darts_x, num_tosses, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		MPI_Send(darts_y, num_tosses, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
	} else {
		size_t total = (size_t)num_tosses * (size_t)size;
		double *aggregated_darts_x = malloc(total * sizeof *aggregated_darts_x);
		double *aggregated_darts_y = malloc(total * sizeof *aggregated_darts_y);

		for (long i = 0; i < num_tosses; i++) {
			aggregated_darts_x[i] = darts_x[i];
			aggregated_darts_y[i] = darts_y[i];
		}
		for (long p = 1; p < size; p++) {
			double *received_x = malloc(num_tosses * sizeof *aggregated_darts_x);
			double *received_y = malloc(num_tosses * sizeof *aggregated_darts_y);
			MPI_Recv(
				received_x,
				num_tosses,
				MPI_DOUBLE,
				p,
				0,
				MPI_COMM_WORLD,
				MPI_STATUS_IGNORE
			);
			MPI_Recv(
				received_y,
				num_tosses,
				MPI_DOUBLE,
				p,
				1,
				MPI_COMM_WORLD,
				MPI_STATUS_IGNORE
			);
			for (long i = 0; i < num_tosses; i++) {
				aggregated_darts_x[p*num_tosses+i] = received_x[i];
				aggregated_darts_y[p*num_tosses+i] = received_y[i];
			}
		}
		long num_hits = 0;
		for (long i = 0; i < num_tosses*size; i++) {
			double x = aggregated_darts_x[i];
			double y = aggregated_darts_y[i];
			num_hits += (x*x + y*y <= 1);
		}
		return num_hits / (double)num_tosses;
	}
	return 0;

}
