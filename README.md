# MPI Monte Carlo pi exercise

Implement `estimate_pi` in `student.c` to compute pi using MPI. The supplied implementation is a sequential baseline that each rank repeats; partition the total `num_tosses` across ranks and combine their hit counts. Every rank calls the function; its return value on rank 0 is printed.

Edit `student.c` and `student.mk`. You may add headers and helper functions there, while preserving the interface in `student.h`. The driver initializes/finalizes MPI and seeds each rank's random generator.

Build with `make`, run `mpirun -n 4 ./pi_parallel 1000000`, and use `make test` for the approximate correctness check. `pi_seq.c` is the sequential reference.


## Custom build settings

Edit `student.mk` to customize the build without changing the protected `Makefile`. For example:

```make
CFLAGS += -O3
CPPFLAGS += -DMY_OPTION=1
# LDLIBS += -lmy_library
```

Available settings are `CC`, `CPPFLAGS`, `CFLAGS`, `LDFLAGS`, and `LDLIBS`. Use `+=` to extend the defaults or `=` to replace them. Command-line settings take precedence, for example `make CFLAGS="-O2 -Wall"`. Changes to `student.mk` automatically rebuild the programs; after changing only command-line settings, run `make clean` before rebuilding. The grader uses the submitted `student.mk` too.

## Local smoke tests

`make test` checks output correctness using Python 3. Run `make test FULL=1`
to check all supplied fixtures. MPI tests default to four ranks; override the
launcher with, for example, `MPIEXEC="mpirun -np 2" make test`.
Set `TEST_TIMEOUT` to change the per-run timeout (default: 300 seconds).
These checks do not verify parallelism or prevent hardcoded answers.
