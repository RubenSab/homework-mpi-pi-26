#ifndef MPI_PI_STUDENT_H
#define MPI_PI_STUDENT_H

/* Called by every rank. Return the global estimate on rank 0. */
double estimate_pi(int num_tosses, int rank, int size);

#endif
